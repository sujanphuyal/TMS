#include "Scheduler.h"

Scheduler::Scheduler(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Scheduler::setupUI() {
    auto *layout = new QVBoxLayout(this);

    // Calendar widget to display shifts
    calendar = new QCalendarWidget(this);
    calendar->setFirstDayOfWeek(Qt::Monday);
    layout->addWidget(calendar);

    // Button to add new shifts
    addShiftButton = new QPushButton("Add New Shift", this);
    layout->addWidget(addShiftButton);

    // Connect button signal to slot for adding shifts (slot to be implemented)
    connect(addShiftButton, &QPushButton::clicked, this, [this]() {
        // Placeholder for adding shift functionality
        // This would ideally open a dialog to input shift details
    });
}
