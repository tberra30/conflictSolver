#include "fileparsercsv.h"
#include <QFile>
#include<QTextStream>
#include <QDebug>

FileParserCsv::FileParserCsv()
{
}

void FileParserCsv::setProperies(QList<QString> list) {
    this->width = list[0].toInt();
    this->height = list[1].toInt();
}

bool FileParserCsv::load(QString fileName) {
    flightListTemp.clear();
    flightList.clear();

    QFile inputFile(fileName);

    if (inputFile.open(QIODevice::ReadOnly))
    {
       int lineId = 0;
       QTextStream in(&inputFile);

       while (!in.atEnd())
       {
          QString line = in.readLine();
          QList list = line.split(";");
          // Remove empty strings
          list.removeAll(QString(""));
          if (lineId == 0) {
            // Remove first elem description
            list.pop_front();
            // Set height and width properties
            this->setProperies(list);
          } else {
            flightListTemp.append(list);
          }
          lineId ++;
       }
       inputFile.close();
    }

    if(!flightListTemp.isEmpty() && this->isApproved()) {
        // Begin 1 to remove headers
        for (int i = 1; i < flightListTemp.length(); i++) {
            QList list = flightListTemp[i];
            int x0 = list[0].toInt();
            int y0 = list[1].toInt();
            int x1 = list[2].toInt();
            int y1 = list[3].toInt();
            int startTime = list[4].toInt();
            int speed = list[5].toInt();
            flightList.append(Flight(x0, y0, x1, y1, startTime, speed, i));
        }
        return true;
    }

    return false;
}

bool FileParserCsv::isApproved() {
    int refLenght = flightListTemp[0].length();

    for (QList list : flightListTemp) {
        if (list.length() != refLenght) {
            return false;
        }
    }

    return true;
}

int FileParserCsv::getHeight() const
{
    return height;
}

const QList<Flight> &FileParserCsv::getFlightList() const
{
    return flightList;
}

int FileParserCsv::getWidth() const
{
    return width;
}

int FileParserCsv::getTotalMinutes() const {
    int max = 0;

    for (Flight flight : flightList) {
        if (flight.getArvTime() > max) {
            max = flight.getArvTime();
        }
    }

    return max;
}
