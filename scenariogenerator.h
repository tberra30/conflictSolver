#ifndef RANDOMFLIGHTS_H
#define RANDOMFLIGHTS_H

#include "simpleflight.h"
#include <QString>
#include <QList>
#include <stdlib.h>

class ScenarioGenerator
{
public:
    ScenarioGenerator();
    void randomScenario(QString path);
    QString getFilePath();
    static int randomNumber(int, int);
    static double random();

    void setProperties(int duration, int minSep, int maxSep, int minSpd, int maxSpd);
    void print();

private:
    const QString ext = ".csv";
    const QString separator = ";";

    QString fileName;
    QString path;
    int duration;
    int width;
    int height;
    int minSep;
    int maxSep;
    int minSpd;
    int maxSpd;
    int x;
    int y;

    QList<SimpleFlight> flightList;
    SimpleFlight generateValidFlight();
    int computeCoordinate(int face, char mapType);
    void writeInFile();
    void setFileName();
};

#endif // RANDOMFLIGHTS_H
