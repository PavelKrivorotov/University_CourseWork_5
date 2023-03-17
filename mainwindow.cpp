#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDir>
#include <QSqlQuery>

#include "sql_connecter/sqlconnecter.h"

#include "dialogs/passenger/addpassengerdialog.h"
#include "dialogs/passenger/deletepassengerdialog.h"
#include "dialogs/passenger/clearpassengersdialog.h"

#include "dialogs/flight/addflightdialog.h"
#include "dialogs/flight/deleteflightdialog.h"
#include "dialogs/flight/clearflightsdialog.h"

#include "dialogs/ticket/addticketdialog.h"
#include "dialogs/ticket/deleteticketdialog.h"


MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    _author = new QLabel("PavelKrivorotov");
    _author->setFont(QFont("Segoe", 11, 1));
    _author->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->statusbar->addWidget(_author, 1);

    QString pathToDatabase = QDir::currentPath() + "/saod_course_work.sqlite";
    _sqlConnecter = new SQLConnecter(pathToDatabase);

    _addPassengerDialog = new AddPassengerDialog(this);
    _deletePassengerDialog = new DeletePassengerDialog(this);
    _clearPassengersDialog = new ClearPassengersDialog(this);

    _addFlightDialog = new AddFlightDialog(this);
    _deleteFlightDialog = new DeleteFlightDialog(this);
    _clearFlightsDialog = new ClearFlightsDialog(this);

    _addTicketDialog = new AddTicketDialog(this);
    _deleteTicketDialog = new DeleteTicketDialog(this);

    setPassengersHeaderLabels();
    setFlightsHeaderLabels();
    setTicketsHeaderLabels();



    // Подключаем демонстрацию окна-заставки (Главное окно):

    connect(ui->actionRoot, &QAction::triggered,
            this, &MainWindow::showRootWindow);



    // Подключение функционала для Пассажиров:

    connect(ui->actionShowAllPassengers, &QAction::triggered,
            this, &MainWindow::showPassengersTable);


    // Подкючаем добавение нового пассажира:

    connect(ui->actionAddPassenger, &QAction::triggered,
            _addPassengerDialog, &AddPassengerDialog::show);

    connect(_addPassengerDialog, &AddPassengerDialog::signalCheckValidatedPassportNumber,
            _sqlConnecter, &SQLConnecter::checkPassportNumber);

    connect(_sqlConnecter, &SQLConnecter::signalValidPassportNumber,
            _addPassengerDialog, &AddPassengerDialog::setValidatedPassportNumber);

    connect(_addPassengerDialog, &AddPassengerDialog::signalAddPassenger,
            _sqlConnecter, &SQLConnecter::addPassengerToDatabase);

    connect(_sqlConnecter, &SQLConnecter::signalUpdatePassengersTable,
            this, &MainWindow::showPassengersTable);


    // Подключаем удаление пассажира:

    connect(ui->actionDeletePassenger, &QAction::triggered,
            _deletePassengerDialog, &DeletePassengerDialog::show);

    connect(_deletePassengerDialog, &DeletePassengerDialog::signalFillPassportNumbers,
            _sqlConnecter, &SQLConnecter::fillPassportNumbersToForm);

    connect(_deletePassengerDialog, &DeletePassengerDialog::signalDeletePassenger,
            _sqlConnecter, &SQLConnecter::deletePassengerFromDatabase);


    // Подключам функционал для очищения данных о пассажирах:

    connect(ui->actionClearAllPassengers, &QAction::triggered,
           _clearPassengersDialog, &ClearPassengersDialog::show);

    connect(_clearPassengersDialog, &ClearPassengersDialog::signalClearPassengers,
            _sqlConnecter, &SQLConnecter::clearPassengersFromDatabase);


    // Подключаем функционал поиск по паспорту:

    connect(ui->actionSelectByPassport, &QAction::triggered,
            this, &MainWindow::showSelectByPassport);

    connect(ui->cB_PassportNumber, &QComboBox::textActivated,
           [this](QString passportNumber) {
                setFlightsByPassengerHeaderLabels();
                _sqlConnecter->fillFlightsByPassengerTable(passportNumber, ui->lE_PassportPlace,
                                                           ui->lE_PassportDate, ui->lE_FirstName,
                                                           ui->lE_LastName, ui->lE_Patronimic,
                                                           ui->lE_PassportBirthday,
                                                           ui->tW_FlightsByPassenger);
    });


    // Подключаем функционал для поиска по ФИО:

    connect(ui->actionSelectByFIO, &QAction::triggered,
            this, &MainWindow::showSelectByFIO);

    connect(ui->lE_FIO, &QLineEdit::textChanged,
            [this](QString fio){
                setPassengersByFIOHeaderLabels();
                _sqlConnecter->fillPassengersByFIO(fio, ui->tW_PassengersByFIO);
    });



    // Подключаем функционал для Авиарейсов:

    connect(ui->actionShowAllFlights, &QAction::triggered,
            this, &MainWindow::showFlightsTable);


    // Подключаем функционал для добавления нового авиарейса:

    connect (ui->actionAddFlight, &QAction::triggered,
             _addFlightDialog, &AddFlightDialog::show);

    connect(_addFlightDialog, &AddFlightDialog::signalCheckValidatedFlightNumber,
            _sqlConnecter, &SQLConnecter::checkFlightNumber);

    connect(_sqlConnecter, &SQLConnecter::signalValidFlightNumber,
            _addFlightDialog, &AddFlightDialog::setValidatedFlightNumber);

    connect(_addFlightDialog, &AddFlightDialog::signalAddFlight,
            _sqlConnecter, &SQLConnecter::addFlightToDatabase);

    connect(_sqlConnecter, &SQLConnecter::signalUpdateFlightsTable,
            this, &MainWindow::showFlightsTable);


    // Подключаем функционал для удаления авиарейса:

    connect(ui->actionDeleteFlight, &QAction::triggered,
            _deleteFlightDialog, &DeleteFlightDialog::show);

    connect(_deleteFlightDialog, &DeleteFlightDialog::signalFillFlightNumbers,
            _sqlConnecter, &SQLConnecter::fillFlightNumbersToForm);

    connect(_deleteFlightDialog, &DeleteFlightDialog::signalDeleteFlight,
            _sqlConnecter, &SQLConnecter::deleteFlightFromDatabase);


    // Подключам функционал для очищения данных о пассажирах:

    connect(ui->actionClearAllFlights, &QAction::triggered,
            _clearFlightsDialog, &ClearFlightsDialog::show);

    connect(_clearFlightsDialog, &ClearFlightsDialog::signalClearFlights,
            _sqlConnecter, &SQLConnecter::clearFlightsFromDatabase);


    // Подключаем функционал для поиска авиарейсов по номеру:

    connect(ui->actionSelectByFlight, &QAction::triggered,
            this, &MainWindow::showSelectByFlight);

    connect(ui->cB_FlightNumbers, &QComboBox::textActivated,
            [this](QString flightNumber){
                setPassengersByFlightsHeaderLabels();
                _sqlConnecter->fillPassengersByFlightTable(flightNumber,ui->lE_Company,
                                                           ui->lE_FromAirport, ui->lE_ToAirport,
                                                           ui->lE_FromDate, ui->lE_ToDate,
                                                           ui->lE_TotalPlace, ui->lE_FreePlace,
                                                           ui->tW_PassengersByFlight);
    });


    // Подключаем функционал для поиска по Аэропотру прибытия:

    connect(ui->actionSelectByToAirport, &QAction::triggered,
            this, &MainWindow::showSelectByToAirpotrt);

    connect(ui->lE_ToAirport_2, &QLineEdit::textChanged,
            [this](QString toAirport){
                setFlightsByToAirportHeaderLabels();
                _sqlConnecter->fillFlightsByToAirport(toAirport, ui->tW_FlightsByToAirport);
    });



    // Подключаем функционал для Билетов:

    connect(ui->actionShowAllTickets, &QAction::triggered,
            this, &MainWindow::showTicketsTable);



    // Подключаем функционал для покупки билетов:

    connect(ui->actionBuyTicket, &QAction::triggered,
            _addTicketDialog, &AddTicketDialog::show);

    connect(_addTicketDialog, &AddTicketDialog::signalFillPassportNumbers,
            _sqlConnecter, &SQLConnecter::fillPassportNumbersToForm);

    connect(_addTicketDialog, &AddTicketDialog::signalFillFlightNumbers,
            _sqlConnecter, &SQLConnecter::fillFlightNumbersToForm);

    connect(_addTicketDialog, &AddTicketDialog::signalAddTicket,
            _sqlConnecter, &SQLConnecter::addTicketToDatabase);

    connect(_sqlConnecter, &SQLConnecter::signalUpdateTicketTable,
            this, &MainWindow::showTicketsTable);



    // Подключаем функционал для возврата билетов:

    connect(ui->actionReturnTicket, &QAction::triggered,
            _deleteTicketDialog, &DeleteTicketDialog::show);

    connect(_deleteTicketDialog, &DeleteTicketDialog::signalFillTicketNumbers,
            _sqlConnecter, &SQLConnecter::fillTicketNumbersToForm);

    connect(_deleteTicketDialog, &DeleteTicketDialog::signalDeleteTicket,
            _sqlConnecter, &SQLConnecter::deleteTicketFromDatabase);
}


