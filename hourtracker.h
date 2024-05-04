#ifndef HOURTRACKER_H
#define HOURTRACKER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QTime>
#include <QMessageBox>

class HourTracker : public QWidget {
    Q_OBJECT

public:
    explicit HourTracker(QWidget *parent = nullptr);
    void addHours(const QTime& startTime, const QTime& endTime);

private:
    QLabel *totalHoursLabel;  // Display total hours worked
    int totalMinutes = 0;     // Store total minutes worked
    void setupUI();
    void updateDisplay();     // Update the hour display
    void checkCompliance();   // Check and handle compliance issues
};

#endif // HOURTRACKER_H
