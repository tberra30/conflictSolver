#ifndef FLIGHT_H
#define FLIGHT_H

#include <QString>
#include <QList>

#include "point.h"

/*
 * Flight object builder, with all required or util informations
*/
class Flight
{
public:
    Flight(const Flight& flight);
    Flight(int x0, int y0, int x1, int y1, int startTime, int speed, int flightNumber);
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
    int getArvTime() const;
    double getRange() const;
    double getHeading() const;
    QString &getFlightNumberStr();
    QList<Point> &getPreviousPositions();
    int getLeg(int time);

    // Setters
    void updatePreviousPositions(int time);
    void setTurn(const QList<int> &newTurn);

    // Methods
    bool isInConflict(QList<Flight> ,int);
    bool hasTurned();
    double distance(Flight, int); // Compute distance between 2 airplanes for a given time
    int computeDelay();

    const QList<Point> &getRoute() const;

    const QList<int> &getLegTimes() const;

private:
    // Native attributes
    Point X0;
    Point X1;
    QList<Point> route;
    QList<int> legTimes;
    int startTime;
    int speed;

    // Computed attributes
    int arvTime;
    double range;
    double heading;
    QString flightNumberStr;
    QList<Point> previousPositions;
    QList<int> turn;

    // private methods
    void setRoute();
    void setRange();
    void setArvTime();
    void setTimes();

};

#endif // FLIGHT_H
