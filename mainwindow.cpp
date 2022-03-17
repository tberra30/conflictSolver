#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileparsercsv.h"
#include "Constants.h"
#include <QDebug>
#include <QTimer>
#include <QAction>
#include <QFileDialog>
#include <QInputDialog>

#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    // Call .ui files
    ui->setupUi(this);

    // Define timers & scene
    uiTimer = new QTimer(this);
    sceneTimer = new QTimer(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, WIDTH, HEIGHT);

    // Initialize variables
    ResetSimulation();

    // Start ui timer
    uiTimer->start();

    // Define actions triggered by buttons (link buttons and actions)
    setUpActions();

    // Displaying (update view)
    ui->graphicsView->setScene(scene);
}

void MainWindow::setUpActions() {
    // Menu
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(ResetSimulation()));
    connect(ui->actionGenerate, SIGNAL(triggered()), this, SLOT(GenerateScenario()));
    connect(ui->actionSaveScreen, SIGNAL(triggered()), this, SLOT(ResetSimulation()));
    connect(ui->actionGenerate_Open, SIGNAL(triggered()), this, SLOT(GenerateAndOpen()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(ShowScenarioConfig()));
    connect(ui->checkBoxShowPath, SIGNAL(stateChanged(int)), this, SLOT(ShowPathWays()));
    connect(ui->checkBoxFlightInfo, SIGNAL(stateChanged(int)), this, SLOT(ShowFlightInfos()));
    connect(ui->checkBoxHorizon, SIGNAL(stateChanged(int)), this, SLOT(ShowHorizon()));
    connect(ui->checkBoxCometes, SIGNAL(stateChanged(int)), this, SLOT(ShowCometes()));
    connect(ui->ActiveSolver, SIGNAL(stateChanged(int)), this, SLOT(setAlgoGenActiveStatus()));

    // Push buttons
    connect(ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(StartSimulation()));
    connect(ui->pushButtonRestart, SIGNAL(clicked()), this, SLOT(RestartSimulation()));
    connect(ui->pushButtonStop, SIGNAL(clicked()), this, SLOT(StopSimulation()));
    connect(ui->pushButtonReset, SIGNAL(clicked()), this, SLOT(ResetSimulation()));
    connect(ui->reloadScenario, SIGNAL(clicked()), this, SLOT(ReloadScenario()));
    connect(&scenarioConfigurator, SIGNAL(scenarioChange()), this, SLOT(setScenarioProperties()));

    // Sliders
    connect(ui->verticalSliderSpeed, SIGNAL(sliderMoved(int)), this, SLOT(SpeedChange()));

    // Timer
    connect(uiTimer, SIGNAL(timeout()), this, SLOT(updateWidgets()));
    connect(sceneTimer, SIGNAL(timeout()), this, SLOT(updateAll()));
    connect(sceneTimer, SIGNAL(timeout()), this, SLOT(launchAlgoGen()));
}

/*
 * Modify aircrafts turn attibute if a turn is ordered
*/
void MainWindow::turn(QList<Individual> indList) {
    for (int k = 0; k < displayedFlightList.length(); k ++) {
        for (int j = 0; j < indList.length(); j ++) {
            if (indList[j].getId() == displayedFlightList[k].getFlightNumberStr()) {
                displayedFlightList[k].setTurn(QList{
                                 timeCounter + indList[j].getTurnBeginTime(),
                                 indList[j].getTurnRadius(),
                                 timeCounter + indList[j].getTurnEndTime()});
            }
        }
    }
}

void MainWindow::StartSimulation() {
    sceneTimer->start();
    ui->verticalSliderSpeed->setDisabled(false);
}

void MainWindow::RestartSimulation() {
    ResetSimulation();
    StartSimulation();
}

void MainWindow::ResetSimulation() {
    // Graphic reset
    scene->clear();
    scene->update();
    algoGen.reset();

    // Data reset
    displayedFlightList.clear();

    // Parameters reset
    timeCounter = 0;
    speedFactor = DEFAULT_SPEED_VALUE;

    // Set timers to emit signal every x millisec
    uiTimer->setInterval(UPDATE_TIME_APP);
    sceneTimer->setInterval(UPDATE_TIME_APP);

    ui->verticalSliderSpeed->setValue(speedFactor);

    StopSimulation();
}

void MainWindow::StopSimulation() {
    sceneTimer->stop();
}

void MainWindow::ReloadScenario() {
    StopSimulation();
    fileParser.load(fileParser.getPath());
    ResetSimulation();
}

void MainWindow::setDisableControls(bool b) {
    ui->pushButtonRestart->setDisabled(b);
    ui->pushButtonStart->setDisabled(b);
    ui->pushButtonStop->setDisabled(b);
    ui->pushButtonReset->setDisabled(b);
    ui->verticalSliderSpeed->setDisabled(b);
}

QString MainWindow::browseDirectory() {
    // Stop simu when file choosing
    StopSimulation();

    QString path = QDir::home().path();
    QString fileName = QFileDialog::getOpenFileName(this,
             tr("Open scenario file"), path, tr("Files (*.csv)"));

    return fileName;
}

void MainWindow::open(QString fileName, bool showWindow) {
    StopSimulation();
    if (!fileName.isEmpty()) {
        if(fileParser.load(fileName)) {
            ResetSimulation();
            setDisableControls(false);
            totalTime = fileParser.getTotalTime();
            graphicPainter.setProperies(fileParser.getWidth(), fileParser.getHeight());
            ui->labelCurrentFile->setText(fileName);
            updateWidgets();
            if (showWindow) QMessageBox::information(this, "Success", "File loaded successfully");
        } else {
            setDisableControls(true);
            if (showWindow) QMessageBox::information(this, "Error", "File does not match the correct format");
        }
    }
}

