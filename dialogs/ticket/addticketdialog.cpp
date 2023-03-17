#include "addticketdialog.h"
#include "ui_addticketdialog.h"

AddTicketDialog::AddTicketDialog(QWidget *parent): QDialog(parent), ui(new Ui::AddTicketDialog) {
    ui->setupUi(this);


    connect(ui->pBtn_Cancel, &QPushButton::clicked,
            this, &AddTicketDialog::cancelBtnClicked);

    connect(ui->pBtn_Add, &QPushButton::clicked,
            this, &AddTicketDialog::addBtnClicked);
}


AddTicketDialog::~AddTicketDialog() {
    delete ui;
}


void AddTicketDialog::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    signalFillPassportNumbers(ui->cB_PassportNumber);
    signalFillFlightNumbers(ui->cB_FlightNumber, false);
}


void AddTicketDialog::closeEvent(QCloseEvent *event) {
    ui->cB_PassportNumber->clear();
    ui->cB_FlightNumber->clear();
}


void AddTicketDialog::cancelBtnClicked() {
    this->close();
}


void AddTicketDialog::addBtnClicked() {
//    Logic
    QString passportNumber = ui->cB_PassportNumber->currentText();
    QString flightNumber = ui->cB_FlightNumber->currentText();

    signalAddTicket(passportNumber, flightNumber);

    this->close();
}
