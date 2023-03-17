#ifndef PASSENGERITEM_H
#define PASSENGERITEM_H

#include <QString>

class PassengerItem {

public:
    PassengerItem(QString passportNumber, QString passportPlace, QString passportDate,
                  QString passportBirthday, QString _firstName, QString _lastName,
                  QString _patronymic);
    virtual ~PassengerItem();

    PassengerItem * getPreviousItem();

    PassengerItem * getNextItem();

    QString getPassportNumber();

    QString getPassportPlace();

    QString getPassportDate();

    QString getPassportBirthday();

    QString getFirstName();

    QString getLastName();

    QString getPatronymic();

    void setPreviousItem(PassengerItem *previousItem);

    void setNextItem(PassengerItem *nextItem);

private:
    PassengerItem *_previousItem;
    PassengerItem *_nextItem;

    QString _passportNumber;
    QString _passportPlace;
    QString _passportDate;
    QString _passportBirthday;

    QString _firstName;
    QString _lastName;
    QString _patronymic;
};

#endif // PASSENGERITEM_H
