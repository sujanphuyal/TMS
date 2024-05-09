#include "Dashboard.h"
#include <QVBoxLayout>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QHeaderView>

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
    layout->setSpacing(20);  // Add spacing between widgets

    hoursLabel = new QLabel("Total Hours Allocated This Week: 0", this);
    hoursLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(hoursLabel);

    shiftsTable = new QTableWidget(this);
    shiftsTable->setColumnCount(4);
    shiftsTable->setHorizontalHeaderLabels(QStringList() << "Job Title" << "Start Time" << "End Time" << "Status");
    shiftsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    shiftsTable->horizontalHeader()->setStretchLastSection(true);
    shiftsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    shiftsTable->setSelectionMode(QAbstractItemView::NoSelection);
    shiftsTable->setStyleSheet("QTableWidget { border: 1px solid black; }"); // Add a border to the table
    layout->addWidget(shiftsTable);
}

void Dashboard::updateShiftsDisplay() {
    shiftsTable->clearContents();
    shiftsTable->setRowCount(0);

    auto shifts = scheduler->getAllShifts();  // Retrieve all shifts

    // Separate shifts into categories
    QVector<Shift> ongoingShifts;
    QVector<Shift> upcomingShifts;
    QVector<Shift> completedShifts;

    QDateTime now = QDateTime::currentDateTime();
    for (const auto& shift : shifts) {
        if (shift.endTime < now) {
            completedShifts.append(shift);
        } else if (shift.startTime <= now && shift.endTime >= now) {
            ongoingShifts.append(shift);
        } else if (shift.startTime > now) {
            upcomingShifts.append(shift);
        }
    }

    // Sort each category by start time
    auto sortShifts = [](const Shift& a, const Shift& b) { return a.startTime < b.startTime; };
    std::sort(ongoingShifts.begin(), ongoingShifts.end(), sortShifts);
    std::sort(upcomingShifts.begin(), upcomingShifts.end(), sortShifts);
    std::sort(completedShifts.begin(), completedShifts.end(), sortShifts);

    // Merge sorted lists
    QVector<Shift> sortedShifts = ongoingShifts + upcomingShifts + completedShifts;

    // Display sorted shifts in the table
    for (int i = 0; i < sortedShifts.size(); ++i) {
        const auto& shift = sortedShifts[i];
        shiftsTable->insertRow(shiftsTable->rowCount());
        shiftsTable->setItem(i, 0, new QTableWidgetItem(shift.jobTitle));
        shiftsTable->setItem(i, 1, new QTableWidgetItem(shift.startTime.toString("yyyy-MM-dd hh:mm AP"))); // 12-hour format with AM/PM
        shiftsTable->setItem(i, 2, new QTableWidgetItem(shift.endTime.toString("yyyy-MM-dd hh:mm AP")));

        QTableWidgetItem* statusItem = new QTableWidgetItem();
        QColor color; // Color to set for the row
        QString status;

        if (shift.endTime < now) {
            status = "Completed";
            color = QColor(Qt::green); // Green for completed shifts
        } else if (shift.startTime <= now && shift.endTime >= now) {
            status = "Ongoing";
            color = QColor(Qt::yellow); // Gray for ongoing shifts
        } else {
            status = "Upcoming";
            color = QColor(Qt::blue); // BLue for upcoming shifts
        }

        statusItem->setText(status);
        shiftsTable->setItem(i, 3, statusItem);

        // Apply color to all columns in the row
        for (int j = 0; j < shiftsTable->columnCount(); ++j) {
            shiftsTable->item(i, j)->setBackground(color);
        }
    }
}


void Dashboard::updateHoursWorked(int hours) {
    totalHoursWorked = hours;
    hoursLabel->setText(QString("Total Hours Allocated This Week: %1").arg(hours));
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
