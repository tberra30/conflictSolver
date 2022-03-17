#include "graphicpainter.h"
#include "Constants.h"
#include <QDebug>
#include <QPainterPath>
#include <QGraphicsSimpleTextItem>

#include <iostream>
using namespace std;

GraphicPainter::GraphicPainter()
{
    this->showPath = false;
    this->showFlightInfo = true;
    this->showHpred = true;
    this->showCometes = false;
}

void GraphicPainter::setProperies(int w, int h) {
    this->maxWidth = w;
    this->maxHeight = h;
}

void GraphicPainter::setShowPath(bool newShowPath)
{
    showPath = newShowPath;
}

void GraphicPainter::setShowFlightInfo(bool newShowFlightInfo)
{
    showFlightInfo = newShowFlightInfo;
}

void GraphicPainter::setShowHpred(bool newShowHpred)
{
    showHpred = newShowHpred;
}

void GraphicPainter::setShowCometes(bool newShowCometes)
{
    showCometes = newShowCometes;
}

/*
    Define graphic object that represents aircraft
*/
QPolygon GraphicPainter::createGraphicPlane() {
    QPolygon aircraft;
    aircraft.setPoints(29,
                   -10, 0, //1
                   -10, 1, //2
                   -8, 2, //3
                   -5, 2, //4
                   3, 2, //7
                   -5, 2, //4
                   2, 7, //5
                   3, 7, //6
                   3, 2, //7
                   8, 2, //8
                   10, 0, //11
                   8, 2, //8
                   9, 4, //9
                   9, 1, //10
                   10, 0, //11
                   9, -1, //12
                   8, -2, //14
                   9, -1, //12
                   9, -4, //13
                   8, -2, //14
                   3, -2, //15
                   -5, -2, //18
                   3, -2, //15
                   3, -7, //16
                   2, -7, //17
                   -5, -2, //18
                   -8, -2, //19
                   -10, -1, //20
                   -10, 0 //1
                    );
    return QTransform().rotateRadians(M_PI).map(aircraft);
}

void GraphicPainter::updatePosition(QList<Flight> flights, QGraphicsScene *scene, int time) {
    // Clear scene
    scene->clear();

    displayedFlights = flights;

    // Show track
    if (showPath) {
        for (Flight flight : flights) drawDottedLine(scene, flight);
    }

    // Prdiction incertitude cone
    if (showHpred) {
        for (Flight flight : flights) drawPredictionHorizon(scene, flight, time, H_PRED * TO_SEC * TO_MILLISEC);
    }

    // Previous pos
    if (showCometes) {
        for (Flight flight : flights) drawPreviousPositions(scene, flight, time);
    }

    // Draw aircrafts
    for (Flight flight : flights) drawAircraft(scene, flight, time);

    // Update scene
    scene->update();
}

void GraphicPainter::drawDottedLine(QGraphicsScene *scene, Flight flight) {
    QList<Point> route = flight.getRoute();
    QPen pen;
    pen.setWidth(1);
    pen.setDashPattern({7, 7});
    for (int i = 0; i < route.length() - 1; i ++) {
        scene->addLine(scaleToSceneX(route[i].x()),
                       scaleToSceneY(route[i].y()),
                       scaleToSceneX(route[i+1].x()),
                       scaleToSceneY(route[i+1].y()),
                       pen);
    }
}

void GraphicPainter::drawPreviousPositions(QGraphicsScene *scene, Flight flight, int t) {
    QColor color = (flight.isInConflict(displayedFlights, t)) ? Qt::red : Qt::white;
    double radius = 5.0;
    for (Point point : flight.getPreviousPositions()) {
        scene->addEllipse(scaleToSceneX(point.x()) - (radius/2),
                          scaleToSceneY(point.y()) - (radius/2),
                          radius,
                          radius,
                          QPen(color, 1));
        radius--;
    }
}

void GraphicPainter::drawPredictionHorizon(QGraphicsScene* scene, Flight flight, int t, int horizon) {
    QColor color = Qt::green;
    QPen pen(color, 1);

    double hMin = (double) horizon / (TO_SEC * TO_MILLISEC);
    double range = ((double) horizon / (TO_SEC * TO_SEC * TO_MILLISEC)) * flight.getSpeed();
    double alpha = atan((double)(hMin * D_SEP) / (2 * range));

    Point Xt = flight.getXt(t);
    Point Xh1 = Xt + Point::toPointFromPolar(range, flight.getHdg(t) + alpha);
    Point Xh2 = Xt + Point::toPointFromPolar(range, flight.getHdg(t) - alpha);

    scene->addLine(scaleToSceneX(Xt.x()), scaleToSceneY(Xt.y()),
                   scaleToSceneX(Xh1.x()), scaleToSceneY(Xh1.y()),
                   pen);

    scene->addLine(scaleToSceneX(Xt.x()), scaleToSceneY(Xt.y()),
                   scaleToSceneX(Xh2.x()), scaleToSceneY(Xh2.y()),
                   pen);
}

void GraphicPainter::drawAircraft(QGraphicsScene *scene, Flight flight, int t) {
    QPolygon aircraft = createGraphicPlane();
    double Xscene = scaleToSceneX(flight.getXt(t).x());
    double Yscene = scaleToSceneY(flight.getXt(t).y());

    aircraft = QTransform().translate(Xscene, Yscene)
                            .rotateRadians(flight.getHdg(t))
                            .map(aircraft);

    QColor color = (flight.isInConflict(displayedFlights, t)) ? Qt::red : Qt::white;
    QColor colorIn = color;
    colorIn.setAlpha(150);
    scene->addPolygon(aircraft, QPen(color, 1), colorIn);

    // Show flight infos only if flight info is checked
    if (showFlightInfo) {
        QGraphicsTextItem *tooltip = new QGraphicsTextItem("AF" + flight.getFlightNumberStr() +
                                                           "\n" + QString::number(flight.getSpeed()) + "kts");
        tooltip->setPos(Xscene + 10, Yscene - 15);
        tooltip->setDefaultTextColor(color);
        scene->addItem(tooltip);
    }
}

double GraphicPainter::scaleToSceneX(double valueX) {
    return (valueX * WIDTH) / (double)maxWidth;
}

double GraphicPainter::scaleToSceneY(double valueY) {
    return (valueY * HEIGHT) / (double)maxHeight;
}
