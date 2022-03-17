#include "individual.h"

Individual::Individual(QString id, int turnBeginTime, int turnRadius, int turnEndTime)
{
    this->id = id;
    this->turnBeginTime = turnBeginTime;
    this->turnRadius = turnRadius;
    this->turnEndTime = turnEndTime;
}

ostream &operator<<(ostream &os, const Individual &i)
{
    os << "Individual(id=" << i.id.toStdString() << ", t0=" << i.turnBeginTime << ", alpha=" << i.turnRadius << ", t1=" << i.turnEndTime << ")";
    return os;
}

ostream &operator<<(ostream &os, const QList<Individual> &indList)
{
    os << "List: " << indList.length() << " items" << endl;
    for (Individual i : indList)
    {
        os << i;
    }
    os << endl;
    return os;
}

bool Individual::isNull()
{
    return (this->turnBeginTime == this->turnEndTime) && (this->turnRadius == 0);
}

bool Individual::validate(int t) {
    return turnBeginTime < turnEndTime &&
           turnBeginTime > t &&
           turnEndTime > t &&
           turnRadius >= -60 &&
           turnRadius <= 60;
}

int Individual::getTurnBeginTime() const
{
    return turnBeginTime;
}

int Individual::getTurnRadius() const
{
    return turnRadius;
}

int Individual::getTurnEndTime() const
{
    return turnEndTime;
}

const QString &Individual::getId() const
{
    return id;
}
