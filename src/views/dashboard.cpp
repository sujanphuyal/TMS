#include "Dashboard.h"

Dashboard::Dashboard(QWidget *parent) : QWidget(parent) {
    setupUI();
}

void Dashboard::setupUI() {
    auto *layout = new QVBoxLayout(this);

    auto *welcomeLabel = new QLabel("Welcome to Your Dashboard!", this);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(welcomeLabel);

    auto *infoLabel = new QLabel("Here you can see your upcoming shifts and hours.", this);
    infoLabel->setAlignment(Qt::AlignCenter);
    layout->addWidget(infoLabel);

    // This is a placeholder. You will add more UI elements like tables or charts here.
}
