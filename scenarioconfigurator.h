#ifndef SCENARIOCONFIGURATOR_H
#define SCENARIOCONFIGURATOR_H

#include "scenariogenerator.h"
#include <QWidget>

namespace Ui {
class ScenarioConfigurator;
}

class ScenarioConfigurator : public QWidget
{
    Q_OBJECT

public:
    explicit ScenarioConfigurator(QWidget *parent = nullptr);
    ~ScenarioConfigurator();

    int getMinSpeed() const;
    int getMaxSpeed() const;
    int getMinTimelaps() const;
    int getMaxTimelaps() const;
    int getDuration() const;

private:
    Ui::ScenarioConfigurator *ui;
    int duration;
    int minSpeed;
    int maxSpeed;
    int minTimelaps;
    int maxTimelaps;

private slots:
    void submitForm();
    void cancel();


signals:
    void scenarioChange();
};

#endif // SCENARIOCONFIGURATOR_H
