#include "NotificationManager.h"
#include "Scheduler.h"
#include <QDateTime>
#include <QMessageBox>

NotificationManager::NotificationManager(Scheduler* scheduler, QObject *parent)
    : QObject(parent), m_scheduler(scheduler) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, &NotificationManager::checkForNotifications);
    m_timer->start(60000);  // Check every minute
}

NotificationManager::~NotificationManager() {
    m_timer->stop();
}

void NotificationManager::checkForNotifications() {
    QDateTime now = QDateTime::currentDateTime();
    // Check if there are upcoming shifts within the next hour
    auto shifts = m_scheduler->getShiftsForDate(now.date());
    for (const auto &shift : shifts) {
        if (shift.startTime > now && shift.startTime < now.addSecs(3600)) {  // Shift starts within the next hour
            emit sendNotification("Shift Reminder", QString("You have a shift starting soon at %1.").arg(shift.startTime.toString("hh:mm")));
        }
    }
}
