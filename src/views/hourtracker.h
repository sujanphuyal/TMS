#ifndef HOURTRACKER_H
#define HOURTRACKER_H

#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>

class HourTracker : public QWidget {
    Q_OBJECT

public:
    explicit HourTracker(QWidget *parent = nullptr);

private:
    QLabel *totalHoursLabel;  // Display total hours worked
    void setupUI();
    void updateHours();       // Placeholder for updating the hours displayed
};

#endif // HOURTRACKER_H
