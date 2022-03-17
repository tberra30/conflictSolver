#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "flight.h"
#include "graphicpainter.h"
#include "fileparsercsv.h"
#include "scenariogenerator.h"
#include "geneticalgorithm.h"
#include "scenarioconfigurator.h"
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
    QTimer *uiTimer, *sceneTimer;
    Ui::MainWindow *ui;
    QGraphicsView *graphicsView;
    QGraphicsScene *scene;
    GraphicPainter graphicPainter;
    ScenarioConfigurator scenarioConfigurator;
    FileParserCsv fileParser;
    ScenarioGenerator scenarioGenerator;
    GeneticAlgorithm algoGen = GeneticAlgorithm(100, 100, 0.4, 0.4);
    QList<Flight> displayedFlightList;
    QString browseDirectory();
    void open(QString, bool);
    void turn(QList<Individual> indList);


private slots:
    void setUpActions();
    void StartSimulation();
    void RestartSimulation();
    void StopSimulation();
    void ResetSimulation();
    void SpeedChange();
    void ReloadScenario();
    void updateTime();
    void updateScene();
    void updateWidgets();
    void updateAll();
    void launchAlgoGen();
    void forceUpdateScene();
    void setDisableControls(bool);
    QString GenerateScenario();
    void Open();
    void GenerateAndOpen();
    void ShowPathWays();
    void ShowFlightInfos();
    void ShowScenarioConfig();
    void ShowHorizon();
    void ShowCometes();
    void setAlgoGenActiveStatus();
    void setScenarioProperties();
};
#endif // MAINWINDOW_H
