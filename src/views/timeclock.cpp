#include "TimeClock.h"
#include <QDateTime>

TimeClock::TimeClock(QWidget *parent, Scheduler* schedulerRef) : QWidget(parent), scheduler(schedulerRef) {
    setupUI();
}

void TimeClock::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);

    // Status label
    statusLabel = new QLabel("Status: Clocked Out", this);
    layout->addWidget(statusLabel);

    // Clock In button
    clockInButton = new QPushButton("Clock In", this);
    layout->addWidget(clockInButton);
    connect(clockInButton, &QPushButton::clicked, this, &TimeClock::clockIn);

    // Clock Out button
    clockOutButton = new QPushButton("Clock Out", this);
    clockOutButton->setEnabled(false);
    layout->addWidget(clockOutButton);
    connect(clockOutButton, &QPushButton::clicked, this, &TimeClock::clockOut);

    displayShifts();  // Call to display shifts
}

void TimeClock::clockIn() {
    shiftStartTime = QTime::currentTime();
    statusLabel->setText("Clocked in at: " + shiftStartTime.toString("hh:mm:ss"));
    clockInButton->setEnabled(false);
    clockOutButton->setEnabled(true);
}

void TimeClock::clockOut() {
    shiftEndTime = QTime::currentTime();
    statusLabel->setText("Clocked out at: " + shiftEndTime.toString("hh:mm:ss"));
    clockOutButton->setEnabled(false);
    clockInButton->setEnabled(true);

    emit shiftRecorded(shiftStartTime, shiftEndTime);
}

void TimeClock::displayShifts() {
    QDateTime now = QDateTime::currentDateTime();
    auto shifts = scheduler->getShiftsForDate(now.date());  // Assuming getShiftsForDate is implemented in Scheduler

    for (const auto &shift : shifts) {
        QString status;
        if (shift.endTime < now) {
            status = "Finished";
        } else if (shift.startTime > now) {
            status = "Upcoming";
        } else {
            status = "Running";
        }
        QLabel* shiftLabel = new QLabel(QString("Shift: %1 - %2, Status: %3").arg(shift.startTime.toString(), shift.endTime.toString(), status), this);
        this->layout()->addWidget(shiftLabel);
    }
}
