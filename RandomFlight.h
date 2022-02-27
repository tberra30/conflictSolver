#ifndef RANDOMFLIGHT_H
#define RANDOMFLIGHT_H

#include <string>
#include <vector>
#include <stdlib.h>
#include "simpleFlight.h"

class RandomFlights
{
public:
    void randomScenario(string, int, int, int, int, int);

private:
    const string ext = ".csv";
    const string separator = ";";

    string path;
    int duration;
    int width;
    int height;
    int x;
    int y;

    vector<SimpleFlight> flightList;
    SimpleFlight generateValidFlight();
    int computeCoordinate(int face, char mapType);
    void writeInFile();
};

#endif // RANDOMFLIGHT_H