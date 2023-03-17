#include "sqlconnecter.h"

#include <QLabel>
#include <QSqlQuery>
#include <QSqlRecord>

#include "data_structures/hash/hashtable.h"
#include "data_structures/tree/binarytree.h"
#include "data_structures/list/tickets/ticketlist.h"
#include "data_structures/search/boyermoore.h"

SQLConnecter::SQLConnecter(QString pathToDatabase): QObject(nullptr) {
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(pathToDatabase);
    _db.open();
    createTables();

    _hashTable = nullptr;
    insertionToPassengers();

    _binaryTree = nullptr;
    insertionToFlights();

    _ticketList = nullptr;
    insertionToTickets();

    _boyerMoore = new BoyerMoore();
}


SQLConnecter::~SQLConnecter() {
    _db.close();

    delete _hashTable;
    delete _binaryTree;
    delete _ticketList;
    delete _boyerMoore;
}


void SQLConnecter::createTables() {
    _db.exec(
        "CREATE TABLE IF NOT EXISTS passengers ("
            "number TEXT PRIMARY KEY,"
            "place TEXT,"
            "date TEXT,"
            "birthday TEXT,"
            "firstName TEXT,"
            "lastName TEXT,"
            "patronymic TEXT"
        ")"
    );

    _db.exec(
        "CREATE TABLE IF NOT EXISTS flights ("
            "number TEXT PRIMARY KEY,"
            "company TEXT,"
            "fromAirport TEXT,"
            "toAirport TEXT,"
            "fromDate TEXT,"
            "toDate TEXT,"
            "totalPlace INTEGER,"
            "freePlace INTEGER"
        ")"
    );

    _db.exec(
        "CREATE TABLE IF NOT EXISTS tickets ("
            "number TEXT PRIMARY KEY,"
            "passengerNumber TEXT,"
            "flightNumber TEXT,"
            "FOREIGN KEY (passengerNumber) REFERENCES passengers(number),"
            "FOREIGN KEY (flightNumber) REFERENCES flights(number)"
        ")"
    );
}


void SQLConnecter::insertionToFlights() {
    delete _binaryTree;
    _binaryTree = new BinaryTree();

    QSqlQuery data = _db.exec(
        "SELECT number, company, fromAirport, toAirport, fromDate, toDate,"
                "totalPlace, freePlace "
        "FROM flights"
    );

    int numberId = data.record().indexOf("number");
    int companyId = data.record().indexOf("company");
    int fromAirportId = data.record().indexOf("fromAirport");
    int toAirportId = data.record().indexOf("toAirport");
    int fromDateId = data.record().indexOf("fromDate");
    int toDateId = data.record().indexOf("toDate");
    int totalPlaceId = data.record().indexOf("totalPlace");
    int freePlaceId = data.record().indexOf("freePlace");

    while (data.next()) {
        QString number = data.value(numberId).toString();
        QString company = data.value(companyId).toString();
        QString fromAirport = data.value(fromAirportId).toString();
        QString toAirport = data.value(toAirportId).toString();
        QString fromDate = data.value(fromDateId).toString();
        QString toDate = data.value(toDateId).toString();
        int totalPlace = data.value(totalPlaceId).toInt();
        int freePlace = data.value(freePlaceId).toInt();

        Node *flight = new Node(number, company, fromAirport, toAirport,
                                fromDate, toDate, totalPlace, freePlace);
        _binaryTree->add(flight);
    }

//    _binaryTree->showInformation(_binaryTree->getRoot());
}


