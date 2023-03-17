#ifndef TICKETITEM_H
#define TICKETITEM_H

#include <QString>

class TicketItem {

public:
    TicketItem(QString ticketNumber, QString passportNumber, QString flightNumber);
//    TicketItem(QString passportNumber, QString flightNumber);
    virtual ~TicketItem();

    TicketItem * getPreviousItem();

    TicketItem * getNextItem();

    QString getTicketNumber();

    QString getPassportNumber();

    QString getFlightNumber();

    void setPreviousItem(TicketItem *previousItem);

    void setNextItem(TicketItem *nextItem);

    void setPassportNumber(QString newPassportNumber);

    void setFlightNumber(QString newFlightNumber);

private:
//    QString generateTicketNumber();

private:
    TicketItem *_previousItem;
    TicketItem *_nextItem;

    QString _ticketNumber;
    QString _passportNumber;
    QString _flightNumber;
};

#endif // TICKETITEM_H
