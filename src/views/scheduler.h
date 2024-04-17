#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QWidget>
#include <QCalendarWidget>
#include <QPushButton>
#include <QVBoxLayout>

class Scheduler : public QWidget {
    Q_OBJECT

public:
    explicit Scheduler(QWidget *parent = nullptr);

private:
    QCalendarWidget *calendar;
    QPushButton *addShiftButton;
    void setupUI();
};

#endif // SCHEDULER_H
