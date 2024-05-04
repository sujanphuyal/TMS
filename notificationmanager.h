#ifndef NOTIFICATIONMANAGER_H
#define NOTIFICATIONMANAGER_H

#include <QObject>
#include <QTimer>

class Scheduler;  // Forward declaration

class NotificationManager : public QObject {
    Q_OBJECT

public:
    explicit NotificationManager(Scheduler* scheduler, QObject *parent = nullptr);
    ~NotificationManager();

private:
    Scheduler* m_scheduler;  // Reference to the Scheduler to access shift data
    QTimer *m_timer;  // Timer to trigger notifications

private slots:
    void checkForNotifications();  // Slot to check and display notifications

signals:
    void sendNotification(const QString &title, const QString &message);  // Signal to send notifications
};

#endif // NOTIFICATIONMANAGER_H
