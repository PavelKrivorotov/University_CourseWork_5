#ifndef ADDTICKETDIALOG_H
#define ADDTICKETDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class AddTicketDialog;
}

class AddTicketDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddTicketDialog(QWidget *parent = nullptr);
    ~AddTicketDialog();

public slots:
    void cancelBtnClicked();

    void addBtnClicked();

protected slots:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::AddTicketDialog *ui;

signals:
    void signalFillPassportNumbers(QComboBox *comboBox);

    void signalFillFlightNumbers(QComboBox *comboBox, bool showAll=false);

    void signalAddTicket(QString passportNumber, QString flightNumber);
};

#endif // ADDTICKETDIALOG_H
