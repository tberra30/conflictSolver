#ifndef POINT_H
#define POINT_H

#include <QtMath>

using namespace std;

class Point
{
public:
    static double getRadius(Point, Point);
    static double getRange(Point, Point);
    static double scalarProd(Point, Point);
    static Point toPointFromPolar(double, double);
    friend ostream& operator<<(ostream&, const Point&);
    friend Point operator+(const Point&, const Point&);
    friend Point operator-(const Point&, const Point&);
    friend Point operator*(const Point&, const double);

    Point();
    Point(double, double);
    double getDist();
    double x();
    double y();

private:
    double X;
    double Y;

};

#endif // POINT_H
