#include "point.h"
#include <iostream>
#include <QList>

Point::Point()
{
    IsDefault = true;
    X = 0.0;
    Y = 0.0;
}

Point::Point(double x, double y)
{
    IsDefault = false;
    X = x;
    Y = y;
}

bool Point::isDefault() {
    return IsDefault;
}

double Point::getDist() {
    return sqrt(pow(X, 2)+pow(Y,2));
}

double Point::getRadius(Point A, Point B) {
    Point C = B - A;
    double add = (C.X < 0) ? M_PI : 0;
    return atan((double)(C.Y) / C.X) + add;
}

double Point::getRange(Point A, Point B) {
    return (B - A).getDist();
}

double Point::scalarProd(Point A, Point B) {
    return (A.X * B.X) + (A.Y * B.Y);
}

Point Point::toPointFromPolar(double r, double alpha) {
    return Point(r*qCos(alpha), r*qSin(alpha));
}

double Point::x() {
    return X;
}

double Point::y() {
    return Y;
}

ostream& operator<<(ostream& os, const Point& pt)
{
    os << "(" << pt.X << ", " << pt.Y << ")";
    return os;
}

ostream& operator<<(ostream& os, const QList<Point> pts) {
    for (Point pt : pts) {
        os << " " << pt;
    }
    return os;
}

Point operator+(const Point& pt1, const Point& pt2) {
    return Point(pt1.X + pt2.X, pt1.Y + pt2.Y);
}

Point operator-(const Point& pt1, const Point& pt2) {
    return Point(pt1.X - pt2.X, pt1.Y - pt2.Y);
}

Point operator*(const Point& pt, const double lambda) {
    return Point(pt.X * lambda, pt.Y * lambda);
}

Point operator/(const Point& pt, const double lambda) {
    return Point(pt.X / lambda, pt.Y / lambda);
}
