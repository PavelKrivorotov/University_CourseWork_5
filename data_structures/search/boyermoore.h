#ifndef BOYERMOORE_H
#define BOYERMOORE_H

#define NO_OF_CHARS 256

#include <QString>


class BoyerMoore {

public:
    BoyerMoore();
    virtual ~BoyerMoore();

    void badCharHeuristic(QString str, int size, int badchar[NO_OF_CHARS]);

    bool search(QString txt, QString pat);
};

#endif // BOYERMOORE_H
