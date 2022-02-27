#ifndef SCENARIODIALOG_H
#define SCENARIODIALOG_H

#include <QDialog>
#include <QWidget>

    namespace Ui2 { class ScenarioDialog; }

class ScenarioDialog : public QDialog
{
    Q_OBJECT

public:
    ScenarioDialog(QWidget *);

    int duration;
    int maxSeparation;
    int minSeparation;
    QString outputPath;

private:
    Ui2::ScenarioDialog *ui;

signals:
    void submit();
};

#endif // SCENARIODIALOG_H