MainWindow::~MainWindow() {
    delete ui;

    delete _author;

    delete _sqlConnecter;

    delete _addPassengerDialog;
    delete _deletePassengerDialog;
    delete _clearPassengersDialog;

    delete _addFlightDialog;
    delete _deleteFlightDialog;
    delete _clearFlightsDialog;

    delete _addTicketDialog;
    delete _deleteTicketDialog;
}


SQLConnecter * MainWindow::getSQLConnecter() {
    return _sqlConnecter;
}


void MainWindow::setPassengersHeaderLabels() {
    QStringList passengerLabels;
    passengerLabels.append("Номер паспорта");
    passengerLabels.append("Место выдачи");
    passengerLabels.append("Дата выдачи");
    passengerLabels.append("Фамилия");
    passengerLabels.append("Имя");
    passengerLabels.append("Отчество");
    passengerLabels.append("Дата рождения");

    ui->tW_Passengers->setHorizontalHeaderLabels(passengerLabels);
}


void MainWindow::setFlightsHeaderLabels() {
    QStringList flightLabels;
    flightLabels.append("Номер авиарейса");
    flightLabels.append("Компания");
    flightLabels.append("Аэропорт отправления");
    flightLabels.append("Аэропорт прибытия");
    flightLabels.append("Дата отправления");
    flightLabels.append("Дата прибытия");
    flightLabels.append("Количество мест");
    flightLabels.append("Свободные места");

    ui->tW_Flights->setHorizontalHeaderLabels(flightLabels);
}


