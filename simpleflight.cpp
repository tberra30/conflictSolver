#include "simpleflight.h"

SimpleFlight::SimpleFlight()
{
    this->entrance = Point(0, 0);
    this->exit = Point(0, 0);
}

SimpleFlight::SimpleFlight(int x0, int y0, int x1, int y1)
{
    this->entrance = Point(x0, y0);
    this->exit = Point(x1, y1);
}

/*
    Return a displayable string object, it is a simpleFlight representation
    ex  : 0 100 100 0 20 150 (with sep = " ")
     => Means the aircraft come from the point (0, 100) Nm to the end point (100, 0)
        20 min after the simulation beginning, and with a speed of 150 kts
*/
QString SimpleFlight::toString(QString sep)
{
    QString X0 = QString::number(entrance.x());
    QString Y0 = QString::number(entrance.y());
    QString X1 = QString::number(exit.x());
    QString Y1 = QString::number(exit.y());
    QString SPEED = QString::number(speed);
    QString START = QString::number(startingTime);

    return X0 + sep + Y0 + sep + X1 + sep + Y1 + sep + SPEED + sep + START;
}

/*
    Call this method to set up speed and starting time props
*/
void SimpleFlight::setProperties(int speed, int startingTime)
{
    this->speed = speed;
    this->startingTime = startingTime;
}

/*
    Retrun true if route is longer than 70 km
*/
bool SimpleFlight::isValid()
{
    bool enoughtRange = Point::getRange(entrance, exit) > 70;
    return enoughtRange;
}
