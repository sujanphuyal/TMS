#ifndef TIMECLOCK_H
#define TIMECLOCK_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QTime>
#include <QVBoxLayout>

class TimeClock : public QWidget {
    Q_OBJECT

public:
    explicit TimeClock(QWidget *parent = nullptr);

signals:
    void shiftRecorded(const QTime& startTime, const QTime& endTime);

private:
    QPushButton *clockInButton;
    QPushButton *clockOutButton;
    QLabel *statusLabel;
    QTime shiftStartTime;
    QTime shiftEndTime;
    void setupUI();
    void clockIn();
    void clockOut();
};

#endif // TIMECLOCK_H
