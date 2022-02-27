#include "point.h"
#include <iostream>

Point::Point()
{
    X = 0.0;
    Y = 0.0;
}

Point::Point(double x, double y)
{
    X = x;
    Y = y;
}

double Point::getDist() {
    return sqrt(pow(X, 2)+pow(Y,2));
}

double Point::getRadius(Point A, Point B) {
    Point C = B - A;
    double additionalRadius = (C.X < 0) ? 0 : M_PI;
    return atan((double)(C.Y) / C.X) + additionalRadius;
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

Point operator+(const Point& pt1, const Point& pt2) {
    return Point(pt1.X + pt2.X, pt1.Y + pt2.Y);
}

Point operator-(const Point& pt1, const Point& pt2) {
    return Point(pt1.X - pt2.X, pt1.Y - pt2.Y);
}

Point operator*(const Point& pt, const double lambda) {
    return Point(pt.X * lambda, pt.Y * lambda);
}
