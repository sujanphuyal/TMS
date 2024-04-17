#include "Scheduler.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

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
    // Check for conflicts before adding the shift
    for (const auto &shift : shifts) {
        if (shift.startTime < newShift.endTime && newShift.startTime < shift.endTime) {
            QMessageBox::critical(this, "Shift Conflict", "This shift overlaps with another. Please choose a different time.");
            return;
        }
    }
    shifts.append(newShift);
    saveShifts();
    updateCalendarView();
}

void Scheduler::updateCalendarView() {
    // Update the calendar display for shifts
    QTextCharFormat format;
    format.setBackground(Qt::yellow);
    for (const auto &shift : shifts) {
        calendar->setDateTextFormat(shift.startTime.date(), format);
    }
}

void Scheduler::shiftSelected(const QDate &date) {
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
}

void Scheduler::deleteShift(int index) {
    // Function to delete a shift
    shifts.removeAt(index);
    saveShifts();
    updateCalendarView();
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