void MainWindow::Open() {
   open(browseDirectory(), true);
}

QString MainWindow::GenerateScenario() {
    StopSimulation();

    QString returnedPath = "";

    /* Init params */
    QString path = QDir::home().path();

    /* Display dialog window */
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose output directory", path);
    if (!dirPath.isEmpty()) {
        path = dirPath;

        /* Compute scenario */
        scenarioGenerator.randomScenario(path);

        returnedPath = scenarioGenerator.getFilePath();

        QMessageBox::information(this, "Success", "Scenario has been generate successfully in " + returnedPath);
    }

    return returnedPath;
}

void MainWindow::launchAlgoGen() {
    if (timeCounter % UPDATE_TIME_ALGO_GEN < UPDATE_TIME_APP * speedFactor) {
        // Apply turn solutions to aircrafts
        ui->label_conflict->setPalette(QPalette(QColor("orange"), QColor("orange"), QColor("orange"), QColor("orange"), QColor("orange"), QColor("orange"), QColor("orange")));
        ui->label_conflict->setText("Algo gen processing ...");
        turn(algoGen.run(displayedFlightList, timeCounter));
        QString conflict = (algoGen.getConflictDetected()) ? "WARNING : conflict(s) detected" : "No conflict detected";
        QColor textColor = (algoGen.getConflictDetected()) ? Qt::red : Qt::green;
        ui->label_conflict->setPalette(QPalette(textColor, textColor, textColor, textColor, textColor, textColor, textColor));
        ui->label_conflict->setText(conflict);
    }
}

void MainWindow::setScenarioProperties() {
    scenarioGenerator.setProperties(scenarioConfigurator.getDuration(),
                                    scenarioConfigurator.getMinTimelaps(),
                                    scenarioConfigurator.getMaxTimelaps(),
                                    scenarioConfigurator.getMaxSpeed(),
                                    scenarioConfigurator.getMinSpeed());
}

void MainWindow::GenerateAndOpen() {
    open(GenerateScenario(), false);
}

void MainWindow::SpeedChange() {
    speedFactor = ui->verticalSliderSpeed->value();
    updateWidgets();
}

/*
 * Increment time counter
 */
void MainWindow::updateTime() {
    timeCounter += UPDATE_TIME_APP * speedFactor;
}

/*
 * Update widgets in IHM
 */
void MainWindow::updateWidgets() {
    double timeMinute = (double) timeCounter/ (TO_MILLISEC * TO_SEC);
    double timeSecond = (timeCounter / TO_MILLISEC) % TO_SEC;

    ui->lcdNumberTimeMin->display(qFloor(timeMinute));
    ui->lcdNumberTimeSec->display(qFloor(timeSecond));
    ui->progressBar->setValue(qFloor(TO_PERCENTAGE * timeCounter / totalTime));
    ui->lcdNumberSpeed->display(speedFactor);
}

void MainWindow::ShowPathWays() {
    graphicPainter.setShowPath(ui->checkBoxShowPath->checkState());
    updateScene();
}

void MainWindow::ShowFlightInfos() {
    graphicPainter.setShowFlightInfo(ui->checkBoxFlightInfo->checkState());
    updateScene();
}

void MainWindow::ShowHorizon() {
    graphicPainter.setShowHpred(ui->checkBoxHorizon->checkState());
    updateScene();
}

void MainWindow::ShowCometes() {
    graphicPainter.setShowCometes(ui->checkBoxCometes->checkState());
    updateScene();
}

void MainWindow::setAlgoGenActiveStatus() {
    bool state = ui->ActiveSolver->checkState();
    QString iconRessourcePath = (state) ? ":/img/correct.png" : ":/img/remove.png";
    QString buttonText = (state) ? "Conflict solver activated" : "Conflict solver disabled";
    ui->ActiveSolver->setText(buttonText);
    ui->ActiveSolver->setIcon(QIcon(iconRessourcePath));
    algoGen.setActive(state);
    updateScene();
}

void MainWindow::ShowScenarioConfig() {
    scenarioConfigurator.show();
}

void MainWindow::setPreviousPositions() {
    for (int i = 0; i < displayedFlightList.length(); i ++) {
        displayedFlightList[i].updatePreviousPositions(timeCounter);
    }
}

void MainWindow::forceUpdateScene() {
    QList<QString> idList;
    for (Flight fl : displayedFlightList) {
        idList.append(fl.getFlightNumberStr());
    }

    // Update displayed flights list
    for (Flight flight : fileParser.getFlightList()) {
        if (idList.contains(flight.getFlightNumberStr())) {
            if (timeCounter > flight.getArvTime()) {
                displayedFlightList.removeOne(flight);
            }
        } else {
            if (timeCounter >= flight.getStartTime() && timeCounter <= flight.getArvTime()) {
                displayedFlightList.append(flight);
            }
        }
    }

   // Update previous positions if timelaps is anough
   setPreviousPositions();

   // Update scene
   graphicPainter.updatePosition(displayedFlightList, scene, timeCounter);
}

void MainWindow::updateScene() {
    // End of simulation condition
    if (timeCounter >= totalTime) {
        ResetSimulation();
    } else {
        forceUpdateScene();
    }
}

void MainWindow::updateAll() {
    updateTime();
    updateScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

