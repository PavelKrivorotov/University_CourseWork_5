#ifndef SQLCONNECTER_H
#define SQLCONNECTER_H

#include <QObject>
#include <QTableWidget>
#include <QComboBox>
#include <QLineEdit>
#include <QSqlDatabase>

class HashTable;
class BinaryTree;
class TicketList;
class BoyerMoore;

class SQLConnecter : public QObject {
    Q_OBJECT

public:
    explicit SQLConnecter(QString pathToDatabase);
    ~SQLConnecter();

    void insertionToFlights();

    void fillFlightsTable(QTableWidget *tableWidget);


    void insertionToPassengers();

    void fillPassengersTable(QTableWidget *tableWidget);


    void insertionToTickets();

    void fillTicketsTable(QTableWidget *tableWidget);


    void fillFlightsByPassengerTable(QString passportNumber, QLineEdit *passportPLace,
                                     QLineEdit *passportDate, QLineEdit *firstName,
                                     QLineEdit *lastName, QLineEdit *patronymic,
                                     QLineEdit *birthday, QTableWidget *tableWidget);

    void fillPassengersByFlightTable(QString flightNumber, QLineEdit *company,
                                     QLineEdit *fromAitport, QLineEdit *toAirport,
                                     QLineEdit *fromDate, QLineEdit *toDate,
                                     QLineEdit *totalPlace, QLineEdit *freePlace,
                                     QTableWidget *tableWidget);

    void fillPassengersByFIO(QString fio, QTableWidget *tableWidget);

    void fillFlightsByToAirport(QString toAirport, QTableWidget *tableWidget);

private:
    void createTables();

public slots:
    void addFlightToDatabase(QString flightNumber, QString company, QString fromAirport,
                             QString toAirport, QString fromDate, QString toDate,
                             int totalPlace, int freePlace);

    void deleteFlightFromDatabase(QString flightNumber);

    void clearFlightsFromDatabase();


    void addPassengerToDatabase(QString passportNumber, QString passportPlace,
                                QString passportDate, QString passportBirthday,
                                QString firstName, QString lastName, QString patronymic);

    void deletePassengerFromDatabase(QString passportNumber);

    void clearPassengersFromDatabase();


    void addTicketToDatabase(QString passportNumber, QString flightNumber);

    void deleteTicketFromDatabase(QString ticketNumber);


    void fillPassportNumbersToForm(QComboBox *comboBox);

    void fillFlightNumbersToForm(QComboBox *comboBox, bool showAll=true);

    void fillTicketNumbersToForm(QComboBox *comboBox);


    void checkPassportNumber(QString passportNumber);

    void checkFlightNumber(QString flightNumber);

private:
    QSqlDatabase _db;

    HashTable *_hashTable;
    BinaryTree *_binaryTree;
    TicketList *_ticketList;
    BoyerMoore *_boyerMoore;

signals:
    void signalUpdateFlightsTable();

    void signalUpdatePassengersTable();

    void signalUpdateTicketTable();


    void signalValidPassportNumber(bool state);

    void signalValidFlightNumber(bool state);
};

#endif // SQLCONNECTER_H
