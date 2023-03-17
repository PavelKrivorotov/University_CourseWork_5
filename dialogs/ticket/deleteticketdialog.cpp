#include "deleteticketdialog.h"
#include "ui_deleteticketdialog.h"

DeleteTicketDialog::DeleteTicketDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::DeleteTicketDialog) {

    ui->setupUi(this);


    connect(ui->pBtn_Cancel, &QPushButton::clicked,
            this, &DeleteTicketDialog::cancelBtnClicked);

    connect(ui->pBtn_Return, &QPushButton::clicked,
            this, &DeleteTicketDialog::returnBtnClicked);
}


DeleteTicketDialog::~DeleteTicketDialog() {
    delete ui;
}


void DeleteTicketDialog::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    signalFillTicketNumbers(ui->cB_TicketNumber);
}


void DeleteTicketDialog::closeEvent(QCloseEvent *event) {
    ui->cB_TicketNumber->clear();

    this->close();
}


void DeleteTicketDialog::cancelBtnClicked() {
    this->close();
}


void DeleteTicketDialog::returnBtnClicked() {
//    Logic
    QString ticketNumber = ui->cB_TicketNumber->currentText();

    signalDeleteTicket(ticketNumber);

    this->close();
}
