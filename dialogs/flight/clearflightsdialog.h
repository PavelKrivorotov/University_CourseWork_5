#ifndef CLEARFLIGHTSDIALOG_H
#define CLEARFLIGHTSDIALOG_H

#include <QDialog>

namespace Ui {
class ClearFlightsDialog;
}

class ClearFlightsDialog : public QDialog {
    Q_OBJECT

public:
    explicit ClearFlightsDialog(QWidget *parent = nullptr);
    ~ClearFlightsDialog();

public slots:
    void noBtnClicked();

    void yesBtnClicked();

private:
    Ui::ClearFlightsDialog *ui;

signals:
    void signalClearFlights();
};

#endif // CLEARFLIGHTSDIALOG_H
