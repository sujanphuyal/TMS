#ifndef TIMECLOCK_H
#define TIMECLOCK_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QVBoxLayout>
#include <QVector>
#include "Scheduler.h"

class TimeClock : public QWidget {
    Q_OBJECT

public:
    explicit TimeClock(QWidget *parent = nullptr, Scheduler* scheduler = nullptr);  // Pass Scheduler reference
    void displayShifts();  // Method to display shifts
    void updateShiftDisplay();  // Method to update the shift display when shifts are updated


signals:
    void shiftRecorded(const QTime& startTime, const QTime& endTime);

private:
    QPushButton *clockInButton;
    QPushButton *clockOutButton;
    QLabel *statusLabel;
    QTime shiftStartTime;
    QTime shiftEndTime;
    Scheduler* scheduler;  // Pointer to the scheduler to access shifts
    QVBoxLayout *layout;   // Layout for displaying shift information
    void setupUI();
    void clockIn();
    void clockOut();
    void clearShiftDisplay();  // Method to clear the current shift display
};

#endif // TIMECLOCK_H
