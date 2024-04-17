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
#include <QMenu>

struct Shift {
    QString jobTitle;
    QDateTime startTime;
    QDateTime endTime;
};

class Scheduler : public QWidget {
    Q_OBJECT

public:
    explicit Scheduler(QWidget *parent = nullptr);
    void addShiftDialog();  // Function to open the dialog for adding a new shift
    void loadShifts();
    void saveShifts();

private:
    QCalendarWidget *calendar;
    QPushButton *addShiftButton;
    QVector<Shift> shifts;  // Vector to store shifts
    void setupUI();
    void saveShift(const Shift& newShift);  // Function to save and display new shifts
    void updateCalendarView();  // Update the calendar view to reflect shifts
    void shiftSelected(const QDate &date);  // Handle shift selection from the calendar
    void editShift(int index);  // Edit a selected shift
    void deleteShift(int index);  // Delete a selected shift
};

#endif // SCHEDULER_H
