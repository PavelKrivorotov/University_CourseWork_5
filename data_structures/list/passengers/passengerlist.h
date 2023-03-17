#ifndef PASSENGERLIST_H
#define PASSENGERLIST_H

#include "passengeritem.h"

class PassengerList {

public:
    PassengerList();
    virtual ~PassengerList();

    PassengerItem * getFirst();

    PassengerItem * getLast();

    PassengerItem * get(QString passportNumber);

    void append(PassengerItem *passengerItem);

    void remove(QString passportNumber);

    void show();

    bool isEmpty();

    int size();

private:
    int _length;
    PassengerItem *_firstItem;
    PassengerItem *_lastItem;
};

#endif // PASSENGERLIST_H
