#include "Constants.h"
#include "geneticalgorithm.h"
#include "quadraticequationsolver.h"
#include <QtGlobal>
#include <QDebug>
#include <qset.h>

GeneticAlgorithm::GeneticAlgorithm(int maxGen, int populationSize, double Pm, double Pc)
{
    this->numGenerations = maxGen;
    this->popSize = populationSize;
    this->Pm = Pm;
    this->Pc = Pc;
    this->active = true;
}

Individual GeneticAlgorithm::generateIndividual(QString id)
{
    // type : (t1, radius, t2)
    int sign = (ScenarioGenerator::randomNumber(0,1) == 0) ? -1 : 1;
    int radius = sign * ScenarioGenerator::randomNumber(20, 60);
    int startTurn = ScenarioGenerator::randomNumber(0, Hpred);
    int endTurn = ScenarioGenerator::randomNumber(startTurn + 3 * TO_SEC * TO_MILLISEC, Hpred);
    return Individual(id, startTurn, radius, endTurn);
}

void GeneticAlgorithm::reset() {
    conflictDetected = false;
    flights.clear();
    population.clear();
}

QList<Individual> GeneticAlgorithm::generateIndividuals()
{
    QList<Individual> indList;
    for (Flight fl : this->flights) {
        indList.append(generateIndividual(fl.getFlightNumberStr()));
    }
    return indList;
}

void GeneticAlgorithm::generatePopulation()
{
    QList<QList<Individual>> beginPopulation;
    for (int i = 0; i < popSize; i ++) {
        beginPopulation.append(generateIndividuals());
    }
    population = beginPopulation;
}

QList<Individual> GeneticAlgorithm::run(QList<Flight> listFl, int t0) {
    QList<Flight> noTurnAircrafts;
    for (Flight f : listFl) {
        if (!f.hasTurned()) {
            noTurnAircrafts.append(f);
        }
    }

    this->t = t0;
    this->flights = noTurnAircrafts;

    conflictDetected = computeConflicts(listFl, t0) > 0;

    if (conflictDetected && active && flights.length() > 0) {
        // Init
        generatePopulation();

        // Iterate
        for (int i = 0; i < numGenerations; i ++) {
            double rand = ScenarioGenerator::random();
            bool crossChoose = (rand <= Pc);
            bool mutationChoose = (rand > Pc && rand <= Pc + Pm);

            sortPopulation();
            selection();
            if (crossChoose) cross();
            if (mutationChoose) mutation();
        }

        sortPopulation();

        // return the best individual
        //cout << "ind : "  << population[0] << endl;
        //cout << "fitness : "  << computeFitness(population[0]) << endl;
        return population[0];
    }

    return QList<Individual>();
}

bool GeneticAlgorithm::getConflictDetected()
{
    return conflictDetected;
}

void GeneticAlgorithm::setActive(bool newActive)
{
    active = newActive;
}

QList<int> mergeLists(int t, int th, QList<int> l1, QList<int> l2) {
    QList<int> result;
    QList<int> longer = (l1.length() > l2.length()) ? l1 : l2;
    QList<int> shorter = (l1.length() <= l2.length()) ? l1 : l2;

    for (int i = 0; i < shorter.length(); i ++) {
        if (l1[i] > l2[i]) {
            if (l2[i] >= t && l2[i] <= th && !result.contains(l2[i])) result.push_back(l2[i]);
            if (l1[i] >= t && l1[i] <= th && !result.contains(l1[i])) result.push_back(l1[i]);
        } else {
            if (l1[i] >= t && l1[i] <= th && !result.contains(l1[i])) result.push_back(l1[i]);
            if (l2[i] >= t && l2[i] <= th && !result.contains(l2[i])) result.push_back(l2[i]);
        }
    }

    for (int i = shorter.length(); i < longer.length(); i ++) {
        if (longer[i] >= t && longer[i] <= th && !result.contains(longer[i])) result.push_back(longer[i]);
    }

    if (!result.contains(t)) result.push_front(t);
    if (!result.contains(th)) result.push_back(th);

    return result;
}

int GeneticAlgorithm::getMaxIndex(QList<double> fitnessList, QList<int> ignoredIndexes) {
    int maxIndex = 0;
    double max = fitnessList[0];

    for (int i = 0; i < fitnessList.length(); i ++) {
        if (fitnessList[i] > max && !ignoredIndexes.contains(i)) maxIndex = i;
    }

    return maxIndex;
}

void GeneticAlgorithm::sortPopulation() {
    QList<QList<Individual>> popTemp = population;
    QList<double> fitnesses;
    QList<int> ignored;

    for (int i = 0; i < popSize; i ++) {
        fitnesses.push_back(computeFitness(population[i]));
    }

    for (int i = 0; i < popSize - 1; i ++) {
        int indexToMove = getMaxIndex(fitnesses, ignored);
        if (indexToMove >= 0) population[i] = popTemp[indexToMove];
        if (indexToMove >= 0) ignored.append(indexToMove);
    }
}

double GeneticAlgorithm::sep(double tMillisec) {
    return SEP0 + (tMillisec / TO_SEC * TO_MILLISEC) * dSep;
}

