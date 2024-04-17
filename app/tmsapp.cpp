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
    // Set the size and title of the main window
    this->resize(800, 600);
    this->setWindowTitle("Time Management System");

    // Setup Menu
    auto *fileMenu = menuBar()->addMenu(tr("&File"));
    auto *exitAction = fileMenu->addAction(tr("Exit"));
    connect(exitAction, &QAction::triggered, this, &TMSApp::close);

    // Setup Status Bar
    statusBar()->showMessage("Ready");

    // Central Widget with tabs
    auto *tabWidget = new QTabWidget(this);
    this->setCentralWidget(tabWidget);

    auto *hourTracker = new HourTracker(tabWidget);
    auto *timeClock = new TimeClock(tabWidget);
    connect(timeClock, &TimeClock::shiftRecorded, hourTracker, &HourTracker::addHours);

    // Add Dashboard, Scheduler, Hour Tracker, and Time Clock tabs
    tabWidget->addTab(new Dashboard(tabWidget), "Dashboard");
    tabWidget->addTab(new Scheduler(tabWidget), "Scheduler");
    tabWidget->addTab(hourTracker, "Hour Tracker");
    tabWidget->addTab(timeClock, "Time Clock");
}

TMSApp::~TMSApp() {
    // Handle destruction if needed
}
