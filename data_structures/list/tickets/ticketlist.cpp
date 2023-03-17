#include "ticketlist.h"
#include <QDebug>

TicketList::TicketList() {
    _length = 0;
    _firstItem = nullptr;
    _lastItem = nullptr;
}


TicketList::~TicketList() {
    if (!_firstItem) { return; }

    TicketItem *currentItem = _firstItem->getNextItem();
    while (currentItem != _firstItem) {
        TicketItem *delItem = currentItem;
        currentItem = currentItem->getNextItem();
        delete delItem;
    }

    delete currentItem;
}


TicketItem * TicketList::getFirst() {
    return _firstItem;
}


TicketItem * TicketList::getLast() {
    return _lastItem;
}


TicketItem * TicketList::get(QString ticketNumber) {
    if (!_firstItem) { return nullptr; }
    if (_firstItem->getTicketNumber() == ticketNumber) { return _firstItem; }

    TicketItem *currentItem = _firstItem->getNextItem();
    while (currentItem != _firstItem) {
        if (currentItem->getTicketNumber() == ticketNumber) { return currentItem; }
        currentItem = currentItem->getNextItem();
    }

    return nullptr;
}


QString TicketList::generateTicketNumber() {
    QString ticketNumber;
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString("-");
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);
//    ticketNumber += QString("-");
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);
    ticketNumber += QString::number(rand() % 10);

    return ticketNumber;
}


void TicketList::append(TicketItem *ticketItem) {
    if (!_length) {
        ticketItem->setPreviousItem(ticketItem);
        ticketItem->setNextItem(ticketItem);

        _firstItem = ticketItem;
        _lastItem = ticketItem;
        _length += 1;
        return;
    }

    _lastItem->setNextItem(ticketItem);
    ticketItem->setPreviousItem(_lastItem);

    ticketItem->setNextItem(_firstItem);
    _firstItem->setPreviousItem(ticketItem);

    _lastItem = ticketItem;
    _length += 1;

    sort();
}


