#include "TimeClock.h"
#include <QDateTime>

TimeClock::TimeClock(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void TimeClock::setupUI() {
    auto *layout = new QVBoxLayout(this);

    // Status label to show the current state
    statusLabel = new QLabel("You are currently clocked out.", this);
    layout->addWidget(statusLabel);

    // Clock In button
    clockInButton = new QPushButton("Clock In", this);
    layout->addWidget(clockInButton);
    connect(clockInButton, &QPushButton::clicked, this, &TimeClock::clockIn);

    // Clock Out button
    clockOutButton = new QPushButton("Clock Out", this);
    clockOutButton->setEnabled(false);  // Disable until clocked in
    layout->addWidget(clockOutButton);
    connect(clockOutButton, &QPushButton::clicked, this, &TimeClock::clockOut);
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

    // Here, you would also handle logging the shift duration to the system for hour tracking
}
