#include "addflightdialog.h"
#include "ui_addflightdialog.h"
#include "exception"

AddFlightDialog::AddFlightDialog(QWidget *parent): QDialog(parent),
    ui(new Ui::AddFlightDialog) {

    ui->setupUi(this);

    _errorMessage = new QMessageBox();

    _validatedFlightNumber = false;

    _defaultFlightNumberPart1 = "";
    _defaultFlightNumberPart2 = "";
    _defaultCompany = "";
    _defaultFromAirport = "";
    _defaultToAirport = "";

    _defaultFromDate = QDate::currentDate();
    _defaultToDate = QDate::currentDate();

    _minimalDate = QDate::currentDate();
    _maximalDate = QDate::currentDate().addDays(10);

    _defaultPlace = 100;

    setDefaultForm();


    connect(ui->pBtn_Cancel, &QPushButton::clicked,
            this, &AddFlightDialog::cancelBtnClicked);

    connect(ui->pBtn_Add, &QPushButton::clicked,
            this, &AddFlightDialog::addBtnClicked);
}


AddFlightDialog::~AddFlightDialog() {
    delete ui;
    delete _errorMessage;
}


void AddFlightDialog::closeEvent(QCloseEvent *event) {
    setDefaultForm();
    this->close();
}


void AddFlightDialog::setDefaultForm() {
    ui->lE_FlightNumberPart1->setText(_defaultFlightNumberPart1);
    ui->lE_FlightNumberPart2->setText(_defaultFlightNumberPart2);
    ui->tE_Company->setText(_defaultCompany);
    ui->lE_FromAirport->setText(_defaultFromAirport);
    ui->lE_ToAirport->setText(_defaultToAirport);

    ui->dE_FromDate->setDate(_defaultFromDate);
    ui->dE_FromDate->setMinimumDate(_minimalDate);
    ui->dE_FromDate->setMaximumDate(_maximalDate);

    ui->dE_ToDate->setDate(_defaultToDate);
    ui->dE_ToDate->setMinimumDate(_minimalDate);
    ui->dE_ToDate->setMaximumDate(_maximalDate);

    ui->sB_TotalPlace->setValue(_defaultPlace);
    ui->sB_FreePlace->setValue(_defaultPlace);
}


void AddFlightDialog::showErrorMessage(QString title, QString msg) {
    _errorMessage->setWindowTitle(title);
    _errorMessage->setModal(true);
    _errorMessage->setText(msg);
    _errorMessage->setIcon(QMessageBox::Critical);
    _errorMessage->show();
}


void AddFlightDialog::setValidatedFlightNumber(bool state) {
    _validatedFlightNumber = state;
}


QString AddFlightDialog::validatedFlightNumber() {
    int charA = (int)'A';
    int charZ = (int)'Z';

    QString flightNumber1 = ui->lE_FlightNumberPart1->text();
    if (flightNumber1.isEmpty()) {
        throw std::invalid_argument("Серия авиарейса не должна быть пустой");
    }

    for (int ind = 0; ind < flightNumber1.size(); ind++) {
        int currentChar = (int)flightNumber1[ind].toLatin1();

        if ((currentChar >= charA) && (currentChar <= charZ)) { }
        else {
            throw std::invalid_argument("Серия авиарейса должна состоять\n"
                                        "из букв латинницы A-Z!");
        }
    }


    int char0 = (int)'0';
    int char9 = (int)'9';

    QString flightNumber2 = ui->lE_FlightNumberPart2->text();
    if (flightNumber2.isEmpty()) {
        throw std::invalid_argument("Номер авиарейса не должен быть пустой!");
    }

    for (int ind = 0; ind < flightNumber2.size(); ind++) {
        int currentChar = (int)flightNumber2[ind].toLatin1();

        if ((currentChar < char0) || (currentChar > char9)) {
            throw std::invalid_argument("Номер авиарейса может содержать\n"
                                        "только цифры 0-9!");
        }
    }

    QString flightNumber = "";
    flightNumber += flightNumber1;
    flightNumber += "-";
    flightNumber += flightNumber2;

    signalCheckValidatedFlightNumber(flightNumber);

    if (!_validatedFlightNumber) {
        throw std::invalid_argument("Авиарейс с таким номером\n"
                                    "уже существует!");
    }

    return flightNumber;
}


