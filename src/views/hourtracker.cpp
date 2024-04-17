#include "HourTracker.h"

HourTracker::HourTracker(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void HourTracker::setupUI() {
    auto *layout = new QVBoxLayout(this);

    // Label to display total hours
    totalHoursLabel = new QLabel("Total Hours Worked: 0", this);
    totalHoursLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(totalHoursLabel);

    // Here you might want to connect to a signal that updates the hours displayed
    // For example:
    // connect(signalSource, &SignalSourceType::hoursUpdated, this, &HourTracker::updateHours);
}

void HourTracker::updateHours() {
    // Logic to update the displayed hours
    // This function needs to pull data from a model storing the work hours
    int hours = 0; // Placeholder for actual data retrieval
    totalHoursLabel->setText(QString("Total Hours Worked: %1").arg(hours));
}
