#ifndef SIMPLEFLIGHT_H
#define SIMPLEFLIGHT_H

#include <QString>
#include "point.h"

class SimpleFlight
{
public:
    SimpleFlight();
    SimpleFlight(int x0, int y0, int x1, int y1);
    void setProperties(int speed, int startingTime);
    bool isValid();
    QString toString(QString sep);

private:
    Point entrance;
    Point exit;
    int speed;
    int startingTime;
};

#endif // SIMPLEFLIGHT_H
