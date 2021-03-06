#ifndef ARSCH_H
#define ARSCH_H

#include <QWidget>
#include <QVector>
#include <QList>
#include <QSettings>

#include <weekdaybutton.h>

namespace Ui {
class AlarmSettings;
}

class AlarmSettings : public QWidget
{
    Q_OBJECT

public:
    explicit AlarmSettings(QWidget *parent = 0);

    int hour;
    int minute;

    int hourStep;
    int minuteStep;

    QSettings *settings;

    QList<WeekDayButton*> *dayButtons;

    bool isButtonActive(int day);

    ~AlarmSettings();

signals:
    void GoBack();

private slots:
    void on_pushButton_clicked();

    void on_hourDown_clicked();
    void on_hourUp_clicked();

    void on_minuteDown_clicked();
    void on_minuteUp_clicked();

    void day_clicked(bool);

    void Blink();
private:
    void UpdateTime();
    QString FormatTime(int hour, int minute);

    Ui::AlarmSettings *ui;

};

#endif // ARSCH_H
