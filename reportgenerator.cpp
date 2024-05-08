#include "ReportGenerator.h"

ReportGenerator::ReportGenerator(Scheduler* scheduler)
    : scheduler(scheduler) {}

QString ReportGenerator::generateReport(QDate from, QDate to) {
    QVector<Shift> shifts = scheduler->getShiftsInRange(from, to);
    QString report;
    for (const Shift& shift : shifts) {
        report += QString("Job Title: %1, Start: %2, End: %3\n")
                      .arg(shift.jobTitle)
                      .arg(shift.startTime.toString())
                      .arg(shift.endTime.toString());
    }
    return report;
}
