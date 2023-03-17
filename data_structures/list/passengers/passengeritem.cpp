#include "passengeritem.h"
#include <QDebug>

//PassengerItem::PassengerItem(QString passportNumber, QString passportPlace,
//                             QString passportDate, QString passportBirthday) {
PassengerItem::PassengerItem(QString passportNumber, QString passportPlace,
                             QString passportDate, QString passportBirthday,
                             QString firstName, QString lastName, QString patronymic) {

    _previousItem = nullptr;
    _nextItem = nullptr;

    _passportNumber = passportNumber;
    _passportPlace = passportPlace;
    _passportDate = passportDate;
    _passportBirthday = passportBirthday;

    _firstName = firstName;
    _lastName = lastName;
    _patronymic = patronymic;
}


PassengerItem::~PassengerItem() {

}


PassengerItem * PassengerItem::getPreviousItem() {
    return _previousItem;
}


PassengerItem * PassengerItem::getNextItem() {
    return _nextItem;
}


QString PassengerItem::getPassportNumber() {
    return _passportNumber;
}


QString PassengerItem::getPassportPlace() {
    return _passportPlace;
}


QString PassengerItem::getPassportDate() {
    return _passportDate;
}


QString PassengerItem::getPassportBirthday() {
    return _passportBirthday;
}


QString PassengerItem::getFirstName() {
    return _firstName;
}


QString PassengerItem::getLastName() {
    return _lastName;
}


QString PassengerItem::getPatronymic() {
    return _patronymic;
}


void PassengerItem::setPreviousItem(PassengerItem *previousItem) {
    _previousItem = previousItem;
}


void PassengerItem::setNextItem(PassengerItem *nextItem) {
    _nextItem = nextItem;
}
