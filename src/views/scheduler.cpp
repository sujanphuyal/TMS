#include "Scheduler.h"
#include <QTextCharFormat>
#include <QDebug>

Scheduler::Scheduler(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Scheduler::setupUI() {
    QVBoxLayout *layout = new QVBoxLayout(this);
    calendar = new QCalendarWidget(this);
    calendar->setFirstDayOfWeek(Qt::Monday);
    layout->addWidget(calendar);

    addShiftButton = new QPushButton("Add New Shift", this);
    layout->addWidget(addShiftButton);
    connect(addShiftButton, &QPushButton::clicked, this, &Scheduler::addShiftDialog);
}

void Scheduler::addShiftDialog() {
    QDialog *dialog = new QDialog(this);
    dialog->setWindowTitle("Add New Shift");

    QFormLayout *formLayout = new QFormLayout(dialog);
    QLineEdit *jobTitleEdit = new QLineEdit(dialog);
    QDateTimeEdit *startTimeEdit = new QDateTimeEdit(dialog);
    startTimeEdit->setCalendarPopup(true);
    QDateTimeEdit *endTimeEdit = new QDateTimeEdit(dialog);
    endTimeEdit->setCalendarPopup(true);

    formLayout->addRow("Job Title:", jobTitleEdit);
    formLayout->addRow("Start Time:", startTimeEdit);
    formLayout->addRow("End Time:", endTimeEdit);

    QPushButton *saveButton = new QPushButton("Save", dialog);
    connect(saveButton, &QPushButton::clicked, [this, dialog, jobTitleEdit, startTimeEdit, endTimeEdit]() {
        Shift newShift { jobTitleEdit->text(), startTimeEdit->dateTime(), endTimeEdit->dateTime() };
        if (!newShift.jobTitle.isEmpty()) {
            this->saveShift(newShift);
            dialog->accept();
        } else {
            QMessageBox::warning(dialog, "Input Error", "Please enter a job title.");
        }
    });

    formLayout->addWidget(saveButton);
    dialog->setLayout(formLayout);
    dialog->exec();
}

void Scheduler::saveShift(const Shift& newShift) {
    for (const auto &shift : shifts) {
        if (shift.startTime < newShift.endTime && newShift.startTime < shift.endTime) {
            QMessageBox::critical(this, "Shift Conflict", "This shift overlaps with another. Please choose a different time.");
            return;
        }
    }
    shifts.append(newShift);
    updateCalendarView();
}

void Scheduler::updateCalendarView() {
    resetAllDateTextFormats();
    for (const auto &shift : shifts) {
        QTextCharFormat format;
        format.setBackground(Qt::yellow);  // Highlight days with shifts
        calendar->setDateTextFormat(shift.startTime.date(), format);
    }
}

void Scheduler::resetDateTextFormat(const QDate& date) {
    QTextCharFormat defaultFormat;  // Default, no formatting
    calendar->setDateTextFormat(date, defaultFormat);
}

void Scheduler::resetAllDateTextFormats() {
    QDate date = calendar->minimumDate();
    QDate endDate = calendar->maximumDate();
    QTextCharFormat defaultFormat;  // Default, no formatting

    while (date <= endDate) {
        calendar->setDateTextFormat(date, defaultFormat);
        date = date.addDays(1);
    }
}
