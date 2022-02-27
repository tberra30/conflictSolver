#include "Constants.h"
#include "geneticalgorithm.h"
#include <QtGlobal>
#include <QDebug>

GeneticAlgorithm::GeneticAlgorithm(int max, int mut, int size)
{
    this->maxGenerations = max;
    this->mutationRatio = mut;
    this->popSize = size;
}

void GeneticAlgorithm::printConflicts(QList<Flight> listFl, int t0) {
    QList<int> conflict;
    QList<QList<int>> conflicts;
    QList<QList<QString>> conflictList;
    int dt2 = (10 * TO_SEC * TO_MILLISEC);

    for (Flight f1 : listFl) {
        for (Flight f2 : listFl) {
            bool cond1 = f1 != f2;
            bool cond2 = !conflictList.contains(QList{f2.getFlightNumberStr(), f1.getFlightNumberStr()});
            bool cond3 = !conflictList.contains(QList{f1.getFlightNumberStr(), f2.getFlightNumberStr()});
            if (cond1 && cond2 && cond3) {
                int hdg1 = f1.getHdg(t0);
                int hdg2 = f2.getHdg(t0);
                int spd1 = f1.getSpeed();
                int spd2 = f2.getSpeed();

                double DT = 1;
                double theta = qMax(hdg1, hdg2) - qMin(hdg1, hdg2);

                double a = pow(spd1, 2) + pow(f2.getSpeed(), 2) - 2 * (spd1 * f2.getSpeed() * qCos(theta));
                double b = 2 * DT * (spd1 * spd2 * qCos(theta) - pow(spd2, 2));
                double c = pow(f2.getSpeed(), 2) - pow(SEP0, 2);

                double delta = pow(b, 2) - 4 * a * c;

                if (delta > 0) {
                    double r1 = (sqrt(delta) - b) / (2 * a);
                    double r2 = (-sqrt(delta) - b) / (2 * a);
                    int tBeginConflict = qFloor(t0  + qMax(0.0, r1) * dt2);  //ms
                    int tEndConflict = qFloor(t0  + qMin(1.0, r2) * dt2);  //ms
                    conflict = QList({tBeginConflict, tEndConflict});
                    qDebug() << "conflit " << conflict;
                } else {
                    conflict = QList<int>();
                }

                conflictList.append(QList({f1.getFlightNumberStr(), f2.getFlightNumberStr()}));
                conflicts.append(conflict);
            }
        }
    }

}

QList<QList<int>> GeneticAlgorithm::select(QList<QList<int>> list) {
    QList<QList<int>> newList;
    for (int i = 0; i < popSize; i ++) {
        newList.append(list[i]);
    }
    return newList;
}

double GeneticAlgorithm::fitnessFunction(QList<int>) {
    if (conflictList.length() > 0) {
        return 1 / (2 + getTotalConflictTime());
    } else {
        return 1 / (1 + getTotalDelay());
    }
}

int GeneticAlgorithm::getTotalConflictTime() {
    return 5;
}

int GeneticAlgorithm::getTotalDelay() {
    return 10;
}
