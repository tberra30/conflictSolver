#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "fileparsercsv.h"
#include "Constants.h"
#include "scenariodialog.h"
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
    // Main call to UI frame
    ui->setupUi(this);
    timer = new QTimer(this);
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, WIDTH, HEIGHT);

    // Initialize variables
    ResetSimulation();

    // Define actions triggered by buttons (link buttons and actions)
    setUpActions();

    // Start timer to emit signal every TIME_BEAN millisec
    timer->start(TIME_BEAN);

    // Displaying (update view)
    ui->graphicsView->setScene(scene);
}

void MainWindow::setUpActions() {
    // Menu
    connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(Open()));
    connect(ui->actionReset, SIGNAL(triggered()), this, SLOT(ResetSimulation()));
    connect(ui->actionConfigure, SIGNAL(triggered()), this, SLOT(turn()));
    connect(ui->actionGenerate, SIGNAL(triggered()), this, SLOT(GenerateScenario()));
    connect(ui->actionSaveScreen, SIGNAL(triggered()), this, SLOT(ResetSimulation()));
    connect(ui->actionGenerate_Open, SIGNAL(triggered()), this, SLOT(GenerateAndOpen()));

    // Push buttons
    connect(ui->pushButtonStart, SIGNAL(clicked()), this, SLOT(StartSimulation()));
    connect(ui->pushButtonRestart, SIGNAL(clicked()), this, SLOT(RestartSimulation()));
    connect(ui->pushButtonStop, SIGNAL(clicked()), this, SLOT(StopSimulation()));
    connect(ui->pushButtonReset, SIGNAL(clicked()), this, SLOT(ResetSimulation()));

    // Sliders
    connect(ui->verticalSliderSpeed, SIGNAL(sliderMoved(int)), this, SLOT(SpeedChange()));

    // Timer
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
}

void MainWindow::turn() {
    QList<Flight> temp = displayedFlightList;
    displayedFlightList.clear();
    for (Flight fl : temp) {
        QList<int> turn;
        turn.append(timeCounter + 300000);
        turn.append(45);
        turn.append(timeCounter + 600000);
        fl.setTurn(turn);
        displayedFlightList.append(fl);
    }
}

void MainWindow::StartSimulation() {
    isRunning = true;
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

    // Data reset
    displayedFlightList = fileParser.getFlightList();

    // Parameters reset
    timeCounter = 0;
    speedFactor = DEFAULT_SPEED_VALUE;
    StopSimulation();
}

void MainWindow::StopSimulation() {
    isRunning = false;
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

void MainWindow::open(QString fileName) {
    if (!fileName.isEmpty()) {
        if(fileParser.load(fileName)) {
            ResetSimulation();
            setDisableControls(false);
            totalTime = fileParser.getTotalMinutes();
            graphicPainter.setProperies(fileParser.getWidth(), fileParser.getHeight());
            ui->labelCurrentFile->setText(fileName);
            QMessageBox::information(this, "Success", "File loaded successfully");
        } else {
            setDisableControls(true);
            QMessageBox::information(this, "Error", "File does not match the correct format");
        }
    }
}

void MainWindow::Open() {
   open(browseDirectory());
}

QString MainWindow::GenerateScenario() {
    QString returnedPath = "";

    /* Init params */
    QString path = QDir::home().path();
    int scenarioTime = TOTAL_TIME_SCENARIO;
    int minSep = MIN_SEP;
    int maxSep = MAX_SEP;

    /* Display dialog window */
    QString dirPath = QFileDialog::getExistingDirectory(this, "Choose output directory", path);
    if (!dirPath.isEmpty()) {
        path = dirPath;

        /* Compute scenario */
        scenarioGenerator.randomScenario(path,
                                         scenarioTime,
                                         minSep,
                                         maxSep,
                                         WINDOW_WIDTH,
                                         WINDOW_HEIGHT);

        returnedPath = scenarioGenerator.getFilePath();

        QMessageBox::information(this, "Success", "Scenario has been generate successfully in " + scenarioGenerator.getFilePath());
    }

    return returnedPath;
}

void MainWindow::GenerateAndOpen() {
    open(GenerateScenario());
}

void MainWindow::SpeedChange() {
    speedFactor = ui->verticalSliderSpeed->value();
}

void MainWindow::updateTime() {
    if (isRunning) {
        timeCounter += TIME_BEAN * speedFactor;
    }

    // Update widgets
    double timeMinute = timeCounter/ (double)(TO_MILLISEC * TO_SEC);
    double timeSecond = (timeCounter / TO_MILLISEC) % TO_SEC;

    ui->lcdNumberTimeMin->display(qFloor(timeMinute));
    ui->lcdNumberTimeSec->display(qFloor(timeSecond));
    ui->progressBar->setValue(qFloor(TO_PERCENTAGE * timeMinute / totalTime));
    ui->verticalSliderSpeed->setValue(speedFactor);
    ui->lcdNumberSpeed->display(speedFactor);

    algoGen.printConflicts(displayedFlightList, timeCounter);
}

void MainWindow::setPreviousPositions() {
    QList<Flight> temp = displayedFlightList;
    displayedFlightList.clear();
    for (Flight flight : temp) {
        flight.updatePreviousPositions(flight.getXt(timeCounter));
        displayedFlightList.append(flight);
    }
}

void MainWindow::forceUpdateScene() {
   // Update previous positions if timelaps is anough
   setPreviousPositions();

   // Update scene
   graphicPainter.updatePosition(displayedFlightList, scene, timeCounter);
}

void MainWindow::updateScene() {
    if (isRunning) {
        if (timeCounter >= totalTime * TO_SEC * TO_MILLISEC) {
            ResetSimulation();
        } else {
            forceUpdateScene();
        }
    }
}

void MainWindow::update() {
    updateTime();
    updateScene();
}

MainWindow::~MainWindow()
{
    delete ui;
}

