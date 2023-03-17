#include "clearpassengersdialog.h"
#include "ui_clearpassengersdialog.h"

ClearPassengersDialog::ClearPassengersDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::ClearPassengersDialog) {

    ui->setupUi(this);


    connect(ui->pBtn_No, &QPushButton::clicked,
            this, &ClearPassengersDialog::noBtnClicked);

    connect(ui->pBtn_Yes, &QPushButton::clicked,
            this, &ClearPassengersDialog::yesBtnClicked);
}

ClearPassengersDialog::~ClearPassengersDialog() {
    delete ui;
}


void ClearPassengersDialog::noBtnClicked() {
    this->close();
}


void ClearPassengersDialog::yesBtnClicked() {
    signalClearPassengers();

    this->close();
}
