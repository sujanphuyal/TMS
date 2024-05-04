#include "Dashboard.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>

Dashboard::Dashboard(QWidget *parent, Scheduler* schedulerRef)
    : QWidget(parent), scheduler(schedulerRef) {
    if (scheduler == nullptr) {
        throw std::runtime_error("Scheduler reference must not be null");
    }
    setupUI();
    loadHoursWorked();
    updateShiftsDisplay();  // Update shifts display on startup
}

void Dashboard::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    hoursLabel = new QLabel("Total Hours Worked This Week: 0", this);
    hoursLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hoursLabel);

    shiftsLayout = new QVBoxLayout();  // Initialize the layout for shifts
    layout->addLayout(shiftsLayout);
    setupShiftsDisplay();
}

void Dashboard::setupShiftsDisplay() {
    QDateTime now = QDateTime::currentDateTime();
    auto shifts = scheduler->getAllShifts();  // Assuming a method to get all shifts

    for (const auto& shift : shifts) {
        QLabel *shiftLabel = new QLabel(this);
        QString status;
        if (shift.endTime < now) {
            status = "Completed";
        } else if (shift.startTime > now) {
            status = "Upcoming";
        } else {
            status = "Ongoing";
        }
        shiftLabel->setText(QString("%1: %2 to %3 - %4")
                                .arg(shift.jobTitle)
                                .arg(shift.startTime.toString("yyyy-MM-dd hh:mm"))
                                .arg(shift.endTime.toString("yyyy-MM-dd hh:mm"))
                                .arg(status));
        shiftsLayout->addWidget(shiftLabel);
    }
}

void Dashboard::updateShiftsDisplay() {
    // Clear existing widgets in the layout
    QLayoutItem *item;
    while ((item = shiftsLayout->takeAt(0)) != nullptr) {
        delete item->widget();  // Delete the widget
        delete item;            // Delete the layout item
    }

    setupShiftsDisplay();  // Re-setup the shifts display
}

void Dashboard::updateHoursWorked(int hours) {
    totalHoursWorked = hours;
    hoursLabel->setText(QString("Total Hours Worked This Week: %1").arg(hours));
    saveHoursWorked();  // Save hours whenever they are updated
}

void Dashboard::loadHoursWorked() {
    QFile file(hoursFilePath);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        totalHoursWorked = doc.object().value("hoursWorked").toInt();
        updateHoursWorked(totalHoursWorked);
        file.close();
    }
}

void Dashboard::saveHoursWorked() {
    QFile file(hoursFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject root;
        root["hoursWorked"] = totalHoursWorked;
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    }
}
