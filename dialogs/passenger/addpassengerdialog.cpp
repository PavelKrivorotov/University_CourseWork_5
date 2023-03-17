#include "addpassengerdialog.h"
#include "ui_addpassengerdialog.h"
#include <exception>

AddPassengerDialog::AddPassengerDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::AddPassengerDialog) {

    ui->setupUi(this);

    _errorMessage = new QMessageBox(this);

    _validatedPassportNumber = false;

    _defaultPassportNumberPart1 = "";
    _defaultPassportNumberPart2 = "";
    _defaultPassportPlace = "";

    _defaultPassportDate = QDate(2021, 10, 12);
    _defaultBirthday = QDate(2001, 9, 23);

    _minimalDate = QDate::currentDate().addYears(-100);
    _maximalDate = QDate::currentDate();

    _defaultFirstName = "";
    _defaultLastName = "";
    _defaultPatronymic = "";

    setDefaultForm();


    connect(ui->pB_Cancel, &QPushButton::clicked,
            this, &AddPassengerDialog::cancelBtnClicked);

    connect(ui->pB_Add, &QPushButton::clicked, this,
            &AddPassengerDialog::addBtnClicked);
}


AddPassengerDialog::~AddPassengerDialog() {
    delete ui;
    delete _errorMessage;
}


void AddPassengerDialog::closeEvent(QCloseEvent *event) {
    setDefaultForm();
    this->close();
}


void AddPassengerDialog::setDefaultForm() {
    ui->lE_PassportNumberPart1->setText(_defaultPassportNumberPart1);
    ui->lE_PassportNumberPart2->setText(_defaultPassportNumberPart2);
    ui->lE_PassportPlace->setText(_defaultPassportPlace);

    ui->dE_PassportDate->setDate(_defaultPassportDate);
    ui->dE_PassportDate->setMinimumDate(_minimalDate);
    ui->dE_PassportDate->setMaximumDate(_maximalDate);

    ui->dE_Birthday->setDate(_defaultBirthday);
    ui->dE_Birthday->setMinimumDate(_minimalDate);
    ui->dE_Birthday->setMaximumDate(_maximalDate);

    ui->lE_FirstName->setText(_defaultFirstName);
    ui->lE_LastName->setText(_defaultLastName);
    ui->lE_Patronymic->setText(_defaultPatronymic);
}


void AddPassengerDialog::showErrorMessage(QString title, QString msg) {
    _errorMessage->setWindowTitle(title);
    _errorMessage->setModal(true);
    _errorMessage->setText(msg);
    _errorMessage->setIcon(QMessageBox::Critical);
    _errorMessage->show();
}


void AddPassengerDialog::setValidatedPassportNumber(bool state) {
    _validatedPassportNumber = state;
}


QString AddPassengerDialog::validatesPassportNumber() {
    int char0 = (int)'0';
    int char9 = (int)'9';

    QString passportNumber1 = ui->lE_PassportNumberPart1->text();
    if (passportNumber1.isEmpty()) {
        throw std::invalid_argument("Серия паспорта должна состоять\n"
                                    "из 4-х цифр 0-9!");
    }

    for (int ind = 0; ind < passportNumber1.size(); ind++) {
        int currentChar = (int)passportNumber1[ind].toLatin1();

        if ((currentChar < char0) || (currentChar > char9)) {
            throw std::invalid_argument("Номер паспорта может содерджать\n"
                                        "только цифры 0-9!");
        }
    }

    QString passportNumber2 = ui->lE_PassportNumberPart2->text();
    if (passportNumber2.isEmpty()) {
        throw std::invalid_argument("Номер паспорта должна состоять\n"
                                    "из 6-и цифр 0-9!");
    }

    for (int ind = 0; ind < passportNumber2.size(); ind++) {
        int currentChar = (int)passportNumber2[ind].toLatin1();

        if ((currentChar < char0) || (currentChar > char9)) {
            throw std::invalid_argument("Номер паспорта может содерджать\n"
                                        "только цифры 0-9!");
        }
    }

    QString passportNumber = "";
    passportNumber += passportNumber1;
    passportNumber += "-";
    passportNumber += passportNumber2;

    signalCheckValidatedPassportNumber(passportNumber);

    if (!_validatedPassportNumber) {
        throw std::invalid_argument("Пассажир с таким номером паспорта\n"
                                    "уже зарегистрирован!");
    }

    return passportNumber;
}


QString AddPassengerDialog::validatedPassportPlace() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';
    int charSpace = (int)' ';
    int charHyphen = (int)'-';

    QString passportPlace = ui->lE_PassportPlace->text();
    if (passportPlace.isEmpty()) {
        throw std::invalid_argument("Место выдачи паспорта должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < passportPlace.size(); ind++) {
        int currentChar = (int)passportPlace[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz)) ||
            (currentChar == charSpace) ||
            (currentChar == charHyphen)) { }
        else {
            throw std::invalid_argument("Место выдачи паспорта может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z и пробел ' ',\n"
                                        "а также символ дефиса '-'!");
        }
    }

    return passportPlace;
}


QString AddPassengerDialog::validatedPassportDate() {
    QDate passportDate = ui->dE_PassportDate->date();
    QDate birthday = ui->dE_Birthday->date();

    if (passportDate < birthday.addYears(14)) {
        throw std::invalid_argument("Паспорт выдаётся только с 14 лет!");
    }

    return passportDate.toString("dd.MM.yyyy");
}


QString AddPassengerDialog::validatedBirthday() {
    QDate birthday = ui->dE_Birthday->date();
    return birthday.toString("dd.MM.yyyy");
}


QString AddPassengerDialog::validateFirstName() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';

    QString firstName = ui->lE_FirstName->text();
    if (firstName.isEmpty()) {
        throw std::invalid_argument("Имя пассажира должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < firstName.size(); ind++) {
        int currentChar = (int)firstName[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz))) { }
        else {
            throw std::invalid_argument("Имя пассажира может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z");
        }
    }

    return firstName;
}


QString AddPassengerDialog::validateLastName() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';

    QString lastName = ui->lE_LastName->text();
    if (lastName.isEmpty()) {
        throw std::invalid_argument("Фамилия пассажира должна\n"
                                    "быть заполнена!");
    }

    for (int ind = 0; ind < lastName.size(); ind++) {
        int currentChar = (int)lastName[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz))) { }
        else {
            throw std::invalid_argument("Фамилия пассажира может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z");
        }
    }

    return lastName;
}


QString AddPassengerDialog::validatePatronymic() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';

    QString patronymic = ui->lE_Patronymic->text();
    if (patronymic.isEmpty()) {
        throw std::invalid_argument("Отчество пассажира должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < patronymic.size(); ind++) {
        int currentChar = (int)patronymic[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz))) { }
        else {
            throw std::invalid_argument("Отчество пассажира может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z");
        }
    }

    return patronymic;
}


void AddPassengerDialog::cancelBtnClicked() {
    this->close();
}


void AddPassengerDialog::addBtnClicked() {
    try {
        QString passportNumber = validatesPassportNumber();
        QString passportPlace = validatedPassportPlace();
        QString passportDate = validatedPassportDate();
        QString birthday = validatedBirthday();
        QString lastName = validateLastName();
        QString firstName = validateFirstName();
        QString patronymic = validatePatronymic();

        signalAddPassenger(passportNumber, passportPlace, passportDate, birthday,
                           firstName, lastName, patronymic);
        this->close();
    }

    catch (std::exception &exc) {
        showErrorMessage("Error", exc.what());
    }
}
