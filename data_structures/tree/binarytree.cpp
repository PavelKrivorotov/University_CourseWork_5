#include "binarytree.h"
#include <QDebug>

BinaryTree::BinaryTree() {
    _root = nullptr;
}


BinaryTree::~BinaryTree() {
    deleteBinaryTree(_root);
}


Node * BinaryTree::getRoot() {
    return _root;
}


Node * BinaryTree::get(QString flightNumber) {
    if (! _root) { return nullptr; }

    Node *currentNode = _root;
    while (currentNode) {
        QString currentValue = currentNode->getFlightNumber();

        if (flightNumber < currentValue) { currentNode = currentNode->getLeftChild(); }
        else if (flightNumber > currentValue) { currentNode = currentNode->getRightChild(); }
        else { return currentNode; }
    }

    return nullptr;
}


void BinaryTree::add(Node *newFlight) {
    QString flightNumber = newFlight->getFlightNumber();

    if (get(flightNumber)) { return; }

    Node *parent = parentNode(flightNumber);
    newFlight->setParent(parent);

    if (!parent) { _root = newFlight; }
    else if (flightNumber < parent->getFlightNumber()) { parent->setLeftChild(newFlight); }
    else { parent->setRightChild(newFlight); }

    setBalanceAdd(newFlight);

//    showInformation(_root);
}


void BinaryTree::remove(QString flightNumber) {
    Node *removeNode = get(flightNumber);
    if (!removeNode) { return; }

    Node *parentRemoveNode = parentNode(flightNumber);

    int state;
    if (!parentRemoveNode) { state = 0; }
    else if (removeNode == parentRemoveNode->getLeftChild()) { state = -1; }
    else { state = 1; }

    Node *replaceNodeLeft, *replaceNodeRight, *replaceNode;
    replaceNodeLeft = maximalNode(removeNode->getLeftChild());
    replaceNodeRight = removeNode->getRightChild();

    if (!replaceNodeLeft) { replaceNode = replaceNodeRight; }
    else { replaceNode = replaceNodeLeft; }

    if (!parentRemoveNode && !replaceNode) {
//        qDebug() << "Delete ONLY Root!";

        _root = nullptr;
    }

    else if (!replaceNode) {
//        qDebug() << "Delete least";

        int height = parentRemoveNode->getHeight();

        if (state == -1) {
            parentRemoveNode->setLeftChild(nullptr);
            parentRemoveNode->setHeight(height + 1);
        }

        else {
            parentRemoveNode->setRightChild(nullptr);
            parentRemoveNode->setHeight(height - 1);
        }

        setBalanceRemove(parentRemoveNode);
    }

    else {
//        qDebug() << "Delete NO least!";

        Node *parentReplaceNode = replaceNode->getParent();

        int newState;
        if (replaceNode == parentReplaceNode->getLeftChild()) { newState = -1; }
        else if (replaceNode == parentReplaceNode->getRightChild()) { newState = 1; }
        else { newState = 0; }


        replaceNode->setParent(parentRemoveNode);
        replaceNode->setHeight(removeNode->getHeight());

        if (parentReplaceNode == removeNode) {
//            qDebug() << "WORK FIRST!";

            int height = replaceNode->getHeight();

            if (newState == -1) {
                replaceNode->setHeight(height + 1);

                Node *parentRightChild = parentReplaceNode->getRightChild();
                replaceNode->setRightChild(parentRightChild);

                if (parentRightChild) { parentRightChild->setParent(replaceNode); }
            }

            else if (newState == 1) {
                replaceNode->setHeight(height - 1);

                Node *parentLeftChild = parentReplaceNode->getLeftChild();
                replaceNode->setLeftChild(parentLeftChild);

                if (parentLeftChild) { parentLeftChild->setParent(replaceNode); }
            }

            parentReplaceNode = replaceNode;
        }

        else {
//            qDebug() << "WORK STANDARD BLOK!";

            Node *removeLeftChild = removeNode->getLeftChild();
            Node *removeRightChild = removeNode->getRightChild();

            replaceNode->setLeftChild(removeLeftChild);
            replaceNode->setRightChild(removeRightChild);

            if (removeLeftChild) { removeLeftChild->setParent(replaceNode); }
            if (removeRightChild) { removeRightChild->setParent(replaceNode); }

            int height = parentReplaceNode->getHeight();

            if (newState == -1) {
                parentReplaceNode->setHeight(height + 1);
                parentReplaceNode->setLeftChild(nullptr);
            }

            else if (newState == 1) {
                parentReplaceNode->setHeight(height - 1);
                parentReplaceNode->setRightChild(nullptr);
            }
        }


        if (!state) { _root = replaceNode; }
        else if (state == -1) { parentRemoveNode->setLeftChild(replaceNode); }
        else if (state == 1) { parentRemoveNode->setRightChild(replaceNode); }

        setBalanceRemove(parentReplaceNode);
    }

    delete  removeNode;

//    showInformation(_root);
}


