#include "TMSApp.h"
#include "../src/views/Dashboard.h"
#include "../src/views/Scheduler.h"
#include "../src/views/HourTracker.h"
#include "../src/views/TimeClock.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QTabWidget>
#include <QWidget>

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
    auto *dashboard = new Dashboard(tabWidget);
    auto *scheduler = new Scheduler(tabWidget);
    auto *hourTracker = new HourTracker(tabWidget);
    auto *timeClock = new TimeClock(tabWidget);

    tabWidget->addTab(dashboard, "Dashboard");
    tabWidget->addTab(scheduler, "Scheduler");
    tabWidget->addTab(hourTracker, "Hour Tracker");
    tabWidget->addTab(timeClock, "Time Clock");

    // Connect signals and slots
    connect(scheduler, &Scheduler::shiftsUpdated, [dashboard, scheduler]() {
        int hours = scheduler->calculateWeeklyHours();
        dashboard->updateHoursWorked(hours);
    });
    connect(timeClock, &TimeClock::shiftRecorded, [hourTracker](const QTime& startTime, const QTime& endTime) {
        hourTracker->addHours(startTime, endTime);
    });

}

TMSApp::~TMSApp() {
    // Cleanup if necessary
}
