#include "HourTracker.h"

HourTracker::HourTracker(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void HourTracker::setupUI() {
    auto *layout = new QVBoxLayout(this);
    totalHoursLabel = new QLabel("Total Hours Worked: 0 hours, 0 minutes", this);
    totalHoursLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(totalHoursLabel);
}

void HourTracker::addHours(const QTime& startTime, const QTime& endTime) {
    int minutes = startTime.msecsTo(endTime) / 60000;  // Convert milliseconds to minutes
    totalMinutes += minutes;
    updateDisplay();
    checkCompliance();
}

void HourTracker::updateDisplay() {
    int hours = totalMinutes / 60;
    int minutes = totalMinutes % 60;
    totalHoursLabel->setText(QString("Total Hours Worked: %1 hours, %2 minutes").arg(hours).arg(minutes));
}

void HourTracker::checkCompliance() {
    const int legalLimit = 24 * 60; // Assuming a 24-hour weekly limit, in minutes
    if (totalMinutes >= legalLimit) {
        QMessageBox::warning(this, "Compliance Alert", "You have exceeded the legal work-hour limit!");
    } else if (totalMinutes >= legalLimit - 60) { // Alert 1 hours before reaching the limit
        QMessageBox::warning(this, "Compliance Alert", "You are close to exceeding the legal work-hour limit!");
    }
}
