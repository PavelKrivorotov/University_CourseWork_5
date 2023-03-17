#ifndef DELETEFLIGHTDIALOG_H
#define DELETEFLIGHTDIALOG_H

#include <QDialog>
#include <QComboBox>

namespace Ui {
class DeleteFlightDialog;
}

class DeleteFlightDialog : public QDialog {
    Q_OBJECT

public:
    explicit DeleteFlightDialog(QWidget *parent = nullptr);
    ~DeleteFlightDialog();

public slots:
    void cancelBtnClicked();

    void deleteBtnClicked();

protected slots:
    void showEvent(QShowEvent *event) override;

    void closeEvent(QCloseEvent *event) override;

private:
    Ui::DeleteFlightDialog *ui;

signals:
    void signalFillFlightNumbers(QComboBox *comboBox, bool showAll=true);

    void signalDeleteFlight(QString flightNumber);
};

#endif // DELETEFLIGHTDIALOG_H
