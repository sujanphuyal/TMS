#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "Scheduler.h"
#include <QDate>
#include <QString>

class ReportGenerator {
public:
    ReportGenerator(Scheduler* scheduler);
    QString generateReport(QDate from, QDate to);

private:
    Scheduler* scheduler;  // Pointer to Scheduler to access shifts
};

#endif // REPORTGENERATOR_H
