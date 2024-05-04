#include "TMSApp.h"
#include "Dashboard.h"
#include "Scheduler.h"
#include "HourTracker.h"
#include "TimeClock.h"
#include "NotificationManager.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QFileDialog>
#include <QTextBrowser>

TMSApp::TMSApp(QWidget *parent) : QMainWindow(parent) {
    this->resize(800, 600);
    this->setWindowTitle("Time Management System");

    // Setup Menu
    QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
    QAction *exitAction = fileMenu->addAction(tr("Exit"));
    connect(exitAction, &QAction::triggered, this, &QMainWindow::close);

    // Setup Status Bar
    statusBar()->showMessage("Ready");

    // Central Widget with tabs
    QTabWidget *tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // Initialize components
    Scheduler *scheduler = new Scheduler(tabWidget);
    Dashboard *dashboard = new Dashboard(tabWidget, scheduler);
    HourTracker *hourTracker = new HourTracker(tabWidget);
    TimeClock *timeClock = new TimeClock(tabWidget, scheduler);
    NotificationManager *notificationManager = new NotificationManager(scheduler, this);

    // Add tabs
    tabWidget->addTab(dashboard, "Dashboard");
    tabWidget->addTab(scheduler, "Scheduler");
    tabWidget->addTab(hourTracker, "Hour Tracker");
    tabWidget->addTab(timeClock, "Time Clock");

    // Connect signals and slots
    connect(scheduler, &Scheduler::shiftsUpdated, timeClock, &TimeClock::updateShiftDisplay);

    // connect(scheduler, &Scheduler::shiftsUpdated, timeClock, &TimeClock::displayShifts);

    connect(scheduler, &Scheduler::shiftsUpdated, dashboard, &Dashboard::updateShiftsDisplay);

    connect(scheduler, &Scheduler::shiftsUpdated, [dashboard, scheduler]() {
        int hours = scheduler->calculateWeeklyHours();
        dashboard->updateHoursWorked(hours);
    });
    connect(timeClock, &TimeClock::shiftRecorded, [hourTracker](const QTime& startTime, const QTime& endTime) {
        hourTracker->addHours(startTime, endTime);
    });
    connect(notificationManager, &NotificationManager::sendNotification, this, [](const QString &title, const QString &message) {
        QMessageBox::information(nullptr, title, message);  // Display the notification as a message box
    });
}

TMSApp::~TMSApp() {
    // Cleanup if necessary
}