void SQLConnecter::fillFlightsTable(QTableWidget *tableWidget) {
    QList<Node*> *queue1 = new QList<Node*>;
    QList<Node*> *queue2 = new QList<Node*>;

    QList<Node*> *tempQueue1;
    QList<Node*> *tempQueue2;

    Node *root = _binaryTree->getRoot();
    if (root) { queue1->append(root); }

    int rowCount = 0;
    while (queue1->size() || queue2->size()) {
        if (queue1->size()) { tempQueue1 = queue1; tempQueue2 = queue2; }
        else { tempQueue1 = queue2; tempQueue2 = queue1; }

        while(tempQueue1->size()) {
            Node *currentNode = tempQueue1->takeAt(0);
            Node *leftChild = currentNode->getLeftChild();
            Node *rightChild = currentNode->getRightChild();

            if (leftChild) { tempQueue2->append(leftChild); }
            if (rightChild) { tempQueue2->append(rightChild); }


            QLabel *lFlightNumber = new QLabel(tableWidget);
            lFlightNumber->setText(currentNode->getFlightNumber());
            lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 0, lFlightNumber);

            QLabel *lCompany = new QLabel(tableWidget);
            lCompany->setText(currentNode->getCompany());
            lCompany->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 1, lCompany);

            QLabel *lFromAirport = new QLabel(tableWidget);
            lFromAirport->setText(currentNode->getFromAirport());
            lFromAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 2, lFromAirport);

            QLabel *lToAirport = new QLabel(tableWidget);
            lToAirport->setText(currentNode->getToAirport());
            lToAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 3, lToAirport);

            QLabel *lFromDate = new QLabel(tableWidget);
            lFromDate->setText(currentNode->getFromDate());
            lFromDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 4, lFromDate);

            QLabel *lToDate = new QLabel(tableWidget);
            lToDate->setText(currentNode->getToDate());
            lToDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 5, lToDate);

            QLabel *lTotalPlace = new QLabel(tableWidget);
            lTotalPlace->setText(QString::number(currentNode->getTotalPlace()));
            lTotalPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 6, lTotalPlace);

            QLabel *lFreePlace = new QLabel(tableWidget);
            lFreePlace->setText(QString::number(currentNode->getFreePlace()));
            lFreePlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 7, lFreePlace);

            rowCount += 1;
        }
    }

    delete queue1;
    delete queue2;
}


void SQLConnecter::addFlightToDatabase(QString flightNumber, QString company, QString fromAirport,
                                  QString toAirport, QString fromDate, QString toDate,
                                  int totalPlace, int freePlace) {

    QSqlQuery query = QSqlQuery(_db);
    query.prepare(
        "INSERT INTO flights (number, company, fromAirport, toAirport,"
            "fromDate, toDate, totalPlace, freePlace) "
        "VALUES (:flightNumber, :company, :fromAirport, :toAirport,"
            ":fromDate, :toDate, :totalPlace, :freePlace)"
    );

    query.bindValue(":flightNumber", flightNumber);
    query.bindValue(":company", company);
    query.bindValue(":fromAirport", fromAirport);
    query.bindValue(":toAirport", toAirport);
    query.bindValue(":fromDate", fromDate);
    query.bindValue(":toDate", toDate);
    query.bindValue(":totalPlace", totalPlace);
    query.bindValue(":freePlace", freePlace);
    query.exec();

    insertionToFlights();
    signalUpdateFlightsTable();
}


void SQLConnecter::deleteFlightFromDatabase(QString flightNumber) {
    // Удаляем билеты, связанные с номером Авиарейса:

    QSqlQuery remove = QSqlQuery(_db);
    remove.prepare(
        "DELETE FROM tickets WHERE tickets.flightNumber = :flightNumber"
    );

    remove.bindValue(":flightNumber", flightNumber);
    remove.exec();


    // Удаляем информауию о Авиарейсе:

    QSqlQuery remove2 = QSqlQuery(_db);
    remove2.prepare(
        "DELETE FROM flights WHERE flights.number = :flightNumber"
    );

    remove2.bindValue(":flightNumber", flightNumber);
    remove2.exec();

    insertionToFlights();
    insertionToTickets();

    signalUpdateFlightsTable();
}


void SQLConnecter::clearFlightsFromDatabase() {
    // ...

    QSqlQuery remove = QSqlQuery(_db);
    remove.exec(
        "DELETE FROM tickets"
    );


    // ...

    QSqlQuery remove2 = QSqlQuery(_db);
    remove2.exec(
        "DELETE FROM flights"
    );

    insertionToFlights();
    insertionToTickets();

    signalUpdateFlightsTable();
}



