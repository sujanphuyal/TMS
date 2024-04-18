#ifndef DASHBOARD_H
#define DASHBOARD_H

#include <QWidget>
#include <QLabel>

class Dashboard : public QWidget {
public:
    explicit Dashboard(QWidget *parent = nullptr);

    void updateHoursWorked(int hours);  // Method to update hours displayed

private:
    QLabel *hoursLabel;  // Label to display the hours
    void setupUI();
};

#endif // DASHBOARD_H
