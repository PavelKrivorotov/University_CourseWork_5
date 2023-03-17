#ifndef CLEARPASSENGERSDIALOG_H
#define CLEARPASSENGERSDIALOG_H

#include <QDialog>

namespace Ui {
class ClearPassengersDialog;
}

class ClearPassengersDialog : public QDialog {
    Q_OBJECT

public:
    explicit ClearPassengersDialog(QWidget *parent = nullptr);
    ~ClearPassengersDialog();

public slots:
    void noBtnClicked();

    void yesBtnClicked();

private:
    Ui::ClearPassengersDialog *ui;

signals:
    void signalClearPassengers();
};

#endif // CLEARPASSENGERSDIALOG_H
