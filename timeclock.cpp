#include "TimeClock.h"
#include <QDateTime>
#include <QTableWidget>
#include <QHeaderView>

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

    // Initialize the table for displaying shifts
    shiftsTable = new QTableWidget(this);
    shiftsTable->setColumnCount(4); // Now includes columns for Shift Title, Start Time, End Time, Status
    shiftsTable->setHorizontalHeaderLabels(QStringList() << "Shift Title" << "Start Time" << "End Time" << "Status");
    shiftsTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    shiftsTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    shiftsTable->setSelectionMode(QAbstractItemView::NoSelection);
    mainLayout->addWidget(shiftsTable);  // Add table to main layout

    displayShifts();  // Call to display shifts
}

void TimeClock::clockIn() {
    shiftStartTime = QTime::currentTime();
    statusLabel->setText("Clocked in at: " + shiftStartTime.toString("hh:mm AP"));
    clockInButton->setEnabled(false);
    clockOutButton->setEnabled(true);
}

void TimeClock::clockOut() {
    shiftEndTime = QTime::currentTime();
    QString message = "Clocked in at: " + shiftStartTime.toString("hh:mm AP") +
                      " | Clocked out at: " + shiftEndTime.toString("hh:mm AP");
    statusLabel->setText(message);
    clockOutButton->setEnabled(false);
    clockInButton->setEnabled(true);

    emit shiftRecorded(shiftStartTime, shiftEndTime);
}

void TimeClock::displayShifts() {
    QDateTime now = QDateTime::currentDateTime();
    auto shifts = scheduler->getShiftsForDate(now.date());  // Fetch shifts for the current date

    QVector<Shift> ongoingShifts;
    QVector<Shift> upcomingShifts;
    QVector<Shift> completedShifts;

    // Categorize shifts
    for (const auto &shift : shifts) {
        if (shift.endTime < now) {
            completedShifts.append(shift);
        } else if (shift.startTime <= now && shift.endTime >= now) {
            ongoingShifts.append(shift);
        } else if (shift.startTime > now) {
            upcomingShifts.append(shift);
        }
    }

    // Sort each category by start time
    auto sortByStartTime = [](const Shift& a, const Shift& b) { return a.startTime < b.startTime; };
    std::sort(ongoingShifts.begin(), ongoingShifts.end(), sortByStartTime);
    std::sort(upcomingShifts.begin(), upcomingShifts.end(), sortByStartTime);
    std::sort(completedShifts.begin(), completedShifts.end(), sortByStartTime);

    // Merge the sorted lists
    QVector<Shift> sortedShifts = ongoingShifts + upcomingShifts + completedShifts;

    // Populate the table
    shiftsTable->clearContents();
    shiftsTable->setRowCount(0);

    for (int i = 0; i < sortedShifts.size(); ++i) {
        const auto& shift = sortedShifts[i];
        int row = shiftsTable->rowCount();
        shiftsTable->insertRow(row);
        shiftsTable->setItem(row, 0, new QTableWidgetItem(shift.jobTitle));
        shiftsTable->setItem(row, 1, new QTableWidgetItem(shift.startTime.toString("hh:mm AP")));
        shiftsTable->setItem(row, 2, new QTableWidgetItem(shift.endTime.toString("hh:mm AP")));

        QString status;
        QColor color; // Color for row based on status
        if (shift.endTime < now) {
            status = "Completed";
            color = QColor(Qt::green);
        } else if (shift.startTime <= now && shift.endTime >= now) {
            status = "Ongoing";
            color = QColor(Qt::yellow);
        } else {
            status = "Upcoming";
            color = QColor(Qt::blue);
        }

        shiftsTable->setItem(row, 3, new QTableWidgetItem(status));
        for (int j = 0; j < shiftsTable->columnCount(); ++j) {
            shiftsTable->item(row, j)->setBackground(color);
        }
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