void SQLConnecter::insertionToPassengers() {
    delete _hashTable;
    _hashTable = new HashTable;

    QSqlQuery data = _db.exec(
        "SELECT number, place, date, birthday, firstName, lastName, patronymic FROM passengers"
    );

    int numberId = data.record().indexOf("number");
    int placeId = data.record().indexOf("place");
    int dateId = data.record().indexOf("date");
    int birthdayId = data.record().indexOf("birthday");
    int firstNameId = data.record().indexOf("firstName");
    int lastNameId = data.record().indexOf("lastName");
    int patronymicId = data.record().indexOf("patronymic");

    while(data.next()) {
        QString number = data.value(numberId).toString();
        QString place = data.value(placeId).toString();
        QString date = data.value(dateId).toString();
        QString birthday = data.value(birthdayId).toString();
        QString firstName = data.value(firstNameId).toString();
        QString lastName = data.value(lastNameId).toString();
        QString patronymic = data.value(patronymicId).toString();

        PassengerItem *passengerItem = new PassengerItem(number, place, date, birthday,
                                                         firstName, lastName, patronymic);
        _hashTable->add(passengerItem);
    }
}


void SQLConnecter::fillPassengersTable(QTableWidget *tableWidget) {
    int rowCount = 0;
    for (int index = 0; index < _hashTable->size(); index++) {
        PassengerList *passengerList = _hashTable->at(index);

        if (!passengerList->isEmpty()) {
            PassengerItem *currentItem = passengerList->getFirst();

            QLabel *lPassportNumber = new QLabel(tableWidget);
            lPassportNumber->setText(currentItem->getPassportNumber());
            lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

            QLabel *lPassportPlace = new QLabel(tableWidget);
            lPassportPlace->setText(currentItem->getPassportPlace());
            lPassportPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 1, lPassportPlace);

            QLabel *lPassportDate = new QLabel(tableWidget);
            lPassportDate->setText(currentItem->getPassportDate());
            lPassportDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 2, lPassportDate);

            QLabel *lLastName = new QLabel(tableWidget);
            lLastName->setText(currentItem->getLastName());
            lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 3, lLastName);

            QLabel *lFirstName = new QLabel(tableWidget);
            lFirstName->setText(currentItem->getFirstName());
            lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 4, lFirstName);

            QLabel *lPatronymic = new QLabel(tableWidget);
            lPatronymic->setText(currentItem->getPatronymic());
            lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 5, lPatronymic);

            QLabel *lPassportBirthday = new QLabel(tableWidget);
            lPassportBirthday->setText(currentItem->getPassportBirthday());
            lPassportBirthday->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 6, lPassportBirthday);

            currentItem = currentItem->getNextItem();
            rowCount += 1;

            while (currentItem != passengerList->getFirst()) {
                QLabel *lPassportNumber = new QLabel(tableWidget);
                lPassportNumber->setText(currentItem->getPassportNumber());
                lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

                QLabel *lPassportPlace = new QLabel(tableWidget);
                lPassportPlace->setText(currentItem->getPassportPlace());
                lPassportPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 1, lPassportPlace);

                QLabel *lPassportDate = new QLabel(tableWidget);
                lPassportDate->setText(currentItem->getPassportDate());
                lPassportDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 2, lPassportDate);

                QLabel *lLastName = new QLabel(tableWidget);
                lLastName->setText(currentItem->getLastName());
                lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 3, lLastName);

                QLabel *lFirstName = new QLabel(tableWidget);
                lFirstName->setText(currentItem->getFirstName());
                lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 4, lFirstName);

                QLabel *lPatronymic = new QLabel(tableWidget);
                lPatronymic->setText(currentItem->getPatronymic());
                lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 5, lPatronymic);

                QLabel *lPassportBirthday = new QLabel(tableWidget);
                lPassportBirthday->setText(currentItem->getPassportBirthday());
                lPassportBirthday->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 6, lPassportBirthday);

                currentItem = currentItem->getNextItem();
                rowCount += 1;
            }
        }
    }
}


void SQLConnecter::addPassengerToDatabase(QString passportNumber, QString passportPlace,
                                          QString passportDate, QString passportBirthday,
                                          QString firstName, QString lastName,
                                          QString patronymic) {

    QSqlQuery query = QSqlQuery(_db);
    query.prepare(
        "INSERT INTO passengers (number, place, date, birthday,"
            "firstName, lastName, patronymic) "
        "VALUES (:passportNumber, :passportPlace, :passportDate, :passportBirthday,"
            ":firstName, :lastName, :patronymic)"
    );

    query.bindValue(":passportNumber", passportNumber);
    query.bindValue(":passportPlace", passportPlace);
    query.bindValue(":passportDate", passportDate);
    query.bindValue(":passportBirthday", passportBirthday);
    query.bindValue(":firstName", firstName);
    query.bindValue(":lastName", lastName);
    query.bindValue(":patronymic", patronymic);
    query.exec();

    insertionToPassengers();
    signalUpdatePassengersTable();
}


