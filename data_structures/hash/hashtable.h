#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <QString>
#include "data_structures/list/passengers/passengerlist.h"

class HashTable {

public:
    HashTable(int size = 2500);
    virtual ~HashTable();

    PassengerItem * get(QString keyPassportNumber);

    void add(PassengerItem *passengerItem);

    void remove(QString keyPassportNumber);

    PassengerList * at(int index);

    int size();

    void print();

    int hash(QString keyPassportNumber);

//    bool checkValidKey(QString keyPassportNumber);

private:
    int _sizeTable;

    int _sizeKey;
    int _hashConstant;

    PassengerList *_arrayOfItems;
};

#endif // HASHTABLE_H
