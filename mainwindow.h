#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SQLConnecter;

class AddPassengerDialog;
class DeletePassengerDialog;
class ClearPassengersDialog;

class AddFlightDialog;
class DeleteFlightDialog;
class ClearFlightsDialog;

class AddTicketDialog;
class DeleteTicketDialog;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    SQLConnecter *getSQLConnecter();

private:
    void setPassengersHeaderLabels();

    void setFlightsHeaderLabels();

    void setTicketsHeaderLabels();

    void setFlightsByPassenger();

    void setFlightsByPassengerHeaderLabels();

    void setPassengersByFlights();

    void setPassengersByFlightsHeaderLabels();

    void setFlightsByToAirport();

    void setFlightsByToAirportHeaderLabels();

    void setPassengersByFIO();

    void setPassengersByFIOHeaderLabels();

public slots:
    void showRootWindow();

    void showPassengersTable();

    void showSelectByPassport();

    void showSelectByFIO();

    void showFlightsTable();

    void showSelectByFlight();

    void showSelectByToAirpotrt();

    void showTicketsTable();

private:
    Ui::MainWindow *ui;

    QLabel *_author;

    SQLConnecter *_sqlConnecter;

    AddPassengerDialog *_addPassengerDialog;
    DeletePassengerDialog *_deletePassengerDialog;
    ClearPassengersDialog *_clearPassengersDialog;

    AddFlightDialog *_addFlightDialog;
    DeleteFlightDialog *_deleteFlightDialog;
    ClearFlightsDialog *_clearFlightsDialog;

    AddTicketDialog *_addTicketDialog;
    DeleteTicketDialog *_deleteTicketDialog;
};
#endif // MAINWINDOW_H