void SQLConnecter::deletePassengerFromDatabase(QString passportNumber) {
    // Находим все номера Авиарейсов на которые зарегистрирован данный Пассажир

    QSqlQuery get = QSqlQuery(_db);
    get.prepare(
        "SELECT flightNumber FROM tickets WHERE tickets.passengerNumber = :passportNumber"
    );

    get.bindValue(":passportNumber", passportNumber);
    get.exec();


    // Производим обновление свободных мест для каждого Авиарейса:

    QSqlQuery update = QSqlQuery(_db);
    update.prepare(
        "UPDATE flights SET freePlace = freePlace + 1 WHERE flights.number = :flightNumber"
    );

    while(get.next()) {
        int flightNumberId = get.record().indexOf("flightNumber");
        QString flightNumber = get.value(flightNumberId).toString();

        update.bindValue(":flightNumber", flightNumber);
        update.exec();
    }


    // Производим удаление Билетов, принадлежащих данному пассажиру:

    QSqlQuery remove = QSqlQuery(_db);
    remove.prepare(
        "DELETE FROM tickets WHERE tickets.passengerNumber = :passportNumber"
    );

    remove.bindValue(":passportNumber", passportNumber);
    remove.exec();


    // Удалям данного Пассажира:

    QSqlQuery remove2 = QSqlQuery(_db);
    remove2.prepare(
        "DELETE FROM passengers WHERE passengers.number = :passportNumber"
    );

    remove2.bindValue(":passportNumber", passportNumber);
    remove2.exec();

    insertionToPassengers();
    insertionToFlights();
    insertionToTickets();

    signalUpdatePassengersTable();
}


void SQLConnecter::clearPassengersFromDatabase() {
    // ...

    QSqlQuery remove = QSqlQuery(_db);
    remove.exec(
        "DELETE FROM tickets"
    );


    // ...

    QSqlQuery remove2 = QSqlQuery(_db);
    remove2.exec(
        "DELETE FROM passengers"
    );


    // ...

    QSqlQuery update = QSqlQuery(_db);
    update.exec(
        "UPDATE flights SET freePlace = totalPlace"
    );

    insertionToPassengers();
    insertionToFlights();
    insertionToTickets();

    signalUpdatePassengersTable();
}



void SQLConnecter::insertionToTickets() {
    delete _ticketList;
    _ticketList = new TicketList();

    QSqlQuery data = _db.exec(
        "SELECT number, passengerNumber, flightNumber FROM tickets"
    );

    int numberId = data.record().indexOf("number");
    int passengerNumberId = data.record().indexOf("passengerNumber");
    int flightNumberId = data.record().indexOf("flightNumber");

    while(data.next()) {
        QString number = data.value(numberId).toString();
        QString passengerNumber = data.value(passengerNumberId).toString();
        QString flightNumber = data.value(flightNumberId).toString();

        TicketItem *ticketItem = new TicketItem(number, passengerNumber, flightNumber);
        _ticketList->append(ticketItem);
    }
}


void SQLConnecter::fillTicketsTable(QTableWidget *tableWidget) {
    if (!_ticketList->size()) { return; }

    TicketItem *currentItem = _ticketList->getFirst();
    int rowCount = 0;

    QLabel *lTicketNumber = new QLabel(tableWidget);
    lTicketNumber->setText(currentItem->getTicketNumber());
    lTicketNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableWidget->setCellWidget(rowCount, 0, lTicketNumber);

    QLabel *lPassportNumber = new QLabel(tableWidget);
    lPassportNumber->setText(currentItem->getPassportNumber());
    lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableWidget->setCellWidget(rowCount, 1, lPassportNumber);

    QLabel *lFlightNumber = new QLabel(tableWidget);
    lFlightNumber->setText(currentItem->getFlightNumber());
    lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    tableWidget->setCellWidget(rowCount, 2, lFlightNumber);

    currentItem = currentItem->getNextItem();
    rowCount += 1;

    while (currentItem != _ticketList->getFirst()) {
        QLabel *lTicketNumber = new QLabel(tableWidget);
        lTicketNumber->setText(currentItem->getTicketNumber());
        lTicketNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 0, lTicketNumber);

        QLabel *lPassportNumber = new QLabel(tableWidget);
        lPassportNumber->setText(currentItem->getPassportNumber());
        lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 1, lPassportNumber);

        QLabel *lFlightNumber = new QLabel(tableWidget);
        lFlightNumber->setText(currentItem->getFlightNumber());
        lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 2, lFlightNumber);

        currentItem = currentItem->getNextItem();
        rowCount += 1;
    }
}