int GeneticAlgorithm::solve(Point A, Point B, Point C, Point D, int dt1, int dt2) {
    int conflictTime = 0;
    int h = (dt2 - dt1);
    int hMin = h / (TO_SEC * TO_MILLISEC);

    Point U = C - A;
    Point V = (D - C) - (B - A);

    double sep1 = sep(dt1);
    double a = pow(V.getDist(), 2) - pow(dSep * hMin, 2);
    double b = 2 * (Point::scalarProd(V, U) - sep1 * dSep * hMin);
    double c = pow(U.getDist(), 2) - pow(sep1, 2);

    QuadraticEquationSolver solver(a, b, c);
    double r1 = solver.getR1();
    double r2 = solver.getR2();

    if (isnan(r1) && isnan(r2)) {
        conflictTime = 0;
    } else if (!isnan(r1) && isnan(r2)) {
        // Case linear
        conflictTime = (b < 0) ? (1 - qMin(r1, 1.0)) * h : qMax(r1, 0.0) * h;
    } else if (!isnan(r1) && !isnan(r2)) {
        // Solving into [0, 1] only
        double realR1 = qMin(qMax(r1, 0.0), 1.0);
        double realR2 = qMax(qMin(r2, 1.0), 0.0);
        if (a > 0) {
            conflictTime = qFloor(h * (realR2 - realR1));
        } else {
            conflictTime = qFloor((1 - realR2) * h);
        }
    } else {
        conflictTime = 0;
    }

    if (conflictTime < 0) conflictTime = 0;
    return conflictTime;

}

int GeneticAlgorithm::computeConflicts(QList<Flight> listFl, int t) {
    double conflictTime = 0;
    QList<QString> doneCouples;

    for (Flight f1 : listFl) {
        for (Flight f2 : listFl) {
            bool cond1 = f1 != f2;
            bool cond2 = !doneCouples.contains(f1.getFlightNumberStr() + f2.getFlightNumberStr());

            if (cond1 && cond2) {
                // If out of window scope : truncate th
                int tH1 = (t + Hpred > f1.getArvTime()) ? f1.getArvTime() : t + Hpred;
                int tH2 = (t + Hpred > f2.getArvTime()) ? f2.getArvTime() : t + Hpred;
                int th = qMin(tH1, tH2);

                QList<int> times = mergeLists(t, th, f1.getLegTimes(), f2.getLegTimes());
                for (int currentLeg = 0; currentLeg < times.length() - 1; currentLeg ++) {
                    int currentTime = times[currentLeg];
                    int nextCurrentTime = times[currentLeg + 1];

                    Point A = f1.getXt(currentTime);
                    Point B = f1.getXt(nextCurrentTime);
                    Point C = f2.getXt(currentTime);
                    Point D = f2.getXt(nextCurrentTime);

                    conflictTime += solve(A, B, C, D, currentTime - t, nextCurrentTime - t);
                };
            }

            doneCouples.append(f1.getFlightNumberStr() + f2.getFlightNumberStr());
            doneCouples.append(f2.getFlightNumberStr() + f1.getFlightNumberStr());
        }
    }

    return conflictTime;
}

void GeneticAlgorithm::cross() {
    for (int i = 1; i < popSize - 1; i ++) {
        population.append(crossOver(population[i], population[i+1]));
    }
}

void GeneticAlgorithm::mutation() {
    for (int i = 0; i < popSize - 1; i ++) {
        population.append(mutate(population[i]));
    }
}

void GeneticAlgorithm::selection() {
    while (population.length() > popSize) {
        population.pop_back();
    }
}

QList<Flight> GeneticAlgorithm::applySolution(QList<Individual> solution) {
    QList<Flight> newFlights;
    for (Flight flight : flights) {
        for (Individual ind : solution) {
            if (ind.getId() == flight.getFlightNumberStr()) {
                Flight newFlight = flight;
                newFlight.setTurn(QList{
                                 t + ind.getTurnBeginTime(),
                                 ind.getTurnRadius(),
                                 t + ind.getTurnEndTime()});
                newFlights.append(newFlight);
            }
        }
    }
    return newFlights;
}

QList<Individual> GeneticAlgorithm::crossOver(QList<Individual> li1, QList<Individual> li2) {
    QList<Individual> newSol;
    for (int i = 0; i < li1.length(); i ++) {
        newSol.append(Individual(li1[i].getId(),
                                 li1[i].getTurnBeginTime(),
                                 li2[i].getTurnRadius(),
                                 li1[i].getTurnEndTime()));
    }
    return newSol;
}

QList<Individual> GeneticAlgorithm::mutate(QList<Individual> li) {
    QList<Individual> mutateIndList;
    QString flightToMutate = QString::number(ScenarioGenerator::randomNumber(1, li.length()));

    for (int i = 0; i < li.length(); i ++) {
        Individual ind = li[i];

        if (ind.getId() == flightToMutate.rightJustified(4, '0')) {
            mutateIndList.append(generateIndividual(ind.getId()));
        } else {
            mutateIndList.append(ind);
        }
    }

    return mutateIndList;
}

/*
 * Compute fitness rate
 */
double GeneticAlgorithm::computeFitness(QList<Individual> indList) {
    double fitness = 0;
    QList<Flight> tempFlightList = applySolution(indList);
    int conflictTime = computeConflicts(tempFlightList, t);

    double conflictTimeSec = (double) conflictTime / TO_SEC;

    if (conflictTime > 0) {
        fitness = (double) 1 / (2 + conflictTimeSec);
    } else {
        int totalDelay = 0;
        for (Flight fl : tempFlightList) {
            totalDelay += fl.computeDelay() / TO_SEC;
        }
        fitness = (double) 1/2 + (double) 1/(2 + totalDelay);
    }

    return fitness;
}
