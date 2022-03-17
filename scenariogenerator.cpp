#include "scenariogenerator.h"
#include "Constants.h"
#include <fstream>
#include <map>
#include <ctime>
#include <iostream>

using namespace std;

// Util maps & function
static QList<QString> headers = {
    "x0",
    "y0",
    "x1",
    "y1",
    "startTime",
    "speed"};

static map<int, int> xMap = {
    {0, 1},
    {1, 100},
    {2, 1},
    {3, 0}};

static map<int, int> yMap = {
    {0, 0},
    {1, 1},
    {2, 100},
    {3, 1}};

/*
    This function returns an int in [min, max]
*/
int ScenarioGenerator::randomNumber(int min, int max)
{
    return (rand() % (max - min + 1)) + min;
}

/*
    This function returns a double in [0, 1]
*/
double ScenarioGenerator::random() {
    return ((double) rand() / (RAND_MAX));
}

/*
    This function returns a string with the current datetime
    ex : "11_21T22_55"
*/
static QString getTime()
{
    time_t rawtime;
    struct tm *timeinfo;
    char buffer[12];

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, 12, "%d_%mT%H_%M", timeinfo);

    return QString::fromStdString(buffer);
}

// Object methods
/*
    1) The rectangle model is described as following : (each side is represented by a number)

                   0
        ------------------------
        |                      |
        |                      |
     3  |                      |  1
        |                      |
        |                      |
        -----------------------
                   2

    2) The returned int is between 0 and maxValue of x or y

    3) Parameters :
            face : int in {0, 1, 2, 3}
            mapType : char 'x' or 'y'
*/
int ScenarioGenerator::computeCoordinate(int face, char mapType)
{
    map<int, int> map = (mapType == 'x') ? xMap : yMap;
    int maxNumber = (mapType == 'x') ? width : height;

    if (map[face] == 0)
    {
        return 0;
    }
    else if (map[face] == 100)
    {
        return maxNumber;
    }
    else
    {
        return randomNumber(0, maxNumber);
    }
}

ScenarioGenerator::ScenarioGenerator() {
    this->height = WINDOW_HEIGHT;
    this->width = WINDOW_WIDTH;
    this->duration = TOTAL_TIME_SCENARIO;
    this->maxSep = MAX_SEP;
    this->minSep = MIN_SEP;
    this->minSpd = MIN_SPD;
    this->maxSpd = MAX_SPD;
}

/*
    Parameters :
        path = C:/dir1/dir2/dir3 (string) || Output directory for csv saving
        duration of simulation (minutes)
        minSep = minimum timelaps between two aircrafts (minutes) || Density parameters
        maxSep = maximum timelaps between two aircrafts (minutes) ||
        minSpd = minimum speed of aircrafts (kts) || Speed parameters
        maxSpd = maximum speed of aircrafts (kts) ||
*/
void ScenarioGenerator::setProperties(int duration, int minSep, int maxSep, int minSpd, int maxSpd) {
    this->duration = duration;
    this->minSep = minSep;
    this->maxSep = maxSep;
    this->minSpd = minSpd;
    this->maxSpd = maxSpd;
}

void ScenarioGenerator::print() {
    cout << "ScenarioGenerator(" << duration << ", " << minSep << ", "
         << maxSep << ", " << minSpd << ", " << maxSpd << ", " << width
         << ", " << height << ")" << endl;
}

void ScenarioGenerator::randomScenario(QString path)
{
    srand(time(NULL));

    // add Flights to list
    int time = 0;
    int deltaT, speed;
    SimpleFlight flightTemp;
    this->path = path;

    while (time < duration)
    {
        deltaT = randomNumber(minSep, maxSep); // Density
        speed = randomNumber(minSpd, maxSpd);  // Kts

        time += deltaT;
        flightTemp = generateValidFlight();
        flightTemp.setProperties(time, speed);
        flightList.append(flightTemp);
    }

    // Write tab in the given path attribute
    writeInFile();
}

/*
    This function returns a random SimpleFlight object
    Which is validate with isValid() method of Flight class
*/
SimpleFlight ScenarioGenerator::generateValidFlight()
{
    SimpleFlight flight;
    while (!flight.isValid())
    {
        int enteringSide = randomNumber(0, 3);
        int leavingSide = (enteringSide + randomNumber(1, 3)) % 4;

        int x0 = computeCoordinate(enteringSide, 'x');
        int x1 = computeCoordinate(leavingSide, 'x');
        int y0 = computeCoordinate(enteringSide, 'y');
        int y1 = computeCoordinate(leavingSide, 'y');

        flight = SimpleFlight(x0, y0, x1, y1);
    }

    return flight;
}

void ScenarioGenerator::setFileName() {
    QString timestamp = getTime();
    QString name{"randomScenario"};
    QString slash{"/"};
    fileName = path + slash + name + timestamp + ext;
}

QString ScenarioGenerator::getFilePath() {
    return fileName;
}

/*
    This function returns nothing
    It write data of flightList in a file at the given path attribute
*/
void ScenarioGenerator::writeInFile()
{
    // Build complete file path
    setFileName();

    // Create & open file
    ofstream myfile;
    myfile.open(fileName.toStdString());

    // Write macro parameters
    myfile << "Parameters (width height)" << separator.toStdString()
           << width << separator.toStdString()
           << height << separator.toStdString()
           << endl;

    // Write headers
    for (QString head : headers)
    {
        myfile << head.toStdString() << separator.toStdString();
    }
    myfile << endl;

    // Write flights data
    for (SimpleFlight flight : flightList)
    {
        myfile << flight.toString(separator).toStdString() << endl;
    }

    // Close file
    myfile.close();
}
