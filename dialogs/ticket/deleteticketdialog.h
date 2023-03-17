#ifndef DELETETICKETDIALOG_H
#define DELETETICKETDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DeleteTicketDialog;
}

class DeleteTicketDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeleteTicketDialog(QWidget *parent = nullptr);
    ~DeleteTicketDialog();

public slots:
    void cancelBtnClicked();

    void returnBtnClicked();

protected slots:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DeleteTicketDialog *ui;

signals:
    void signalFillTicketNumbers(QComboBox *comboBox);

    void signalDeleteTicket(QString ticketNumber);
};

#endif // DELETETICKETDIALOG_H
