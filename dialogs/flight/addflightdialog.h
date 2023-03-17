#ifndef ADDFLIGHTDIALOG_H
#define ADDFLIGHTDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class AddFlightDialog;
}

class AddFlightDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddFlightDialog(QWidget *parent = nullptr);
    ~AddFlightDialog();

private:
    void setDefaultForm();

    void showErrorMessage(QString title, QString msg);

    QString validatedFlightNumber();

    QString validatedCompany();

    QString validatedFromAirport();

    QString validatedToAirport();

    QString validatedFromDate();

    QString validatedToDate();

    int validatedTotalPlace();

    int validatedFreePlace();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void setValidatedFlightNumber(bool state);

private slots:
    void cancelBtnClicked();

    void addBtnClicked();

private:
    Ui::AddFlightDialog *ui;

    bool _validatedFlightNumber;

    QString _defaultFlightNumberPart1;
    QString _defaultFlightNumberPart2;
    QString _defaultCompany;
    QString _defaultFromAirport;
    QString _defaultToAirport;

    QDate _defaultFromDate;
    QDate _defaultToDate;

    int _defaultPlace;

    QDate _minimalDate;
    QDate _maximalDate;

    QMessageBox *_errorMessage;

signals:
    void signalCheckValidatedFlightNumber(QString flightNumber);

    void signalAddFlight(QString flightNumber, QString company, QString fromAirport,
                         QString toAirport, QString fromDate, QString toDate,
                         int totalPlace, int freePlace);
};

#endif // ADDFLIGHTDIALOG_H
