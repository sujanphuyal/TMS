#include "ReportGenerator.h"
#include <QTextStream>
#include <algorithm>  // Include for std::sort

ReportGenerator::ReportGenerator(Scheduler* scheduler)
    : scheduler(scheduler) {}

QString ReportGenerator::generateReport(QDate from, QDate to) {
    QVector<Shift> shifts = scheduler->getShiftsInRange(from, to);

    // Sort shifts by start time
    std::sort(shifts.begin(), shifts.end(), [](const Shift& a, const Shift& b) {
        return a.startTime < b.startTime;
    });

    QString report;
    QTextStream stream(&report);

    QDate currentDate;
    for (const Shift& shift : shifts) {
        // Check if we are at a new date
        if (currentDate != shift.startTime.date()) {
            currentDate = shift.startTime.date();
            stream << "\nDate: " << currentDate.toString("yyyy-MM-dd") << "\n";
        }
        // Add shift details
        stream << "    Job Title: " << shift.jobTitle
               << ", Start: " << shift.startTime.toString("hh:mm AP")
               << ", End: " << shift.endTime.toString("hh:mm AP") << "\n";
    }
    return report;
}
