#ifndef TICKETLIST_H
#define TICKETLIST_H

#include "ticketitem.h"

class TicketList {

public:
    TicketList();
    virtual ~TicketList();

    TicketItem * getFirst();

    TicketItem * getLast();

    TicketItem * get(QString ticketNumber);

    QString generateTicketNumber();

    void append(TicketItem *ticketItem);

    void remove(QString ticketNumber);

    void show();

    void sort();

    void print();

    void showDetailsInfo();

    int size();

private:
    int _length;
    TicketItem *_firstItem;
    TicketItem *_lastItem;
};

#endif // TICKETLIST_H