void BinaryTree::setBalanceAdd(Node * newNode) {
    Node *currentNode = newNode;

    while (currentNode) {
        Node *parent = currentNode->getParent();

        if (abs(currentNode->getHeight()) == 2) {
//            cout << "Make rotation by: " << currentNode->getValue() << " node." << endl;

            currentNode = makeRotation(currentNode);
        }

        if (!currentNode->getHeight()) {
            if (currentNode->getLeftChild() || currentNode->getRightChild()) { break; }
        }

        if (parent) {
            int heightParent = parent->getHeight();

            if (currentNode == parent->getLeftChild()) { parent->setHeight(heightParent - 1); }
            else { parent->setHeight(heightParent + 1); }
        }

        currentNode = parent;
    }
}


void BinaryTree::setBalanceRemove(Node *parentReplaceNode) {
    if (!parentReplaceNode) { return; }

    Node *currentNode = parentReplaceNode;

    while (currentNode) {
        Node *parentNode = currentNode->getParent();
        int height = currentNode->getHeight();

        if (abs(height) == 2) { currentNode = makeRotation(currentNode); }

        if (abs(currentNode->getHeight()) == 1) { break; }

        if (parentNode) {
            int parentHeight = parentNode->getHeight();

            if (currentNode == parentNode->getLeftChild()) { parentHeight = parentHeight + 1; }
            else { parentHeight = parentHeight - 1; }

            parentNode->setHeight(parentHeight);
        }

        currentNode = parentNode;
    }
}


Node * BinaryTree::makeRotation(Node *rootNode) {
    if (!rootNode) { return nullptr; }

    Node *aRoot, *bLeft, *bRight;
    Node *cLeft = nullptr, *cRight = nullptr;

    aRoot = rootNode;

    bLeft = aRoot->getLeftChild();
    bRight = aRoot->getRightChild();

    if (bRight) { cLeft = bRight->getLeftChild(); }
    if (bLeft) { cRight = bLeft->getRightChild(); }


    if (cLeft && bRight) {
        if ((aRoot->getHeight() == 2) && (bRight->getHeight() == -1)) {
            if ((cLeft->getHeight() == 0) || (cLeft->getHeight() == -1) || (cLeft->getHeight() == 1)) {
                Node *newRoot = bigLeftRotation(rootNode);
                return newRoot;
            }
        }
    }

    if (cRight && bLeft) {
        if ((aRoot->getHeight() == -2) && (bLeft->getHeight() == 1)) {
            if ((cRight->getHeight() == 0) || (cRight->getHeight() == -1) || (cRight->getHeight() == 1)) {
                Node *newRoot = bigRightRotation(rootNode);
                return newRoot;
            }
        }
    }

    if (bRight) {
        if (aRoot->getHeight() == 2) {
            if ((bRight->getHeight() == 0) || (bRight->getHeight() == 1)) {
                Node *newRoot = leftRotation(rootNode);
                return newRoot;
            }
        }
    }

    if (bLeft) {
        if (aRoot->getHeight() == -2) {
            if ((bLeft->getHeight() == 0) || (bLeft->getHeight() == -1)) {
                Node *newRoot = rightRotation(rootNode);
                return newRoot;
            }
        }
    }

    return nullptr;
}


Node * BinaryTree::leftRotation(Node *currentNode) {
    if (!currentNode) { return nullptr; }

    Node *parent = currentNode->getParent();

    int state;

    if (!parent) { state = 0; }
    else if (currentNode == parent->getLeftChild()) { state = -1; }
    else { state = 1; }

    if (!currentNode->getRightChild()) { return nullptr; }

    Node *newRoot, *newRootLeft;
    newRoot = currentNode->getRightChild();
    newRootLeft = newRoot->getLeftChild();

    currentNode->setRightChild(newRootLeft);
    if (newRootLeft) { newRootLeft->setParent(currentNode); }

    newRoot->setLeftChild(currentNode);
    currentNode->setParent(newRoot);

    newRoot->setParent(parent);


    if (!state) { _root = newRoot; }
    else if (state == -1) { parent->setLeftChild(newRoot); }
    else { parent->setRightChild(newRoot); }

    recursiveBalanceHeight(newRoot, newRoot);

    return newRoot;
}


