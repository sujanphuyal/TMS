#include "TimeClock.h"
#include <QDateTime>

TimeClock::TimeClock(QWidget *parent, Scheduler* schedulerRef) : QWidget(parent), scheduler(schedulerRef) {
    setupUI();
}

void TimeClock::setupUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout(this);  // Main layout

    // Status label and buttons layout
    QVBoxLayout *controlLayout = new QVBoxLayout();

    // Status label
    statusLabel = new QLabel("Status: Clocked Out", this);
    controlLayout->addWidget(statusLabel);

    // Clock In button
    clockInButton = new QPushButton("Clock In", this);
    controlLayout->addWidget(clockInButton);
    connect(clockInButton, &QPushButton::clicked, this, &TimeClock::clockIn);

    // Clock Out button
    clockOutButton = new QPushButton("Clock Out", this);
    clockOutButton->setEnabled(false);
    controlLayout->addWidget(clockOutButton);
    connect(clockOutButton, &QPushButton::clicked, this, &TimeClock::clockOut);

    mainLayout->addLayout(controlLayout);  // Add control layout to main layout

    // Layout for displaying shifts (dynamic content)
    layout = new QVBoxLayout();
    mainLayout->addLayout(layout);  // Add shift layout to main layout

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
    auto shifts = scheduler->getShiftsForDate(now.date());

    clearShiftDisplay();  // Clear the existing display before updating

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
        layout->addWidget(shiftLabel);
    }
}

void TimeClock::updateShiftDisplay() {
    displayShifts();  // Re-display the shifts
}

void TimeClock::clearShiftDisplay() {
    QLayoutItem *item;
    while ((item = layout->takeAt(0)) != nullptr) {
        delete item->widget();  // Delete the widget
        delete item;            // Delete the layout item
    }
}



