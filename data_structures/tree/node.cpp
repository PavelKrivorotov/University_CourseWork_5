#include "node.h"

Node::Node(QString flightNumber, QString company, QString fromAirport, QString toAirport,
           QString fromDate, QString toDate, int totalPlace, int freePlace) {

    _parent = nullptr;

    _leftChild = nullptr;
    _rightChild = nullptr;

    _height = 0;

    _flightNumber = flightNumber;
    _company = company;
    _fromAirport = fromAirport;
    _toAirport = toAirport;
    _fromDate = fromDate;
    _toDate = toDate;
    _totalPlace = totalPlace;
    _freePlace = freePlace;
}


Node::~Node() {

}


void Node::setParent(Node *newParent) {
    _parent = newParent;
}


void Node::setLeftChild(Node *newLeftChild) {
    _leftChild = newLeftChild;
}


void Node::setRightChild(Node *newRightChild) {
    _rightChild = newRightChild;
}


void Node::setHeight(int newHeight) {
    _height = newHeight;
}


Node * Node::getParent() {
    return _parent;
}


Node * Node::getLeftChild() {
    return _leftChild;
}


Node * Node::getRightChild() {
    return _rightChild;
}


QString Node::getFlightNumber() {
    return _flightNumber;
}


QString Node::getCompany() {
    return _company;
}


QString Node::getFromAirport() {
    return _fromAirport;
}


QString Node::getToAirport() {
    return _toAirport;
}


QString Node::getFromDate() {
    return _fromDate;
}


QString Node::getToDate() {
    return _toDate;
}


int Node::getTotalPlace() {
    return _totalPlace;
}


int Node::getFreePlace() {
    return _freePlace;
}


int Node::getHeight() {
    return _height;
}
