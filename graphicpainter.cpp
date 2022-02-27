#include "graphicpainter.h"
#include "Constants.h"
#include <QDebug>
#include <QPainterPath>

#include <iostream>
using namespace std;

GraphicPainter::GraphicPainter()
{
}

void GraphicPainter::setProperies(int w, int h) {
    this->maxWidth = w;
    this->maxHeight = h;
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
    return aircraft;
}

void GraphicPainter::updatePosition(QList<Flight> flights, QGraphicsScene *scene, int time) {
    // Clear scene
    scene->clear();

    // Update displayed flights list
    displayedFlights.clear();
    for (Flight flight : flights) {
        if (time >= flight.getStartTime() * TO_MILLISEC * TO_SEC && time <= flight.getArvTime() * TO_MILLISEC * TO_SEC) {
            displayedFlights.append(flight);
        }
    }

    // Update graphic objects list & append to scene
    for (Flight flight : displayedFlights) {

        // Around aircraft
        drawDottedLine(scene, flight);

        // Previous pos
        drawPreviousPositions(scene, flight, time);

        // Aircraft
        drawAircraft(scene, flight, time);
    }

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

void GraphicPainter::drawAircraft(QGraphicsScene *scene, Flight flight, int t) {
    QPolygon aircraft = createGraphicPlane();
    double Xscene = scaleToSceneX(flight.getXt(t).x());
    double Yscene = scaleToSceneY(flight.getXt(t).y());
    aircraft = QTransform().translate(Xscene, Yscene)
                            .rotateRadians(flight.getHdg(t)).map(aircraft);
    QColor color = (flight.isInConflict(displayedFlights, t)) ? Qt::red : Qt::white;
    scene->addPolygon(aircraft, QPen(color, 1));
}

double GraphicPainter::scaleToSceneX(double valueX) {
    return (valueX * WIDTH) / (double)maxWidth;
}

double GraphicPainter::scaleToSceneY(double valueY) {
    return (valueY * HEIGHT) / (double)maxHeight;
}