Node * BinaryTree::rightRotation(Node *currentNode) {
    if (!currentNode) { return nullptr; }

    Node *parent = currentNode->getParent();

    int state;

    if (!parent) { state = 0; }
    else if (currentNode == parent->getLeftChild()) { state = -1; }
    else { state = 1; }

    if (!currentNode->getLeftChild()) { return nullptr; }

    Node *newRoot, *newRootRight;
    newRoot = currentNode->getLeftChild();
    newRootRight = newRoot->getRightChild();

    currentNode->setLeftChild(newRootRight);
    if (newRootRight) { newRootRight->setParent(currentNode); }

    newRoot->setRightChild(currentNode);
    currentNode->setParent(newRoot);

    newRoot->setParent(parent);

    if (!state) { _root = newRoot; }
    else if (state == -1) { parent->setLeftChild(newRoot); }
    else { parent->setRightChild(newRoot); }

    recursiveBalanceHeight(newRoot, newRoot);

    return newRoot;
}


Node * BinaryTree::bigLeftRotation(Node *currentNode) {
    rightRotation(currentNode->getRightChild());
    Node *newRoot = leftRotation(currentNode);

    return newRoot;
}


Node * BinaryTree::bigRightRotation(Node *currentNode) {
    leftRotation(currentNode->getLeftChild());
    Node *newRoot = rightRotation(currentNode);

    return newRoot;
}



void BinaryTree::showInformation(Node *rootNode){
    if (!rootNode) { return; }

    qDebug() << "Value: " << rootNode->getFlightNumber();

    if (rootNode->getParent()) { qDebug() << ". Parent: " << rootNode->getParent()->getFlightNumber(); }
    else { qDebug() << ". Parent: " << "NULL"; }

    if (rootNode->getLeftChild()) { qDebug() << ". LeftChild: " << rootNode->getLeftChild()->getFlightNumber(); }
    else { qDebug() << ". LeftChild: " << "NULL";}

    if (rootNode->getRightChild()) { qDebug() << ". RightChild: " << rootNode->getRightChild()->getFlightNumber(); }
    else { qDebug() << ". RightChild: " << "NULL"; }

    qDebug();

    showInformation(rootNode->getLeftChild());
    showInformation(rootNode->getRightChild());
}


Node * BinaryTree::parentNode(QString flightNumber) {
    Node *parent = nullptr;
    Node *child = _root;

    while (child) {
        QString currentValue = child->getFlightNumber();

        if (currentValue == flightNumber) { break; }

        parent = child;

        if (flightNumber < currentValue) { child = child->getLeftChild(); }
        else { child = child->getRightChild(); }
    }

    return parent;
}


Node * BinaryTree::minimalNode(Node *rootNode) {
    if (!rootNode) { return nullptr; }

    Node *parent = rootNode->getParent();
    Node *child = rootNode;

    while (child) {
        parent = child;
        child = child->getLeftChild();
    }

    return parent;
}


Node * BinaryTree::maximalNode(Node *rootNode) {
    if (!rootNode) { return nullptr; }

    Node *parent = rootNode->getParent();
    Node *child = rootNode;

    while (child) {
        parent = child;
        child = child->getRightChild();
    }

    return parent;
}


int BinaryTree::maximalHeight(Node *rootNode) {
    if (!rootNode) { return -1; }

    int maximalHeightLeft = maximalHeight(rootNode->getLeftChild()) + 1;
    int maximalHeightRight = maximalHeight(rootNode->getRightChild()) + 1;

    if (maximalHeightLeft > maximalHeightRight) { return maximalHeightLeft; }
    else { return maximalHeightRight; }
}


void BinaryTree::recursiveBalanceHeight(Node *currentNode, Node *rootNode) {
    if (!currentNode) { return; }

    int heightLeft = maximalHeight(currentNode->getLeftChild());
    int heightRight = maximalHeight(currentNode->getRightChild());

    currentNode->setHeight(heightRight - heightLeft);

    recursiveBalanceHeight(currentNode->getLeftChild(), rootNode);
    recursiveBalanceHeight(currentNode->getRightChild(), rootNode);
}


void BinaryTree::deleteBinaryTree(Node *rootNode) {
    if (! rootNode) { return; }

    deleteBinaryTree(rootNode->getLeftChild());
    deleteBinaryTree(rootNode->getRightChild());

    delete rootNode;
}