void TicketList::remove(QString ticketNumber) {
    TicketItem *removeItem = get(ticketNumber);

    if (!removeItem) { return; }
    if ((removeItem == _firstItem) && (removeItem == _lastItem)) {
        _firstItem = nullptr;
        _lastItem = nullptr;
        _length = 0;

        qDebug() << "Complete";

        delete removeItem;
        return;
    }

    TicketItem *previousItem = removeItem->getPreviousItem();
    TicketItem *nextItem = removeItem->getNextItem();

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


void TicketList::show() {
    if (!_length) { return; }

    TicketItem *currentItem = _firstItem;
    qDebug() << "Ticket: " << currentItem->getTicketNumber()
             << ". Passport: " << currentItem->getPassportNumber()
             << ". Flight: " << currentItem->getFlightNumber();

    currentItem = currentItem->getNextItem();
    while (currentItem != _firstItem) {
        qDebug() << "Ticket: " << currentItem->getTicketNumber()
                 << ". Passport: " << currentItem->getPassportNumber()
                 << ". Flight: " << currentItem->getFlightNumber();

        currentItem = currentItem->getNextItem();
    }
}


//void TicketList::sort() {
//    TicketItem *currentItem = _firstItem;
//    if (!currentItem) { return; }

//    while (currentItem != _lastItem) {
//        TicketItem *nextItem = currentItem->getNextItem();

//        while(nextItem != _firstItem) {
//            if (nextItem->getTicketNumber() < currentItem->getTicketNumber()) {
//                if ((currentItem->getNextItem() == nextItem) &&
//                    (nextItem->getNextItem() == currentItem)) {

//                    _firstItem = nextItem;
//                    _lastItem = currentItem;
//                }

//                else if (currentItem->getNextItem() == nextItem) {
//                    TicketItem *previousCurrentItem = currentItem->getPreviousItem();
//                    TicketItem *nextNextItem = nextItem->getNextItem();

//                    previousCurrentItem->setNextItem(nextItem);
//                    nextItem->setPreviousItem(previousCurrentItem);

//                    nextItem->setNextItem(currentItem);
//                    currentItem->setPreviousItem(nextItem);

//                    currentItem->setNextItem(nextNextItem);
//                    nextNextItem->setPreviousItem(currentItem);

////                    if (currentItem == _firstItem) { _firstItem = nextItem; }
////                    if (nextItem == _lastItem) { _lastItem = currentItem; }
//                }

//                else {
//                    TicketItem *previousCurrentItem = currentItem->getPreviousItem();
//                    TicketItem *nextCurrentItem = currentItem->getNextItem();
//                    TicketItem *previousNextItem = nextItem->getPreviousItem();
//                    TicketItem *nextNextItem = nextItem->getNextItem();

//                    previousCurrentItem->setNextItem(nextItem);
//                    nextItem->setPreviousItem(previousCurrentItem);

//                    nextItem->setNextItem(nextCurrentItem);
//                    nextCurrentItem->setPreviousItem(nextItem);

//                    previousNextItem->setNextItem(currentItem);
//                    currentItem->setPreviousItem(previousNextItem);

//                    currentItem->setNextItem(nextNextItem);
//                    nextNextItem->setPreviousItem(currentItem);

////                    if (currentItem == _firstItem) { _firstItem = nextItem; }
////                    if (nextItem == _lastItem) { _lastItem = currentItem; }
//                }

//                TicketItem *tmp = currentItem;
//                currentItem = nextItem;
//                nextItem = tmp;

////                qDebug() << currentItem->getTicketNumber();
//            }

//            nextItem = nextItem->getNextItem();
//        }

//        currentItem = currentItem->getNextItem();
//    }
//}


void TicketList::sort() {
    TicketItem *currentItem = _firstItem;
    if (!currentItem) { return; }

    while (currentItem != _lastItem) {
        TicketItem *nextItem = currentItem->getNextItem();

        while(nextItem != _firstItem) {
//            int nextNum = nextItem->getTicketNumber().toInt();
//            int currentNum = currentItem->getTicketNumber().toInt();

            long nextNum = nextItem->getTicketNumber().toLong();
            long currentNum = currentItem->getTicketNumber().toLong();

            if (nextNum < currentNum) {
//                qDebug() << nextItem->getTicketNumber() << " < " << currentItem->getTicketNumber();

                if ((currentItem->getNextItem() == nextItem) &&
                    (nextItem->getNextItem() == currentItem)) {

//                    qDebug() << "\n2 Item in list: " << currentItem->getTicketNumber()
//                             << " and " << nextItem->getTicketNumber() << "\n";

                    _firstItem = nextItem;
                    _lastItem = currentItem;
                }

                else if ((currentItem->getNextItem() == nextItem) ||
                         (nextItem->getNextItem() == currentItem)) {

//                    qDebug() << "\nItem: " << currentItem->getTicketNumber()
//                             << " and next Item: " << nextItem->getTicketNumber() << "\n";

                    if (currentItem->getPreviousItem() == nextItem) {
                        TicketItem *nextCurrentItem = currentItem->getNextItem();
                        TicketItem *previousNextItem = nextItem->getPreviousItem();

                        nextItem->setPreviousItem(currentItem);
                        currentItem->setNextItem(nextItem);

                        nextItem->setNextItem(nextCurrentItem);
                        nextCurrentItem->setPreviousItem(nextItem);

                        previousNextItem->setNextItem(currentItem);
                        currentItem->setPreviousItem(previousNextItem);

                        if (currentItem == _firstItem) { _firstItem = nextItem; }
                        if (nextItem == _lastItem) { _lastItem = currentItem; }
                    }

                    else {
                        TicketItem *previousCurrentItem = currentItem->getPreviousItem();
                        TicketItem *nextNextItem = nextItem->getNextItem();

                        previousCurrentItem->setNextItem(nextItem);
                        nextItem->setPreviousItem(previousCurrentItem);

                        nextItem->setNextItem(currentItem);
                        currentItem->setPreviousItem(nextItem);

                        currentItem->setNextItem(nextNextItem);
                        nextNextItem->setPreviousItem(currentItem);

                        if (currentItem == _firstItem) { _firstItem = nextItem; }
                        if (nextItem == _lastItem) { _lastItem = currentItem; }
                    }
                }

                else {
//                    qDebug() << "\nItem: " << currentItem->getTicketNumber()
//                             << " and ... Item: " << nextItem->getTicketNumber() << "\n";

                    TicketItem *previousCurrentItem = currentItem->getPreviousItem();
                    TicketItem *nextCurrentItem = currentItem->getNextItem();
                    TicketItem *previousNextItem = nextItem->getPreviousItem();
                    TicketItem *nextNextItem = nextItem->getNextItem();

                    previousCurrentItem->setNextItem(nextItem);
                    nextItem->setPreviousItem(previousCurrentItem);

                    nextItem->setNextItem(nextCurrentItem);
                    nextCurrentItem->setPreviousItem(nextItem);

                    previousNextItem->setNextItem(currentItem);
                    currentItem->setPreviousItem(previousNextItem);

                    currentItem->setNextItem(nextNextItem);
                    nextNextItem->setPreviousItem(currentItem);

                    if (currentItem == _firstItem) { _firstItem = nextItem; }
                    if (nextItem == _lastItem) { _lastItem = currentItem; }
                }

                TicketItem *tmp = currentItem;
                currentItem = nextItem;
                nextItem = tmp;
            }

            nextItem = nextItem->getNextItem();
        }

        currentItem = currentItem->getNextItem();
    }
}


int TicketList::size() {
    return _length;
}


void TicketList::print() {
    TicketItem *currentItem = _firstItem;
    if (!currentItem) { return; }

    qDebug() << "\n Value: " << currentItem->getTicketNumber();

    currentItem = currentItem->getNextItem();
    while (currentItem != _firstItem) {
        qDebug() << " Value: " << currentItem->getTicketNumber();

        currentItem = currentItem->getNextItem();
    }

    qDebug() << "\n";
}


void TicketList::showDetailsInfo() {
    TicketItem *currentItem = _firstItem;
    if (!currentItem) { return; }

    qDebug() << " Value: " << currentItem->getTicketNumber() << "\n"
             << "Previous: " << currentItem->getPreviousItem()->getTicketNumber() << "\n"
             << "Next: " << currentItem->getNextItem()->getTicketNumber() << "\n";

    currentItem = currentItem->getNextItem();
    while (currentItem != _firstItem) {
        qDebug() << " Value: " << currentItem->getTicketNumber() << "\n"
                 << "Previous: " << currentItem->getPreviousItem()->getTicketNumber() << "\n"
                 << "Next: " << currentItem->getNextItem()->getTicketNumber() << "\n";

        currentItem = currentItem->getNextItem();
    }
}
