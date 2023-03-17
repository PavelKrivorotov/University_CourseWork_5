#include "deletepassengerdialog.h"
#include "ui_deletepassengerdialog.h"

DeletePassengerDialog::DeletePassengerDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::DeletePassengerDialog) {

    ui->setupUi(this);


    connect(ui->pBtn_Cancel, &QPushButton::clicked,
            this, &DeletePassengerDialog::cancelBtnClicked);

    connect(ui->pBtn_Delete, &QPushButton::clicked,
            this, &DeletePassengerDialog::deleteBtnClicked);
}

DeletePassengerDialog::~DeletePassengerDialog() {
    delete ui;
}


void DeletePassengerDialog::showEvent(QShowEvent *event) {
    QWidget::showEvent(event);

    signalFillPassportNumbers(ui->cB_PassportNumber);
}


void DeletePassengerDialog::closeEvent(QCloseEvent *event) {
    ui->cB_PassportNumber->clear();
    this->close();
}


void DeletePassengerDialog::cancelBtnClicked() {
    this->close();
}


void DeletePassengerDialog::deleteBtnClicked() {
    QString passportNumber = ui->cB_PassportNumber->currentText();

    signalDeletePassenger(passportNumber);

    this->close();
}
