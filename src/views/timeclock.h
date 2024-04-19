#ifndef TIMECLOCK_H
#define TIMECLOCK_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QVBoxLayout>
#include <QVector>
#include "../src/views/Scheduler.h"  // Include path might need to be adjusted based on your project structure

class TimeClock : public QWidget {
    Q_OBJECT

public:
    explicit TimeClock(QWidget *parent = nullptr, Scheduler* scheduler = nullptr);  // Pass Scheduler reference
    void displayShifts();  // Method to display shifts

signals:
    void shiftRecorded(const QTime& startTime, const QTime& endTime);

private:
    QPushButton *clockInButton;
    QPushButton *clockOutButton;
    QLabel *statusLabel;
    QTime shiftStartTime;
    QTime shiftEndTime;
    Scheduler* scheduler;  // Pointer to the scheduler to access shifts
    void setupUI();
    void clockIn();
    void clockOut();
};

#endif // TIMECLOCK_H
