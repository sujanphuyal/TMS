#include "Scheduler.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>
#include <QDate>

Scheduler::Scheduler(QWidget *parent) : QWidget(parent) {
    setupUI();
    loadShifts();  // Load shifts from file or database on startup
}

void Scheduler::setupUI() {
    auto *layout = new QVBoxLayout(this);
    calendar = new QCalendarWidget(this);
    calendar->setSelectionMode(QCalendarWidget::SelectionMode::SingleSelection);
    connect(calendar, &QCalendarWidget::clicked, this, &Scheduler::shiftSelected);
    layout->addWidget(calendar);

    addShiftButton = new QPushButton("Add New Shift", this);
    connect(addShiftButton, &QPushButton::clicked, this, &Scheduler::addShiftDialog);
    layout->addWidget(addShiftButton);
}

void Scheduler::addShiftDialog() {
    QDialog dialog(this);
    dialog.setWindowTitle("Add New Shift");
    QFormLayout formLayout(&dialog);

    QLineEdit jobTitleEdit;
    QDateTimeEdit startTimeEdit;
    startTimeEdit.setCalendarPopup(true);
    QDateTimeEdit endTimeEdit;
    endTimeEdit.setCalendarPopup(true);

    QDateTime defaultStartTime(selectedDate, QTime(9, 0)); // Default start time at 9:00 AM
    QDateTime defaultEndTime(selectedDate, QTime(17, 0));  // Default end time at 5:00 PM
    startTimeEdit.setDateTime(defaultStartTime);
    endTimeEdit.setDateTime(defaultEndTime);

    formLayout.addRow("Job Title:", &jobTitleEdit);
    formLayout.addRow("Start Time:", &startTimeEdit);
    formLayout.addRow("End Time:", &endTimeEdit);

    QPushButton saveButton("Save", &dialog);
    formLayout.addWidget(&saveButton);
    connect(&saveButton, &QPushButton::clicked, [&]() {
        Shift newShift { jobTitleEdit.text(), startTimeEdit.dateTime(), endTimeEdit.dateTime() };
        if (!newShift.jobTitle.isEmpty()) {
            saveShift(newShift);
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Input Error", "Please enter a job title.");
        }
    });

    dialog.setLayout(&formLayout);
    dialog.exec();
}

void Scheduler::saveShift(const Shift& newShift) {
    // Calculate the total hours including the new shift
    int additionalMinutes = newShift.startTime.secsTo(newShift.endTime) / 60;
    int projectedHours = (calculateWeeklyHours() * 60 + additionalMinutes) / 60;

    if (projectedHours > 24) {
        QMessageBox::warning(this, "Shift Limit Exceeded", "Adding this shift would exceed the 24-hour limit for the week.");
        return;  // Do not add the shift if it exceeds the limit
    }

    // Check for conflicts before adding the shift
    for (const auto &shift : shifts) {
        if (shift.startTime < newShift.endTime && newShift.startTime < shift.endTime) {
            QMessageBox::warning(this, "Shift Conflict", "This shift overlaps with another. Please choose a different time.");
            return;
        }
    }

    shifts.append(newShift);
    emit shiftsUpdated();  // Emit signal after updating shifts
    saveShifts();
    updateCalendarView();
}

void Scheduler::updateCalendarView() {
    // Update the calendar display for shifts
    QTextCharFormat format;
    format.setBackground(Qt::blue);
    for (const auto &shift : shifts) {
        calendar->setDateTextFormat(shift.startTime.date(), format);
    }
}

void Scheduler::shiftSelected(const QDate &date) {
    selectedDate = date;  // Save the selected date
    // Find shifts for the selected date
    QVector<int> shiftIndexes;
    for (int i = 0; i < shifts.size(); ++i) {
        if (shifts[i].startTime.date() == date) {
            shiftIndexes.append(i);
        }
    }

    if (!shiftIndexes.isEmpty()) {
        QMenu menu;
        for (int index : shiftIndexes) {
            QAction *editAction = menu.addAction("Edit " + shifts[index].jobTitle);
            connect(editAction, &QAction::triggered, [this, index]() { this->editShift(index); });
            QAction *deleteAction = menu.addAction("Delete " + shifts[index].jobTitle);
            connect(deleteAction, &QAction::triggered, [this, index]() { this->deleteShift(index); });
        }
        menu.exec(QCursor::pos());
    }
}

