#include "mainwindow.h"
#include "geneticalgorithm.h"

#include <QApplication>

#include <iostream>

/*
 * Launch app
 * git repo : https://github.com/tberra30/conflictSolver
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