void SQLConnecter::addTicketToDatabase(QString passportNumber,
                                    QString flightNumber) {

    // Обновляем количесво свободных мест для переданного Авиарейса:

    QSqlQuery update = QSqlQuery(_db);
    update.prepare(
        "UPDATE flights SET freePlace = freePlace - 1 WHERE flights.number = :flightNumber"
    );

    update.bindValue(":flightNumber", flightNumber);
    update.exec();


    // Вставляем данные о новом билете:

    QString ticketNumber = _ticketList->generateTicketNumber();
    QSqlQuery query = QSqlQuery(_db);
    query.prepare(
        "INSERT INTO tickets (number, passengerNumber, flightNumber) "
        "VALUES (:number, :passengerNumber, :flightNumber)"
    );

    query.bindValue(":number", ticketNumber);
    query.bindValue(":passengerNumber", passportNumber);
    query.bindValue(":flightNumber", flightNumber);
    query.exec();

    insertionToFlights();
    insertionToTickets();

    signalUpdateTicketTable();
}


void SQLConnecter::deleteTicketFromDatabase(QString ticketNumber) {
    // Находим номер Авиварейса по номеру билета:

    QSqlQuery get = QSqlQuery(_db);
    get.prepare(
        "SELECT flightNumber FROM tickets WHERE tickets.number = :ticketNumber"
    );

    get.bindValue(":ticketNumber", ticketNumber);
    get.exec();

    get.first();
    int flightNumberId = get.record().indexOf("flightNumber");
    QString flightNumber = get.value(flightNumberId).toString();


    // Обновляем количество свободных мест (увеличиваем на 1):

    QSqlQuery update = QSqlQuery(_db);
    update.prepare(
        "UPDATE flights SET freePlace = freePlace + 1 WHERE flights.number = :flightNumber"
    );

    update.bindValue(":flightNumber", flightNumber);
    update.exec();


    // Удаляем Билет с переданным номером:

    QSqlQuery remove = QSqlQuery(_db);
    remove.prepare(
        "DELETE FROM tickets WHERE tickets.number = :ticketNumber"
    );

    remove.bindValue(":ticketNumber", ticketNumber);
    remove.exec();

    insertionToFlights();
    insertionToTickets();

    signalUpdateTicketTable();
}



void SQLConnecter::fillPassportNumbersToForm(QComboBox *comboBox) {
    comboBox->setPlaceholderText("Выберите номер паспорта");

    for (int index = 0; index < _hashTable->size(); index++) {
        PassengerList *passengerList = _hashTable->at(index);

        if (!passengerList->isEmpty()) {
            PassengerItem *currentItem = passengerList->getFirst();
            comboBox->addItem(currentItem->getPassportNumber());
            currentItem = currentItem->getNextItem();

            while (currentItem != passengerList->getFirst()) {
                comboBox->addItem(currentItem->getPassportNumber());
                currentItem = currentItem->getNextItem();
            }
        }
    }
}



void SQLConnecter::fillFlightNumbersToForm(QComboBox *comboBox, bool showAll) {
    QList<Node*> *queue1 = new QList<Node*>;
    QList<Node*> *queue2 = new QList<Node*>;

    QList<Node*> *tempQueue1;
    QList<Node*> *tempQueue2;

    Node *root = _binaryTree->getRoot();
    if (root) { queue1->append(root); }

    comboBox->setPlaceholderText("Выберите номер авиарейса");

    while (queue1->size() || queue2->size()) {
        if (queue1->size()) { tempQueue1 = queue1; tempQueue2 = queue2; }
        else { tempQueue1 = queue2; tempQueue2 = queue1; }

        while(tempQueue1->size()) {
            Node *currentNode = tempQueue1->takeAt(0);
            Node *leftChild = currentNode->getLeftChild();
            Node *rightChild = currentNode->getRightChild();

            if (leftChild) { tempQueue2->append(leftChild); }
            if (rightChild) { tempQueue2->append(rightChild); }

            if (!showAll) {
                if (currentNode->getFreePlace()) {
                    comboBox->addItem(currentNode->getFlightNumber());
                }
            }

            else {
                comboBox->addItem(currentNode->getFlightNumber());
            }
        }
    }

    delete queue1;
    delete queue2;
}


