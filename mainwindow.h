#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flight.h"
#include "graphicpainter.h"
#include "fileparsercsv.h"
#include "randomflights.h"
#include "geneticalgorithm.h"
#include <QMainWindow>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
    namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void setPreviousPositions();
    ~MainWindow();

private:
    int timeCounter; // Time counter in millisecond (increment while isRunning is 1)
    int totalTime; // Total time of scenario (might be initialize at loading)
    int speedFactor; // mini 1 : 1 real sec = 1sec, maxi 60 : 1 real sec = 1 min
    bool isRunning; // false if pause, true if running
    QTimer *timer;
    Ui::MainWindow *ui;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    GraphicPainter graphicPainter;
    FileParserCsv fileParser;
    RandomFlights scenarioGenerator;
    GeneticAlgorithm algoGen = GeneticAlgorithm(1000, 50, 10);
    QList<Flight> displayedFlightList;
    QString browseDirectory();
    void open(QString);

private slots:
    void setUpActions();
    void StartSimulation();
    void RestartSimulation();
    void StopSimulation();
    void ResetSimulation();
    void SpeedChange();
    void update();
    void updateTime();
    void updateScene();
    void forceUpdateScene();
    void setDisableControls(bool);
    QString GenerateScenario();
    void Open();
    void GenerateAndOpen();
    void turn();

};
#endif // MAINWINDOW_H
