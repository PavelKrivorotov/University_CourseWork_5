#include "passengerlist.h"
#include <QDebug>

PassengerList::PassengerList() {
    _length = 0;
    _firstItem = nullptr;
    _lastItem = nullptr;
}


PassengerList::~PassengerList() {
    if (!_firstItem) { return; }

    PassengerItem *currentItem = _firstItem->getNextItem();
    while (currentItem != _firstItem) {
        PassengerItem *delItem = currentItem;
        currentItem = currentItem->getNextItem();
        delete delItem;
    }

    delete currentItem;
}


PassengerItem * PassengerList::getFirst() {
    return _firstItem;
}



PassengerItem *PassengerList::getLast() {
    return _lastItem;
}


PassengerItem * PassengerList::get(QString passporttNumber) {
    if (!_firstItem) { return nullptr; }
    if (_firstItem->getPassportNumber() == passporttNumber) { return _firstItem; }

    PassengerItem *currentItem = _firstItem->getNextItem();
    while (currentItem != _firstItem) {
        if (currentItem->getPassportNumber() == passporttNumber) { return currentItem; }
        currentItem = currentItem->getNextItem();
    }

    return nullptr;
}


void PassengerList::append(PassengerItem *passengerItem) {
    if (!_length) {
        passengerItem->setPreviousItem(passengerItem);
        passengerItem->setNextItem(passengerItem);

        _firstItem = passengerItem;
        _lastItem = passengerItem;
        _length += 1;
        return;
    }

    _lastItem->setNextItem(passengerItem);
    passengerItem->setPreviousItem(_lastItem);

    passengerItem->setNextItem(_firstItem);
    _firstItem->setPreviousItem(passengerItem);

    _lastItem = passengerItem;
    _length += 1;
}


void PassengerList::remove(QString passportNumber) {
    PassengerItem *removeItem = get(passportNumber);

    if (!removeItem) { return; }
    if ((removeItem == _firstItem) && (removeItem == _lastItem)) {
        _firstItem = nullptr;
        _lastItem = nullptr;
        _length = 0;

        qDebug() << "Complete";

        delete removeItem;
        return;
    }

    PassengerItem *previousItem = removeItem->getPreviousItem();
    PassengerItem *nextItem = removeItem->getNextItem();

    if (removeItem == _firstItem) { _firstItem = nextItem; }
    else if (removeItem == _lastItem) { _lastItem = previousItem; }

    if (previousItem == nextItem) {
        previousItem->setPreviousItem(previousItem);
        previousItem->setNextItem(previousItem);

        _firstItem = previousItem;
        _lastItem = previousItem;
    }

    else {
        previousItem->setNextItem(nextItem);
        nextItem->setPreviousItem(previousItem);
    }

    _length -= 1;
    delete removeItem;

    qDebug() << "Complete";
}


void PassengerList::show() {
    if (!_length) { return; }

    PassengerItem *currentItem = _firstItem;
    qDebug() << "Passport: " << currentItem->getPassportNumber()
             << ". Place: " << currentItem->getPassportPlace()
             << ". Date: " << currentItem->getPassportDate()
             << ". Birthday: " << currentItem->getPassportBirthday();

    currentItem = currentItem->getNextItem();
    while (currentItem != _firstItem) {
        qDebug() << "Passport: " << currentItem->getPassportNumber()
                 << ". Place: " << currentItem->getPassportPlace()
                 << ". Date: " << currentItem->getPassportDate()
                 << ". Birthday: " << currentItem->getPassportBirthday();

        currentItem = currentItem->getNextItem();
    }
}


bool PassengerList::isEmpty() {
    if (_firstItem) { return false; }
    return true;
}


int PassengerList::size() {
    return _length;
}
