#include "Dashboard.h"
#include <QVBoxLayout>

Dashboard::Dashboard(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Dashboard::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    hoursLabel = new QLabel("Total Hours Worked This Week: 0", this);
    hoursLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hoursLabel);
}

void Dashboard::updateHoursWorked(int hours) {
    hoursLabel->setText(QString("Total Hours Worked This Week: %1").arg(hours));
}
