#include "ticketitem.h"

TicketItem::TicketItem(QString ticketNumber, QString passportNumber, QString flightNumber) {
    _previousItem = nullptr;
    _nextItem = nullptr;

    _ticketNumber = ticketNumber;
    _passportNumber = passportNumber;
    _flightNumber = flightNumber;
}


//TicketItem::TicketItem(QString passportNumber, QString flightNumber) {
//    _previousItem = nullptr;
//    _nextItem = nullptr;

//    _ticketNumber = generateTicketNumber();
//    _passportNumber = passportNumber;
//    _flightNumber = flightNumber;
//}


TicketItem::~TicketItem() {

}


TicketItem * TicketItem::getPreviousItem() {
    return _previousItem;
}


TicketItem * TicketItem::getNextItem() {
    return _nextItem;
}


QString TicketItem::getTicketNumber() {
    return _ticketNumber;
}


QString TicketItem::getPassportNumber() {
    return _passportNumber;
}


QString TicketItem::getFlightNumber() {
    return _flightNumber;
}


void TicketItem::setPreviousItem(TicketItem *previousItem) {
    _previousItem = previousItem;
}


void TicketItem::setNextItem(TicketItem *nextItem) {
    _nextItem = nextItem;
}


void TicketItem::setPassportNumber(QString newPassportNumber) {
    _passportNumber = newPassportNumber;
}


void TicketItem::setFlightNumber(QString newFlightNumber) {
    _flightNumber = newFlightNumber;
}


//QString TicketItem::generateTicketNumber() {
//    QString ticketNumber;
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString("-");
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString("-");
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString::number(rand() % 10);

//    return ticketNumber;
//}
