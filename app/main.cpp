#include <QApplication>
#include "TMSApp.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TMSApp mainApp;
    mainApp.show();
    return app.exec();
}
