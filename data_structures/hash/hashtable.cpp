#include "hashtable.h"
#include <QDebug>

HashTable::HashTable(int size) {
    _sizeTable = size;

    _sizeKey = 11;
    _hashConstant = 25065;

    _arrayOfItems = new PassengerList[size];

    for (int index = 0; index < size; index++) {
        PassengerList currentItem = PassengerList();
        _arrayOfItems[index] = currentItem;;
    }
}


HashTable::~HashTable() {
    delete [] _arrayOfItems;
}


PassengerItem * HashTable::get(QString keyPassportNumber) {
    int index = hash(keyPassportNumber);

    PassengerList &passengerList = _arrayOfItems[index];
    PassengerItem *passengerItem = passengerList.get(keyPassportNumber);

    if (!passengerItem) { return nullptr; } // Make exception!
    return passengerItem;
}


void HashTable::add(PassengerItem *passengerItem) {
    QString keyPassportNumber = passengerItem->getPassportNumber();
    int index = hash(keyPassportNumber) % _sizeTable;

    PassengerList &currentItem = _arrayOfItems[index];

    if (currentItem.get(keyPassportNumber)) { return; } // Make exception
    currentItem.append(passengerItem);
}


void HashTable::remove(QString keyPassportNumber) {
    int index = hash(keyPassportNumber);

    PassengerList &passengerList = _arrayOfItems[index];
    PassengerItem *passengerItem = passengerList.get(keyPassportNumber);

    if (!passengerItem) { return; } // Make exception

    passengerList.remove(keyPassportNumber);
}


PassengerList * HashTable::at(int index) {
    return &_arrayOfItems[index];
}


int HashTable::size() {
    return _sizeTable;
}


void HashTable::print() {
    for (int index = 0; index < _sizeTable; index++) {
        PassengerList &passengerList = _arrayOfItems[index];

        if (!passengerList.isEmpty()) {
            qDebug() << "\nPassengerList at index: " << index << ":";
            passengerList.show();
        }
    }
}


int HashTable::hash(QString keyPassportNumber) {
    int hashSum = 0;
    for (int index = 0; index < keyPassportNumber.size(); index++) {
        char c = *keyPassportNumber.toStdString().c_str();
        hashSum += pow((int)c, 2);
    }

    return (hashSum - _hashConstant) % _sizeTable;
}


//bool HashTable::checkValidKey(QString keyPassportNumber) {
//    if (key.length() != SIZE_KEY) { return false; }

//    for (int index = 0; index < SIZE_KEY; index++) {
//        if ((index == 0) || (index == 5)) {
//            if (!(( (int)key.at(index) > 47 ) && ( (int)key.at(index) < 58 ))) { return false; }
//        }

//        else {
//            if (!(( (int)key.at(index) > 64 ) && ( (int)key.at(index) < 91 ))) { return false; }
//        }
//    }

//    return true;
//}
