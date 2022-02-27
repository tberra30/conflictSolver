#ifndef GRAPHICPAINTER_H
#define GRAPHICPAINTER_H

#include "flight.h"
#include <QPolygon>
#include <QGraphicsScene>
#include <QList>

/*
    Manage all graphics issues
    Draw aircrafts, lines, ...
*/
class GraphicPainter
{
public:
    GraphicPainter();
    void updatePosition(QList<Flight>, QGraphicsScene*, int);
    void setProperies(int, int);

private:
    QList<Flight> displayedFlights;
    int maxWidth, maxHeight;

    double scaleToSceneX(double);
    double scaleToSceneY(double);
    QPolygon createGraphicPlane();
    void drawDottedLine(QGraphicsScene*, Flight);
    void drawPreviousPositions(QGraphicsScene*, Flight, int);
    void drawAircraft(QGraphicsScene*, Flight, int);
};

#endif // GRAPHICPAINTER_H