QString AddFlightDialog::validatedCompany() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';
    int charSpace = (int)' ';
    int charHyphen = (int)'-';

    QString company = ui->tE_Company->toPlainText();
    if (company.isEmpty()) {
        throw std::invalid_argument("Название авиакомпании должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < company.size(); ind++) {
        int currentChar = (int)company[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz)) ||
            (currentChar == charSpace) ||
            (currentChar == charHyphen)) { }
        else {
            throw std::invalid_argument("Название компании может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z и пробел ' ',\n"
                                        "а также символ дефиса '-'!");
        }
    }

    return company;
}


QString AddFlightDialog::validatedFromAirport() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';
    int charSpace = (int)' ';
    int charHyphen = (int)'-';

    QString fromAirport = ui->lE_FromAirport->text();
    if (fromAirport.isEmpty()) {
        throw std::invalid_argument("Название аэропорта отправления должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < fromAirport.size(); ind++) {
        int currentChar = (int)fromAirport[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz)) ||
            (currentChar == charSpace) ||
            (currentChar == charHyphen)) { }
        else {
            throw std::invalid_argument("Название аэропорта отправления может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z и пробел ' ',\n"
                                        "а также символ дефиса '-'!");
        }
    }

    return fromAirport;
}


QString AddFlightDialog::validatedToAirport() {
    int charA = (int)'A';
    int charZ = (int)'Z';
    int chara = (int)'a';
    int charz = (int)'z';
    int charSpace = (int)' ';
    int charHyphen = (int)'-';

    QString toAirport = ui->lE_ToAirport->text();
    if (toAirport.isEmpty()) {
        throw std::invalid_argument("Название аэропорта прибытия должно\n"
                                    "быть заполнено!");
    }

    for (int ind = 0; ind < toAirport.size(); ind++) {
        int currentChar = (int)toAirport[ind].toLatin1();

        if (((currentChar >= charA) && (currentChar <= charZ)) ||
            ((currentChar >= chara) && (currentChar <= charz)) ||
            (currentChar == charSpace) ||
            (currentChar == charHyphen)) { }
        else {
            throw std::invalid_argument("Название аэропорта прибытия может\n"
                                        "содержать только символы\n"
                                        "латинницы a-z, A-Z и пробел ' ',\n"
                                        "а также символ дефиса '-'!");
        }
    }

    return toAirport;
}


QString AddFlightDialog::validatedFromDate() {
    QDate fromDate = ui->dE_FromDate->date();
    QDate toDate = ui->dE_ToDate->date();

    if (fromDate > toDate) {
        throw std::invalid_argument("Дата прибытия не может быть\n"
                                    "меньше даты отправления!");
    }

    return fromDate.toString("dd.MM.yyyy");
}


QString AddFlightDialog::validatedToDate() {
    QDate fromDate = ui->dE_FromDate->date();
    QDate toDate = ui->dE_ToDate->date();

    if (fromDate > toDate) {
        throw std::invalid_argument("Дата прибытия не может быть\n"
                                    "меньше даты отправления!");
    }

    return toDate.toString("dd.MM.yyyy");
}


int AddFlightDialog::validatedTotalPlace() {
    int totalPlace = ui->sB_TotalPlace->value();
    int freePlace = ui->sB_FreePlace->value();

    if (freePlace > totalPlace) {
        throw std::invalid_argument("Количество свободных мест не может\n"
                                    "быть больше общего количества!");
    }

    return totalPlace;
}


int AddFlightDialog::validatedFreePlace() {
    int totalPlace = ui->sB_TotalPlace->value();
    int freePlace = ui->sB_FreePlace->value();

    if (freePlace > totalPlace) {
        throw std::invalid_argument("Количество свободных мест не может\n"
                                    "быть больше общего количества!");
    }

    return freePlace;
}


void AddFlightDialog::cancelBtnClicked() {
    this->close();
}


void AddFlightDialog::addBtnClicked() {
    try {
        QString flightNumber = validatedFlightNumber();
        QString company = validatedCompany();
        QString fromAirport = validatedFromAirport();
        QString toAirport = validatedToAirport();
        QString fromDate = validatedFromDate();
        QString toDate = validatedToDate();
        int totalPlace = validatedTotalPlace();
        int freePlace = validatedFreePlace();


        signalAddFlight(flightNumber, company, fromAirport, toAirport, fromDate, toDate,
                        totalPlace, freePlace);
        this->close();
    }

    catch (std::invalid_argument &exc) {
        showErrorMessage("Error", exc.what());
    }
}