void SQLConnecter::fillTicketNumbersToForm(QComboBox *comboBox) {
    comboBox->setPlaceholderText("Выберите номер билета");

    TicketItem *currentItem = _ticketList->getFirst();
    if (!currentItem) { return; }

    comboBox->addItem(currentItem->getTicketNumber());
    currentItem = currentItem->getNextItem();

    while (currentItem != _ticketList->getFirst()) {
        comboBox->addItem(currentItem->getTicketNumber());
        currentItem = currentItem->getNextItem();
    }
}


void SQLConnecter::fillFlightsByPassengerTable(QString passportNumber, QLineEdit *passportPLace,
                                               QLineEdit *passportDate, QLineEdit *firstName,
                                               QLineEdit *lastName, QLineEdit *patronymic,
                                               QLineEdit *birthday, QTableWidget *tableWidget) {

    PassengerItem *passenger = _hashTable->get(passportNumber);
    passportPLace->setText(passenger->getPassportPlace());
    passportDate->setText(passenger->getPassportDate());
    firstName->setText(passenger->getFirstName());
    lastName->setText(passenger->getLastName());
    patronymic->setText(passenger->getPatronymic());
    birthday->setText(passenger->getPassportBirthday());


    TicketItem *currentItem = _ticketList->getFirst();
    if (!currentItem) { return; }

    QString flightNumber = currentItem->getFlightNumber();
    Node *flightNode = _binaryTree->get(flightNumber);
    int rowCount = 0;

    if (currentItem->getPassportNumber() == passportNumber &&
        currentItem->getFlightNumber() == flightNumber) {

        QLabel *lFlightNumber = new QLabel(tableWidget);
        lFlightNumber->setText(flightNumber);
        lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 0, lFlightNumber);

        QLabel *lFlightFromAirport = new QLabel(tableWidget);
        lFlightFromAirport->setText(flightNode->getFromAirport());
        lFlightFromAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 1, lFlightFromAirport);

        QLabel *lFlightToAirport = new QLabel(tableWidget);
        lFlightToAirport->setText(flightNode->getToAirport());
        lFlightToAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 2, lFlightToAirport);

        rowCount += 1;
    }

    currentItem = currentItem->getNextItem();


    while (currentItem != _ticketList->getFirst()) {
        QString flightNumber = currentItem->getFlightNumber();
        Node *flightNode = _binaryTree->get(flightNumber);

        if (currentItem->getPassportNumber() == passportNumber &&
            currentItem->getFlightNumber() == flightNumber) {

            QLabel *lFlightNumber = new QLabel(tableWidget);
            lFlightNumber->setText(flightNumber);
            lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 0, lFlightNumber);

            QLabel *lFlightFromAirport = new QLabel(tableWidget);
            lFlightFromAirport->setText(flightNode->getFromAirport());
            lFlightFromAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 1, lFlightFromAirport);

            QLabel *lFlightToAirport = new QLabel(tableWidget);
            lFlightToAirport->setText(flightNode->getToAirport());
            lFlightToAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 2, lFlightToAirport);

            rowCount += 1;
        }

        currentItem = currentItem->getNextItem();
    }
}


