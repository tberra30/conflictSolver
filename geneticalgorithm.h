#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "Constants.h"
#include "flight.h"
#include "scenariogenerator.h"
#include "individual.h"

/*
    Conflict solver with genetic algorithm
*/
class GeneticAlgorithm
{
public:
    GeneticAlgorithm(int maxGen, int populationSize, double Pm, double Pc);
    QList<Individual> run(QList<Flight> listFl, int t0);
    void reset();

    bool getConflictDetected();
    void setActive(bool newActive);
    int t;
    QList<Flight> flights;
    void generatePopulation();
    void selection();
    void cross();
    void mutation();
    static int getT();
    QList<QList<Individual>> population;
    static int getMaxIndex(QList<double> fitnessList, QList<int> ignoredIndexes);

private:
    static const int Hpred = H_PRED * TO_SEC * TO_MILLISEC; //millisec
    static const int dSep = 1; // incertitude : 1 Nm/min
    static const int maxTurn = 60;
    static const int minTurn = -60;
    bool active;
    int popSize;
    int numGenerations;
    double Pm;
    double Pc;
    bool conflictDetected = false;


    static double sep(double tMin);
    QList<Flight> applySolution(QList<Individual> solution);
    void sortPopulation();
    Individual generateIndividual(QString id);
    QList<Individual> generateIndividuals();
    QList<Individual> crossOver(QList<Individual> i1, QList<Individual> i2);
    QList<Individual> mutate(QList<Individual> i);
    int solve(Point A, Point B, Point C, Point D, int t, int h);
    int computeConflicts(QList<Flight> listFl, int t0);
    double computeFitness(QList<Individual> indList);

};

#endif // GENETICALGORITHM_H
