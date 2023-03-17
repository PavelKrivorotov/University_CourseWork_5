#ifndef DELETEPASSENGERDIALOG_H
#define DELETEPASSENGERDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DeletePassengerDialog;
}

class DeletePassengerDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeletePassengerDialog(QWidget *parent = nullptr);
    ~DeletePassengerDialog();

public slots:
    void cancelBtnClicked();

    void deleteBtnClicked();

protected slots:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DeletePassengerDialog *ui;

signals:
    void signalFillPassportNumbers(QComboBox *comboBox);

    void signalDeletePassenger(QString passportNumber);

};

#endif // DELETEPASSENGERDIALOG_H