void MainWindow::setTicketsHeaderLabels() {
    QStringList ticketLabels;
    ticketLabels.append("Номер билета");
    ticketLabels.append("Номер паспорта");
    ticketLabels.append("Номер авиарейса");

    ui->tW_Tickets->setHorizontalHeaderLabels(ticketLabels);
}


void MainWindow::setFlightsByPassenger() {
    ui->cB_PassportNumber->clear();
    ui->lE_PassportPlace->clear();
    ui->lE_PassportDate->clear();
    ui->lE_FirstName->clear();
    ui->lE_LastName->clear();
    ui->lE_Patronimic->clear();
    ui->lE_PassportBirthday->clear();
}


void MainWindow::setFlightsByPassengerHeaderLabels() {
    ui->tW_FlightsByPassenger->clear();

    QStringList flightsByPassemger;
    flightsByPassemger.append("Номер авиарейса");
    flightsByPassemger.append("Аэропорт отправления");
    flightsByPassemger.append("Аэропорт прибытия");
    ui->tW_FlightsByPassenger->setHorizontalHeaderLabels(flightsByPassemger);
}



void MainWindow::setPassengersByFlights() {
    ui->cB_FlightNumbers->clear();
    ui->lE_Company->clear();
    ui->lE_FromAirport->clear();
    ui->lE_ToAirport->clear();
    ui->lE_FromDate->clear();
    ui->lE_ToDate->clear();
    ui->lE_TotalPlace->clear();
    ui->lE_FreePlace->clear();
}