void SQLConnecter::fillPassengersByFlightTable(QString flightNumber, QLineEdit *company,
                                               QLineEdit *fromAitport, QLineEdit *toAirport,
                                               QLineEdit *fromDate, QLineEdit *toDate,
                                               QLineEdit *totalPlace, QLineEdit *freePlace,
                                               QTableWidget *tableWidget) {

    Node *flightNode = _binaryTree->get(flightNumber);
    company->setText(flightNode->getCompany());
    fromAitport->setText(flightNode->getFromAirport());
    toAirport->setText(flightNode->getToAirport());
    fromDate->setText(flightNode->getFromDate());
    toDate->setText(flightNode->getToDate());
    totalPlace->setText(QString::number(flightNode->getTotalPlace()));
    freePlace->setText(QString::number(flightNode->getFreePlace()));


    TicketItem *currentItem = _ticketList->getFirst();
    if (!currentItem) { return; }

    int rowCount = 0;

    if (currentItem->getFlightNumber() == flightNumber) {
        PassengerItem *passenger = _hashTable->get(currentItem->getPassportNumber());

        QLabel *lPassportNumber = new QLabel(tableWidget);
        lPassportNumber->setText(passenger->getPassportNumber());
        lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

        QLabel *lLastName = new QLabel(tableWidget);
        lLastName->setText(passenger->getLastName());
        lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 1, lLastName);

        QLabel *lFirstName = new QLabel(tableWidget);
        lFirstName->setText(passenger->getFirstName());
        lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 2, lFirstName);

        QLabel *lPatronymic = new QLabel(tableWidget);
        lPatronymic->setText(passenger->getPatronymic());
        lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        tableWidget->setCellWidget(rowCount, 3, lPatronymic);

        rowCount += 1;
    }

    currentItem = currentItem->getNextItem();


    while (currentItem != _ticketList->getFirst()) {
        if (currentItem->getFlightNumber() == flightNumber) {
            PassengerItem *passenger = _hashTable->get(currentItem->getPassportNumber());

            QLabel *lPassportNumber = new QLabel(tableWidget);
            lPassportNumber->setText(passenger->getPassportNumber());
            lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

            QLabel *lLastName = new QLabel(tableWidget);
            lLastName->setText(passenger->getLastName());
            lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 1, lLastName);

            QLabel *lFirstName = new QLabel(tableWidget);
            lFirstName->setText(passenger->getFirstName());
            lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 2, lFirstName);

            QLabel *lPatronymic = new QLabel(tableWidget);
            lPatronymic->setText(passenger->getPatronymic());
            lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
            tableWidget->setCellWidget(rowCount, 3, lPatronymic);

            rowCount += 1;
        }

        currentItem = currentItem->getNextItem();
    }
}


void SQLConnecter::fillPassengersByFIO(QString fio, QTableWidget *tableWidget) {
//    qDebug() << "Input FIO: " << fio;


    int rowCount = 0;
    for (int index = 0; index < _hashTable->size(); index++) {
        PassengerList *passengerList = _hashTable->at(index);

        if (!passengerList->isEmpty()) {
            PassengerItem *currentItem = passengerList->getFirst();
            QString fioFull = currentItem->getLastName() +
                              " " +
                              currentItem->getFirstName() +
                              " " +
                              currentItem->getPatronymic();

            if (_boyerMoore->search(fioFull, fio)) {
                QLabel *lPassportNumber = new QLabel(tableWidget);
                lPassportNumber->setText(currentItem->getPassportNumber());
                lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

                QLabel *lPassportPlace = new QLabel(tableWidget);
                lPassportPlace->setText(currentItem->getPassportPlace());
                lPassportPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 1, lPassportPlace);

                QLabel *lPassportDate = new QLabel(tableWidget);
                lPassportDate->setText(currentItem->getPassportDate());
                lPassportDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 2, lPassportDate);

                QLabel *lLastName = new QLabel(tableWidget);
                lLastName->setText(currentItem->getLastName());
                lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 3, lLastName);

                QLabel *lFirstName = new QLabel(tableWidget);
                lFirstName->setText(currentItem->getFirstName());
                lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 4, lFirstName);

                QLabel *lPatronymic = new QLabel(tableWidget);
                lPatronymic->setText(currentItem->getPatronymic());
                lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 5, lPatronymic);

                QLabel *lPassportBirthday = new QLabel(tableWidget);
                lPassportBirthday->setText(currentItem->getPassportBirthday());
                lPassportBirthday->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 6, lPassportBirthday);

                rowCount += 1;
            }

            currentItem = currentItem->getNextItem();


            while (currentItem != passengerList->getFirst()) {
                QString fioFull = currentItem->getLastName() +
                                  " " +
                                  currentItem->getFirstName() +
                                  " " +
                                  currentItem->getPatronymic();

                if (_boyerMoore->search(fioFull, fio)) {
                    QLabel *lPassportNumber = new QLabel(tableWidget);
                    lPassportNumber->setText(currentItem->getPassportNumber());
                    lPassportNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 0, lPassportNumber);

                    QLabel *lPassportPlace = new QLabel(tableWidget);
                    lPassportPlace->setText(currentItem->getPassportPlace());
                    lPassportPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 1, lPassportPlace);

                    QLabel *lPassportDate = new QLabel(tableWidget);
                    lPassportDate->setText(currentItem->getPassportDate());
                    lPassportDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 2, lPassportDate);

                    QLabel *lLastName = new QLabel(tableWidget);
                    lLastName->setText(currentItem->getLastName());
                    lLastName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 3, lLastName);

                    QLabel *lFirstName = new QLabel(tableWidget);
                    lFirstName->setText(currentItem->getFirstName());
                    lFirstName->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 4, lFirstName);

                    QLabel *lPatronymic = new QLabel(tableWidget);
                    lPatronymic->setText(currentItem->getPatronymic());
                    lPatronymic->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 5, lPatronymic);

                    QLabel *lPassportBirthday = new QLabel(tableWidget);
                    lPassportBirthday->setText(currentItem->getPassportBirthday());
                    lPassportBirthday->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                    tableWidget->setCellWidget(rowCount, 6, lPassportBirthday);

                    rowCount += 1;
                }

                currentItem = currentItem->getNextItem();
            }
        }
    }
}


