#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "flight.h"

/*
    Conflict solver with genetic algorithm
*/
class GeneticAlgorithm
{
public:
    GeneticAlgorithm(int, int, int);
    void printConflicts(QList<Flight>, int);
    void solve();

private:
    const int minTurn = -60;
    const int maxTurn = 60;
    int popSize;
    int maxGenerations;
    int mutationRatio;
    QList<int> conflictList;

    QList<QList<int>> select(QList<QList<int>>);
    double fitnessFunction(QList<int>);
    int getTotalConflictTime();        //ms
    int getTotalDelay();               //ms

};

#endif // GENETICALGORITHM_H
