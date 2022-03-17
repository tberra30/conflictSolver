#include "flight.h"
#include "Constants.h"
#include <QtMath>
#include <QDebug>
#include <iostream>

Flight::Flight(const Flight& flight) {
    this->X0 = Point(flight.getX0().x(), flight.getX0().y());
    this->X1 = Point(flight.getX1().x(), flight.getX1().y());
    this->route = flight.route;
    this->startTime = flight.getStartTime();
    this->speed = flight.getSpeed();
    this->range = flight.getRange();
    this->arvTime = flight.getArvTime();
    this->heading = flight.getHeading();
    this->flightNumberStr = flight.flightNumberStr;
    this->legTimes = flight.getLegTimes();

    for (Point p : flight.previousPositions) {
        this->previousPositions.append(p);
    }

    this->turn = flight.turn;
}

Flight::Flight(int x0, int y0, int x1, int y1, int startTime, int speed, int flightNumber)
{
    this->X0 = Point(x0, y0);
    this->X1 = Point(x1, y1);
    this->route = QList({X0, X1});
    this->startTime = startTime * TO_SEC * TO_MILLISEC;
    this->speed = speed;

    // Computed attributes
    this->range = Point::getRange(X1, X0);
    this->setArvTime();
    this->legTimes = QList({startTime, arvTime});
    this->heading = Point::getRadius(X0, X1);

    this->flightNumberStr = QString::number(flightNumber).rightJustified(4, '0');

    this->turn = QList{0,0,0};
}

Point Flight::getX0() const
{
    return X0;
}

Point Flight::getX1() const
{
    return X1;
}

int Flight::getStartTime() const
{
    return startTime;
}

int Flight::getSpeed() const
{
    return speed;
}

QList<Point> &Flight::getPreviousPositions()
{
    return previousPositions;
}

int Flight::getLeg(int time) {
    int leg = -1;
    double dist = 0.0;

    double advance = (double) (time - startTime) / (arvTime - startTime); // in  [0, 1]
    double distDone = range * advance;
    QList<double> legRanges{dist};

    if (advance < 0 || advance > 1) {
        qWarning() << "leg = -1 !!! Aircraft outside timescope, leg : " + QString::number(leg);
    } else {
        // Found leg number
        do {
            leg ++;
            dist += Point::getRange(route[leg], route[leg+1]);
            legRanges.append(dist);
        } while (dist < distDone && leg < route.length() - 1);

    }

    return leg;
}

double Flight::getHdg(int time) {
    int legNumber = getLeg(time);
    Point A = route[legNumber];
    Point B = route[legNumber + 1];
    return Point::getRadius(A, B);
}


/*
 * Return the position of the aircraft at time
 */
Point Flight::getXt(int time) {
    int legNumber = getLeg(time);

    Point A = route[legNumber];
    Point B = route[legNumber + 1];
    Point v_AB = B - A;

    // [kts] = N/h => min = N/N/h * 60
    double D1 = 0;
    int i = 0;
    while(i < legNumber) {
        D1 += Point::getRange(route[i], route[i+1]);
        i++;
    }

    double D2 = D1 + Point::getRange(route[i], route[i+1]);
    int t0 = TO_SEC * TO_SEC * TO_MILLISEC * (D1 / speed) + this->startTime;
    int t1 = TO_SEC * TO_SEC * TO_MILLISEC * (D2 / speed) + this->startTime;

    double advance = (double) (time - t0)/(t1 - t0);

    return A + (v_AB * advance);
}

int Flight::getArvTime() const
{
    return arvTime;
}

double Flight::getRange() const
{
    return range;
}

double Flight::getHeading() const
{
    return heading;
}

QString &Flight::getFlightNumberStr()
{
    return flightNumberStr;
}

ostream& operator<<(ostream& os, const Flight& flt) {
    os << "Flight : A" << flt.X0 << " -> "
             << "B" << flt.X1 << ", speed : " << flt.speed
             << ", heading : " << flt.getHeading() << "rad"
             << ", start : " << flt.startTime/(TO_SEC*TO_MILLISEC) << " min, end :" << flt.arvTime/(TO_SEC*TO_MILLISEC)
             << "min , flight Number : " << flt.flightNumberStr.toUtf8().constData()
             << ", turn : " << flt.turn[0] << ", " << flt.turn[1] << ", " << flt.turn[2] << endl;
    return os;
}

bool operator==(const Flight& f1, const Flight& f2) {
    return f1.flightNumberStr == f2.flightNumberStr;
}

bool operator!=(const Flight& f1, const Flight& f2) {
    return !(f1 == f2);
}

bool Flight::isInConflict(QList<Flight> flightList, int time) {
    for (Flight flight : flightList) {
        if (this->flightNumberStr != flight.flightNumberStr && this->distance(flight, time) < SEP0) {
            return true;
        }
    }

    return false;
}

bool Flight::hasTurned() {
    return route.length() > 2;
}

double Flight::distance(Flight f, int time) {
    return Point::getRange(this->getXt(time), f.getXt(time));
}

int Flight::computeDelay() {
    if (hasTurned()) {
        int t1 = TO_SEC * TO_SEC * TO_MILLISEC * Point::getRange(X0, X1) / speed;
        int t2 = arvTime - startTime;
        return t2 - t1;
    } else {
        return 0;
    }
}

const QList<Point> &Flight::getRoute() const
{
    return route;
}

const QList<int> &Flight::getLegTimes() const
{
    return legTimes;
}

void Flight::setTurn(const QList<int> &newTurn)
{
    turn = newTurn;
    setRoute();
    setRange();
    setArvTime();
    setTimes();
}

void Flight::setRoute() {
    double t0 = turn[0];
    double alpha = turn[1];
    double t1 = turn[2];
    double segLenght = (double)(speed * (t1 - t0))/ (TO_SEC * TO_SEC * TO_MILLISEC);
    double alphaRad = qDegreesToRadians(alpha);

    Point B = getXt(t0);
    Point C = B + Point::toPointFromPolar(segLenght, heading - alphaRad);

    route.pop_back();
    route << B << C << X1;
}

void Flight::setRange() {
    double rangeTemp = 0.0;
    for (int i = 0; i < route.length() - 1; i++) {
        rangeTemp += Point::getRange(route[i], route[i+1]);
    }
    range = rangeTemp;
}

void Flight::setArvTime() {
    // millisec = millisec + 3600 * (Nm/Nm/1h)
    arvTime = startTime + TO_SEC * TO_SEC * TO_MILLISEC * (range / speed);
}

void Flight::setTimes() {
    double t0 = turn[0];
    double t1 = turn[2];

    legTimes.pop_back();
    legTimes << t0 << t1 << arvTime;
}

void Flight::updatePreviousPositions(int time)
{
    Point newPoint = getXt(time);
    if (previousPositions.length() == 0 || Point::getRange(previousPositions[0], newPoint) > 1.5) {
        if (previousPositions.length() == 5) {
            previousPositions.pop_back();
        }

        previousPositions.push_front(newPoint);
    }
}
