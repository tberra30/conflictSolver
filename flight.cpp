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

    for (Point p : flight.previousPositions) {
        this->previousPositions.append(p);
    }

    this->turn = flight.turn;
}

Flight::Flight(int x0, int y0, int x1, int y1, int startTime, int speed, int flightNumber)
{
    this->X0 = Point(x0, y0);
    this->X1 = Point(x1, y1);
    this->route.append(X0);
    this->route.append(X1);
    this->startTime = startTime;
    this->speed = speed;

    // Computed attributes
    this->range = Point::getRange(X1, X0);
    this->arvTime = startTime + TO_SEC * (range / speed);         // min = min + sec * (Nm/NM/60min)
    double additionalRadius = ((x1-x0) < 0) ? 0 : M_PI;
    this->heading = atan((double)(y1 - y0) / (x1 - x0)) + additionalRadius;

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

double Flight::getHdg(int time) {
    if (!isDetourned()) {
        return heading;
    } else {
        double t0 = turn[0];
        double alpha = qDegreesToRadians(turn[1]);
        double t1 = turn[2];
        double t2 = t1 + (t1 - t0);
        if (time < t0) {
            return heading;
        } else if (time < t1) {
            return heading - alpha;
        } else if (time < t2) {
            return heading + alpha;
        } else {
            return heading;
        }
    }
}

Point Flight::getXt(int time) {
    Point position;
    double timeMin = (double) time / (TO_SEC * TO_MILLISEC);

    double advance = (double) (timeMin - startTime) / (arvTime - startTime); // in  [0, 1]

    if (!this->isDetourned()) {
        // Case no turn
        position = X0 + ((X1 - X0) * advance);
    } else {
        int i = 0;
        double dist = 0;
        QList<double> distances{0};
        double currentDist = advance * range;
        while (dist < currentDist) {
            dist += Point::getRange(route[i], route[i+1]);
            distances.append(dist);
            i ++;
        }

        Point A =  route[i - 1];
        Point B =  route[i];
        double t0 = TO_SEC * (distances[i - 1] / speed);
        double t1 = TO_SEC * (distances[i] / speed);
        Point v_AB = B - A;

        advance = (timeMin - t0)/(t1 - t0);
        position = A + (v_AB * advance);
    }
    return position;
}

double Flight::getArvTime() const
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
             << ", heading : " << qRadiansToDegrees(flt.getHeading()) << "deg"
             << ", start : " << flt.startTime << ", end :" << flt.arvTime
             << ", flight Number : " << flt.flightNumberStr.toUtf8().constData()
             << ", turn : " << flt.turn[0] << ", " << flt.turn[1] << ", " << flt.turn[2];
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

bool Flight::isDefault() {
    return flightNumberStr == "DEFAULT";
}

bool Flight::isDetourned() {
    return route.length() > 2;
}

double Flight::distance(Flight f, int time) {
    return Point::getRange(this->getXt(time), f.getXt(time));
}

const QList<Point> &Flight::getRoute() const
{
    return route;
}

void Flight::setTurn(const QList<int> &newTurn)
{
    turn = newTurn;
    setRoute();
    setRange();
    setArvTime();
}

void Flight::setRoute() {
    double t0 = turn[0];
    double alpha = turn[1];
    double t1 = turn[2];
    double segLenght = (double)(speed * (t1 - t0))/ (TO_SEC * TO_SEC * TO_MILLISEC);
    double alphaRad = qDegreesToRadians(alpha);

    Point B = getXt(t0);
    Point C = B + Point::toPointFromPolar(segLenght, (heading - M_PI) - alphaRad);
    Point D = C + Point::toPointFromPolar(segLenght, (heading - M_PI) + alphaRad);
    Point E = X1;

    route.pop_back();
    route << B << C << D << E;
}

void Flight::setRange() {
    double rangeTemp = 0.0;
    for (int i = 0; i < route.length() - 1; i++) {
        rangeTemp += Point::getRange(route[i], route[i+1]);
    }
    range = rangeTemp;
}

void Flight::setArvTime() {
    arvTime = (double) TO_SEC * (range / speed);
}

void Flight::updatePreviousPositions(Point newPoint)
{
    if (previousPositions.length() == 0 || Point::getRange(previousPositions[0], newPoint) > 1.5) {
        if (previousPositions.length() == 5) {
            previousPositions.pop_back();
        }

        previousPositions.push_front(newPoint);
    }
}
