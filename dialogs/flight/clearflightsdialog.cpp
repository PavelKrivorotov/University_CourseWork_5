#include "clearflightsdialog.h"
#include "ui_clearflightsdialog.h"

ClearFlightsDialog::ClearFlightsDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::ClearFlightsDialog) {

    ui->setupUi(this);


    connect(ui->pBtn_No, &QPushButton::clicked,
            this, &ClearFlightsDialog::noBtnClicked);

    connect(ui->pBtn_Yse, &QPushButton::clicked,
            this, &ClearFlightsDialog::yesBtnClicked);
}


ClearFlightsDialog::~ClearFlightsDialog() {
    delete ui;
}


void ClearFlightsDialog::noBtnClicked() {
    this->close();
}


void ClearFlightsDialog::yesBtnClicked() {
    signalClearFlights();

    this->close();
}