void SQLConnecter::fillFlightsByToAirport(QString toAirport, QTableWidget *tableWidget) {
    qDebug() << "Input toAirport: " << toAirport;


    QList<Node*> *queue1 = new QList<Node*>;
    QList<Node*> *queue2 = new QList<Node*>;

    QList<Node*> *tempQueue1;
    QList<Node*> *tempQueue2;

    Node *root = _binaryTree->getRoot();
    if (root) { queue1->append(root); }

    int rowCount = 0;
    while (queue1->size() || queue2->size()) {
        if (queue1->size()) { tempQueue1 = queue1; tempQueue2 = queue2; }
        else { tempQueue1 = queue2; tempQueue2 = queue1; }

        while(tempQueue1->size()) {
            Node *currentNode = tempQueue1->takeAt(0);
            Node *leftChild = currentNode->getLeftChild();
            Node *rightChild = currentNode->getRightChild();

            if (leftChild) { tempQueue2->append(leftChild); }
            if (rightChild) { tempQueue2->append(rightChild); }

            QString toAirportFull = currentNode->getToAirport();
            if (_boyerMoore->search(toAirportFull, toAirport)) {
                QLabel *lFlightNumber = new QLabel(tableWidget);
                lFlightNumber->setText(currentNode->getFlightNumber());
                lFlightNumber->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 0, lFlightNumber);

                QLabel *lCompany = new QLabel(tableWidget);
                lCompany->setText(currentNode->getCompany());
                lCompany->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 1, lCompany);

                QLabel *lFromAirport = new QLabel(tableWidget);
                lFromAirport->setText(currentNode->getFromAirport());
                lFromAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 2, lFromAirport);

                QLabel *lToAirport = new QLabel(tableWidget);
                lToAirport->setText(currentNode->getToAirport());
                lToAirport->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 3, lToAirport);

                QLabel *lFromDate = new QLabel(tableWidget);
                lFromDate->setText(currentNode->getFromDate());
                lFromDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 4, lFromDate);

                QLabel *lToDate = new QLabel(tableWidget);
                lToDate->setText(currentNode->getToDate());
                lToDate->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 5, lToDate);

                QLabel *lTotalPlace = new QLabel(tableWidget);
                lTotalPlace->setText(QString::number(currentNode->getTotalPlace()));
                lTotalPlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 6, lTotalPlace);

                QLabel *lFreePlace = new QLabel(tableWidget);
                lFreePlace->setText(QString::number(currentNode->getFreePlace()));
                lFreePlace->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
                tableWidget->setCellWidget(rowCount, 7, lFreePlace);

                rowCount += 1;
            }
        }
    }

    delete queue1;
    delete queue2;
}


void SQLConnecter::checkPassportNumber(QString passportNumber) {
    if (!_hashTable->get(passportNumber)) {
        signalValidPassportNumber(true);
        return;
    }

    signalValidPassportNumber(false);
}


void SQLConnecter::checkFlightNumber(QString flightNumber) {
    if (!_binaryTree->get(flightNumber)) {
        signalValidFlightNumber(true);
        return;
    }

    signalValidFlightNumber(false);
}
