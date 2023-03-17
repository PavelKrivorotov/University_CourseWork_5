#ifndef BINARYTREE_H
#define BINARYTREE_H

#include "node.h"

class BinaryTree {

public:
    BinaryTree();
    virtual ~BinaryTree();

    Node * getRoot();

    Node * get(QString flightNumber);

    void add(Node *newFlight);

    void remove(QString flightNumber);

    void showInformation(Node *rootNode);

private:
    int maximalHeight(Node *rootNode);

    void recursiveBalanceHeight(Node *currentNode, Node *rootNode);

    void setBalanceAdd(Node *newNode);

    void setBalanceRemove(Node * parentReplaceNode);

    Node * makeRotation(Node *rootNode);

    Node * leftRotation(Node *currentNode);

    Node * rightRotation(Node *currentNode);

    Node * bigLeftRotation(Node *currentNode);

    Node * bigRightRotation(Node *currentNode);

    Node * parentNode(QString flightNumber);

    Node * minimalNode(Node *rootNode);

    Node * maximalNode(Node *rootNode);

    void deleteBinaryTree(Node *rootNode);

private:
    Node *_root;
};

#endif // BINARYTREE_H