void Scheduler::editShift(int index) {
    // Function to edit a shift at a specific index
    QDialog dialog(this);
    dialog.setWindowTitle("Edit Shift");
    QFormLayout formLayout(&dialog);

    QLineEdit jobTitleEdit;
    jobTitleEdit.setText(shifts[index].jobTitle);
    QDateTimeEdit startTimeEdit;
    startTimeEdit.setDateTime(shifts[index].startTime);
    startTimeEdit.setCalendarPopup(true);
    QDateTimeEdit endTimeEdit;
    endTimeEdit.setDateTime(shifts[index].endTime);
    endTimeEdit.setCalendarPopup(true);

    formLayout.addRow("Job Title:", &jobTitleEdit);
    formLayout.addRow("Start Time:", &startTimeEdit);
    formLayout.addRow("End Time:", &endTimeEdit);

    QPushButton saveButton("Save Changes", &dialog);
    formLayout.addWidget(&saveButton);
    connect(&saveButton, &QPushButton::clicked, [&]() {
        Shift editedShift { jobTitleEdit.text(), startTimeEdit.dateTime(), endTimeEdit.dateTime() };
        if (!editedShift.jobTitle.isEmpty()) {
            shifts[index] = editedShift;
            saveShifts();
            updateCalendarView();
            dialog.accept();
        } else {
            QMessageBox::warning(&dialog, "Input Error", "Please enter a job title.");
        }
    });

    dialog.setLayout(&formLayout);
    dialog.exec();
    emit shiftsUpdated();  // Emit signal after updating shifts
}

void Scheduler::deleteShift(int index) {
    // Function to delete a shift
    shifts.removeAt(index);
    saveShifts();
    updateCalendarView();
    emit shiftsUpdated();  // Emit signal after updating shifts
}

void Scheduler::loadShifts() {
    QFile file("shifts.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonArray shiftsArray = doc.array();
        for (auto val : shiftsArray) {
            QJsonObject obj = val.toObject();
            Shift shift;
            shift.jobTitle = obj["jobTitle"].toString();
            shift.startTime = QDateTime::fromString(obj["startTime"].toString(), Qt::ISODate);
            shift.endTime = QDateTime::fromString(obj["endTime"].toString(), Qt::ISODate);
            shifts.append(shift);
        }
        file.close();
        updateCalendarView();  // Make sure to update the calendar view after loading the shifts
    }
}

void Scheduler::saveShifts() {
    QFile file("shifts.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray shiftsArray;
        for (const auto &shift : shifts) {
            QJsonObject obj;
            obj["jobTitle"] = shift.jobTitle;
            obj["startTime"] = shift.startTime.toString(Qt::ISODate);
            obj["endTime"] = shift.endTime.toString(Qt::ISODate);
            shiftsArray.append(obj);
        }
        QJsonDocument doc(shiftsArray);
        file.write(doc.toJson());
        file.close();
    }
}

int Scheduler::calculateWeeklyHours() {
    int totalMinutes = 0;
    QDateTime now = QDateTime::currentDateTime();
    QDate startOfWeek = QDate::currentDate().addDays(-QDate::currentDate().dayOfWeek() + 1);

    for (const auto &shift : shifts) {
        if (shift.startTime.date() >= startOfWeek && shift.endTime <= now) {
            totalMinutes += shift.startTime.secsTo(shift.endTime) / 60;
        }
    }
    int totalHours = totalMinutes / 60;

    // Emit alert if hours are about to exceed or have exceeded the limit
    if (totalHours >= 24) {
        QMessageBox::critical(nullptr, "Weekly Hour Limit Exceeded", "You have exceeded the maximum of 24 work hours this week.");
    } else if (totalHours >= 22) {  // Alert two hours before reaching the limit
        QMessageBox::warning(nullptr, "Weekly Hour Limit Approaching", "You are close to exceeding the 24-hour work limit this week.");
    }

    return totalHours;
}

QVector<Shift> Scheduler::getShiftsForDate(const QDate& date) {
    QVector<Shift> result;
    for (const Shift& shift : shifts) {
        if (shift.startTime.date() == date) {
            result.push_back(shift);
        }
    }
    return result;
}

void Scheduler::saveWeeklyHours() {
    QFile file("weekly_hours.json");
    if (file.open(QIODevice::WriteOnly)) {
        QJsonObject root;
        for (auto it = weeklyHours.begin(); it != weeklyHours.end(); ++it) {
            root[it.key().toString("yyyy-MM-dd")] = it.value();
        }
        QJsonDocument doc(root);
        file.write(doc.toJson());
        file.close();
    }
}

void Scheduler::loadWeeklyHours() {
    QFile file("weekly_hours.json");
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject root = doc.object();
        for (auto it = root.begin(); it != root.end(); ++it) {
            weeklyHours[QDate::fromString(it.key(), "yyyy-MM-dd")] = it.value().toInt();
        }
        file.close();
    }
}
