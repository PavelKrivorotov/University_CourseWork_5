#include "boyermoore.h"
#include <QDebug>

BoyerMoore::BoyerMoore() {

}


BoyerMoore::~BoyerMoore() {

}


// The preprocessing function for Boyer Moore's
// bad character heuristic
void BoyerMoore::badCharHeuristic(QString str, int size, int badchar[NO_OF_CHARS]) {
    int i;

    // Initialize all occurrences as -1
    for (i = 0; i < NO_OF_CHARS; i++)
        badchar[i] = -1;

    // Fill the actual value of last occurrence
    // of a character
    for (i = 0; i < size; i++)
        badchar[(int)str[i].toLatin1()] = i;
}


/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
bool BoyerMoore::search(QString txt, QString pat) {
    int m = pat.size();
    int n = txt.size();

    int badchar[NO_OF_CHARS];

    /* Fill the bad character array by calling
    the preprocessing function badCharHeuristic()
    for given pattern */
    badCharHeuristic(pat, m, badchar);

    int s = 0; // s is shift of the pattern with
                // respect to text
    while(s <= (n - m)) {
        int j = m - 1;

        /* Keep reducing index j of pattern while
        characters of pattern and text are
        matching at this shift s */
        while(j >= 0 && pat[j] == txt[s + j])
            j--;

        /* If the pattern is present at current
        shift, then index j will become -1 after
        the above loop */
        if (j < 0) {
//            qDebug() << "pattern occurs at shift = " <<  s;
            return true;

            /* Shift the pattern so that the next
            character in text aligns with the last
            occurrence of it in pattern.
            The condition s+m < n is necessary for
            the case when pattern occurs at the end
            of text */
            s += (s + m < n)? m - badchar[(int)txt[s + m].toLatin1()] : 1;

        }

        else
            /* Shift the pattern so that the bad character
            in text aligns with the last occurrence of
            it in pattern. The max function is used to
            make sure that we get a positive shift.
            We may get a negative shift if the last
            occurrence of bad character in pattern
            is on the right side of the current
            character. */
            if (1 > j - badchar[(int)txt[s + j].toLatin1()]) {
                s += 1;
            }

            else {
                s += j - badchar[(int)txt[s + j].toLatin1()];
            }
    }

    return false;
}
