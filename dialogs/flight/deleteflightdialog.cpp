#include "deleteflightdialog.h"
#include "ui_deleteflightdialog.h"

DeleteFlightDialog::DeleteFlightDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::DeleteFlightDialog) {

    ui->setupUi(this);


    connect(ui->pBtn_Cancel, &QPushButton::clicked,
            this, &DeleteFlightDialog::cancelBtnClicked);

    connect(ui->pBtn_Delete, &QPushButton::clicked,
            this, &DeleteFlightDialog::deleteBtnClicked);
}


DeleteFlightDialog::~DeleteFlightDialog() {
    delete ui;
}


void DeleteFlightDialog::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    signalFillFlightNumbers(ui->cB_FlightNumber, true);
}


void DeleteFlightDialog::closeEvent(QCloseEvent *event) {
    ui->cB_FlightNumber->clear();

    this->close();
}


void DeleteFlightDialog::cancelBtnClicked() {
    this->close();
}


void DeleteFlightDialog::deleteBtnClicked() {
    QString flightNumber = ui->cB_FlightNumber->currentText();

    signalDeleteFlight(flightNumber);

    this->close();
}
