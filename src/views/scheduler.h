#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QWidget>
#include <QCalendarWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QVector>
#include <QDialog>
#include <QFormLayout>
#include <QLineEdit>
#include <QDateTimeEdit>
#include <QMessageBox>

struct Shift {
    QString jobTitle;
    QDateTime startTime;
    QDateTime endTime;

    bool operator==(const Shift& other) const {
        return jobTitle == other.jobTitle && startTime == other.startTime && endTime == other.endTime;
    }
};

class Scheduler : public QWidget {
    Q_OBJECT

public:
    explicit Scheduler(QWidget *parent = nullptr);
    void addShiftDialog();  // Function to open the dialog for adding a new shift

private:
    QCalendarWidget *calendar;
    QPushButton *addShiftButton;
    QVector<Shift> shifts;  // Vector to store shifts
    void setupUI();
    void saveShift(const Shift& newShift);  // Function to save and display new shifts
    void updateCalendarView();  // Function to update the calendar display
    void resetDateTextFormat(const QDate& date);  // New method to reset text format for a specific date
    void resetAllDateTextFormats();  // New method to reset text formats for all dates
};

#endif // SCHEDULER_H
