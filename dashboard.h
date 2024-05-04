#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>
#include <QTableWidget>
#include <QString>
#include "Scheduler.h"

class Dashboard : public QWidget {
public:
    explicit Dashboard(QWidget *parent = nullptr, Scheduler* scheduler = nullptr);
    void updateHoursWorked(int hours);
    void loadHoursWorked();   // Load hours from file
    void saveHoursWorked();   // Save hours to file
    void updateShiftsDisplay();  // Update the display of shifts

private:
    QLabel *hoursLabel;
    QVBoxLayout *shiftsLayout;  // Layout to display shifts information
    int totalHoursWorked = 0;  // Store total hours worked
    Scheduler* scheduler;       // Reference to the Scheduler
    QString hoursFilePath = "hours_worked.json";  // File path for storing hours

    void setupUI();
    void setupShiftsDisplay();  // Set up the UI part that displays shifts
};
#endif // DASHBOARD_H
