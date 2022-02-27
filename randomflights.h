#ifndef RANDOMFLIGHTS_H
#define RANDOMFLIGHTS_H

#include "simpleflight.h"
#include <QString>
#include <QList>
#include <stdlib.h>

class RandomFlights
{
public:
    void randomScenario(QString, int, int, int, int, int);
    QString getFilePath();

private:
    const QString ext = ".csv";
    const QString separator = ";";

    QString fileName;
    QString path;
    int duration;
    int width;
    int height;
    int x;
    int y;

    QList<SimpleFlight> flightList;
    SimpleFlight generateValidFlight();
    int computeCoordinate(int face, char mapType);
    void writeInFile();
    void setFileName();
};

#endif // RANDOMFLIGHTS_H
