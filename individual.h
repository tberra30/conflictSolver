#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include <iostream>
#include <QString>
#include "flight.h"

using namespace std;

/*
    Conflict solver with genetic algorithm
*/
class Individual
{
public:
    Individual(QString id, int turnBeginTime, int turnRadius, int turnEndTime);

    friend ostream &operator<<(ostream &, const Individual &);
    friend ostream& operator<<(ostream&, const QList<Individual>&);

    int getTurnBeginTime() const;
    int getTurnRadius() const;
    int getTurnEndTime() const;
    const QString &getId() const;
    int getTime() const;
    const QList<Flight> &getFlightList() const;
    bool isNull();
    bool validate(int t);

private:
    QString id;
    int turnBeginTime;
    int turnRadius;
    int turnEndTime;
};

#endif // GENETICALGORITHM_H
