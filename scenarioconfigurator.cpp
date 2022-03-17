#include "scenarioconfigurator.h"
#include "ui_scenarioconfigurator.h"

ScenarioConfigurator::ScenarioConfigurator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScenarioConfigurator)
{
    ui->setupUi(this);
    connect(ui->submitButton, SIGNAL(clicked()), this, SLOT(submitForm()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(cancel()));
}

void ScenarioConfigurator::submitForm() {
    int density = (11 - ui->density->value());
    duration = ui->duration->value();
    minSpeed = ui->minSpeed->value();
    maxSpeed = ui->maxSpeed->value();
    minTimelaps = density;
    maxTimelaps = density + 2;
    this->close();
    emit scenarioChange();
}

void ScenarioConfigurator::cancel() {
    this->close();
}

ScenarioConfigurator::~ScenarioConfigurator()
{
    delete ui;
}

int ScenarioConfigurator::getMinSpeed() const
{
    return minSpeed;
}

int ScenarioConfigurator::getMaxSpeed() const
{
    return maxSpeed;
}

int ScenarioConfigurator::getMinTimelaps() const
{
    return minTimelaps;
}

int ScenarioConfigurator::getMaxTimelaps() const
{
    return maxTimelaps;
}

int ScenarioConfigurator::getDuration() const
{
    return duration;
}
