#ifndef ADDPASSENGERDIALOG_H
#define ADDPASSENGERDIALOG_H

#include <QDialog>
#include <QDate>
#include <QMessageBox>

namespace Ui {
class AddPassengerDialog;
}

class MainWindow;

class AddPassengerDialog : public QDialog {
    Q_OBJECT

public:
    explicit AddPassengerDialog(QWidget *parent = nullptr);
    ~AddPassengerDialog();

private:
    void setDefaultForm();

    void showErrorMessage(QString title, QString msg);

    QString validatesPassportNumber();

    QString validatedPassportPlace();

    QString validatedPassportDate();

    QString validatedBirthday();

    QString validateFirstName();

    QString validateLastName();

    QString validatePatronymic();

protected:
    void closeEvent(QCloseEvent *event) override;

public slots:
    void setValidatedPassportNumber(bool state);

private slots:
    void cancelBtnClicked();

    void addBtnClicked();

private:
    Ui::AddPassengerDialog *ui;

    bool _validatedPassportNumber;

    QString _defaultPassportNumberPart1;
    QString _defaultPassportNumberPart2;
    QString _defaultPassportPlace;

    QDate _defaultPassportDate;
    QDate _defaultBirthday;

    QDate _minimalDate;
    QDate _maximalDate;

    QString _defaultFirstName;
    QString _defaultLastName;
    QString _defaultPatronymic;

    QMessageBox *_errorMessage;

signals:
    void signalCheckValidatedPassportNumber(QString passportNumber);

    void signalAddPassenger(QString passportNumber, QString passportPlace,
                            QString passportDate, QString passportBirthday,
                            QString firstName, QString lastName, QString patronymic);
};

#endif // ADDPASSENGERDIALOG_H
