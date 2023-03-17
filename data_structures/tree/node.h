#ifndef NODE_H
#define NODE_H

#include <QString>

class Node {

public:
    Node(QString flightNumber, QString company, QString fromAirport, QString toAirport,
         QString fromDate, QString toDate, int totalPlace, int freePlace);
    virtual ~Node();

    void setParent(Node *newParent);

    void setLeftChild(Node *newLeftChild);

    void setRightChild(Node *newRightChild);

    void setHeight(int newHeight);

    Node * getParent();

    Node * getLeftChild();

    Node * getRightChild();

    QString getFlightNumber();

    QString getCompany();

    QString getFromAirport();

    QString getToAirport();

    QString getFromDate();

    QString getToDate();

    int getTotalPlace();

    int getFreePlace();

    int getHeight();

private:
    Node *_parent;
    Node *_leftChild;
    Node *_rightChild;

    int _height;

    QString _flightNumber;
    QString _company;
    QString _fromAirport;
    QString _toAirport;
    QString _fromDate;
    QString _toDate;

    int _totalPlace;
    int _freePlace;
};

#endif // NODE_H