void MainWindow::setPassengersByFlightsHeaderLabels() {
    ui->tW_PassengersByFlight->clear();

    QStringList passengerByFlights;
    passengerByFlights.append("Номер паспорта");
    passengerByFlights.append("Фамилия");
    passengerByFlights.append("Имя");
    passengerByFlights.append("Отчество");
    ui->tW_PassengersByFlight->setHorizontalHeaderLabels(passengerByFlights);
}


void MainWindow::setFlightsByToAirport() {
    ui->lE_ToAirport_2->clear();
}


void MainWindow::setFlightsByToAirportHeaderLabels() {
    ui->tW_FlightsByToAirport->clear();

    QStringList flightLabels;
    flightLabels.append("Номер авиарейса");
    flightLabels.append("Компания");
    flightLabels.append("Аэропорт отправления");
    flightLabels.append("Аэропорт прибытия");
    flightLabels.append("Дата отправления");
    flightLabels.append("Дата прибытия");
    flightLabels.append("Количество мест");
    flightLabels.append("Свободные места");

    ui->tW_FlightsByToAirport->setHorizontalHeaderLabels(flightLabels);
}


void MainWindow::setPassengersByFIO() {
    ui->lE_FIO->clear();
}


void MainWindow::setPassengersByFIOHeaderLabels() {
    ui->tW_PassengersByFIO->clear();

    QStringList passengerLabels;
    passengerLabels.append("Номер паспорта");
    passengerLabels.append("Место выдачи");
    passengerLabels.append("Дата выдачи");
    passengerLabels.append("Фамилия");
    passengerLabels.append("Имя");
    passengerLabels.append("Отчество");
    passengerLabels.append("Дата рождения");

    ui->tW_PassengersByFIO->setHorizontalHeaderLabels(passengerLabels);
}


void MainWindow::showRootWindow() {
    ui->stackedWidget->setCurrentWidget(ui->page0);
}


void MainWindow::showPassengersTable() {
    ui->tW_Passengers->clear();
    setPassengersHeaderLabels();

    _sqlConnecter->fillPassengersTable(ui->tW_Passengers);
    ui->stackedWidget->setCurrentWidget(ui->page1);
}


void MainWindow::showSelectByPassport() {
    setFlightsByPassenger();
    setFlightsByPassengerHeaderLabels();

    _sqlConnecter->fillPassportNumbersToForm(ui->cB_PassportNumber);
    ui->stackedWidget->setCurrentWidget(ui->page4);
}


void MainWindow::showSelectByFIO() {
    setPassengersByFIO();
    setPassengersByFIOHeaderLabels();

    ui->stackedWidget->setCurrentWidget(ui->page6);
}


void MainWindow::showFlightsTable() {
    ui->tW_Flights->clear();
    setFlightsHeaderLabels();

    _sqlConnecter->fillFlightsTable(ui->tW_Flights);
    ui->stackedWidget->setCurrentWidget(ui->page2);
}


void MainWindow::showSelectByFlight() {
    setPassengersByFlights();
    setPassengersByFlightsHeaderLabels();

    _sqlConnecter->fillFlightNumbersToForm(ui->cB_FlightNumbers);
    ui->stackedWidget->setCurrentWidget(ui->page5);
}


void MainWindow::showSelectByToAirpotrt() {
    setFlightsByToAirport();
    setFlightsByToAirportHeaderLabels();

    ui->stackedWidget->setCurrentWidget(ui->page7);
}


void MainWindow::showTicketsTable() {
    ui->tW_Tickets->clear();
    setTicketsHeaderLabels();

    _sqlConnecter->fillTicketsTable(ui->tW_Tickets);
    ui->stackedWidget->setCurrentWidget(ui->page3);
}
