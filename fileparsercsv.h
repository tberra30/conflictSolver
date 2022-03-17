#ifndef FILEPARSERCSV_H
#define FILEPARSERCSV_H

#include "flight.h"
#include <QString>
#include <QList>

/*
    Parse a given file
    Check if the file match attemps
    Data owner & manager
*/
class FileParserCsv {
public:
    FileParserCsv();
    bool load(QString);
    void setProperies(QList<QString>);
    void setFlightList(QList<Flight>);

    int getTotalTime() const;
    int getWidth() const;
    int getHeight() const;
    const QList<Flight> &getFlightList() const;
    const QString &getPath() const;

private:
    QList<QList<QString>> flightListTemp;
    QList<Flight> flightList;
    QString path;
    bool isApproved();
    int width, height;
};

#endif // FILEPARSERCSV_H
