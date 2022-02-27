#ifndef FLIGHT_H
#define FLIGHT_H

#include <QString>
#include <QList>

#include "point.h"

class Flight
{
public:
    Flight(const Flight&);
    Flight(int, int, int, int, int, int, int);
    friend ostream& operator<<(ostream&, const Flight&);
    friend bool operator==(const Flight&, const Flight&);
    friend bool operator!=(const Flight&, const Flight&);

    // Getters
    Point getX0() const;
    Point getX1() const;
    Point getXt(int);
    double getHdg (int);
    int getStartTime() const;
    int getSpeed() const;
    double getArvTime() const;
    double getRange() const;
    double getHeading() const;
    QString &getFlightNumberStr();
    QList<Point> &getPreviousPositions();

    // Setters
    void updatePreviousPositions(Point newPoint);
    void setTurn(const QList<int> &newTurn);

    // Methods
    bool isInConflict(QList<Flight> ,int);
    bool isDefault();
    double distance(Flight, int); // Compute distance between 2 airplanes for a given time

    const QList<Point> &getRoute() const;

private:
    // Native attributes
    Point X0;
    Point X1;
    QList<Point> route;
    int startTime;
    int speed;

    // Computed attributes
    double arvTime;
    double range;
    double heading;
    QString flightNumberStr;
    QList<Point> previousPositions;
    QList<int> turn;

    // private methods
    bool isDetourned();
    void setRoute();
    void setRange();
    void setArvTime();

};

#endif // FLIGHT_H
