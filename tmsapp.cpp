#include "TMSApp.h"
#include "Dashboard.h"
#include "Scheduler.h"
#include "HourTracker.h"
#include "TimeClock.h"
#include "NotificationManager.h"
#include "ReportGenerator.h"
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

    // Central Widget with tabs
    QTabWidget *tabWidget = new QTabWidget(this);
    setCentralWidget(tabWidget);

    // Initialize components
    Scheduler *scheduler = new Scheduler(tabWidget);
    Dashboard *dashboard = new Dashboard(tabWidget, scheduler);
    HourTracker *hourTracker = new HourTracker(tabWidget);
    TimeClock *timeClock = new TimeClock(tabWidget, scheduler);
    NotificationManager *notificationManager = new NotificationManager(scheduler, this);
    ReportGenerator *reportGenerator = new ReportGenerator(scheduler);

    // Add Report tab
    QWidget *reportTab = new QWidget();
    QVBoxLayout *reportLayout = new QVBoxLayout(reportTab);
    QDateEdit *fromDateEdit = new QDateEdit();
    fromDateEdit->setCalendarPopup(true);
    fromDateEdit->setDate(QDate::currentDate());
    QDateEdit *toDateEdit = new QDateEdit();
    toDateEdit->setCalendarPopup(true);
    toDateEdit->setDate(QDate::currentDate().addDays(7));

    QPushButton *generateReportButton = new QPushButton("Generate Report");
    QTextBrowser *reportDisplay = new QTextBrowser();

    reportLayout->addWidget(new QLabel("Select From Date:"));
    reportLayout->addWidget(fromDateEdit);
    reportLayout->addWidget(new QLabel("Select To Date:"));
    reportLayout->addWidget(toDateEdit);
    reportLayout->addWidget(generateReportButton);
    reportLayout->addWidget(new QLabel("Report Output:"));
    reportLayout->addWidget(reportDisplay);


    // Add tabs
    tabWidget->addTab(dashboard, "Dashboard");
    tabWidget->addTab(scheduler, "Scheduler");
    tabWidget->addTab(hourTracker, "Hour Tracker");
    tabWidget->addTab(timeClock, "Time Clock");
    tabWidget->addTab(reportTab, "Reports");


    // Connect button click to report generation
    connect(generateReportButton, &QPushButton::clicked, [reportGenerator, fromDateEdit, toDateEdit, reportDisplay]() {
        QString report = reportGenerator->generateReport(fromDateEdit->date(), toDateEdit->date());
        reportDisplay->setText(report);
    });

    // Connect signals and slots
    connect(scheduler, &Scheduler::shiftsUpdated, timeClock, &TimeClock::updateShiftDisplay);

    // connect(scheduler, &Scheduler::shiftsUpdated, timeClock, &TimeClock::displayShifts);

    connect(scheduler, &Scheduler::shiftsUpdated, dashboard, &Dashboard::updateShiftsDisplay);

    connect(scheduler, &Scheduler::shiftsUpdated, [dashboard, scheduler]() {
        QDate currentDate = QDate::currentDate();
        int hours = scheduler->calculateWeeklyHours(currentDate);
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
