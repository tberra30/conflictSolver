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
    void setShowPath(bool newShowPath);
    void setShowFlightInfo(bool newShowFlightInfo);
    void setShowHpred(bool newShowHpred);
    void setShowCometes(bool newShowCometes);

private:
    QList<Flight> displayedFlights;
    int maxWidth, maxHeight;
    bool showPath;
    bool showFlightInfo;
    bool showHpred;
    bool showCometes;

    double scaleToSceneX(double);
    double scaleToSceneY(double);
    QPolygon createGraphicPlane();
    void drawDottedLine(QGraphicsScene*, Flight);
    void drawPreviousPositions(QGraphicsScene*, Flight, int);
    void drawAircraft(QGraphicsScene*, Flight, int);
    void drawPredictionHorizon(QGraphicsScene* scene, Flight flight, int t, int horizon);
};

#endif // GRAPHICPAINTER_H
