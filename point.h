#ifndef POINT_H
#define POINT_H

#include <QtMath>
#include <QList>

using namespace std;

class Point
{
public:
    static double getRadius(Point, Point);
    static double getRange(Point, Point);
    static double scalarProd(Point, Point);
    static Point toPointFromPolar(double r, double alpha);
    friend ostream& operator<<(ostream&, const Point&);
    friend ostream& operator<<(ostream&, const QList<Point>);
    friend Point operator+(const Point&, const Point&);
    friend Point operator-(const Point&, const Point&);
    friend Point operator*(const Point&, const double);
    friend Point operator/(const Point&, const double);

    Point();
    Point(double, double);
    double getDist();
    bool isDefault();
    double x();
    double y();

private:
    double X;
    double Y;
    bool IsDefault;

};

#endif // POINT_H
