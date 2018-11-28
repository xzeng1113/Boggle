/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: Boggle.h
 * -------------------------
 * This is the header file of the boggle game
 * which defines the Boggle class, including all
 * the public and private function prototypes
 * and variables.
*/

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include "lexicon.h"
#include "grid.h"
using namespace std;

class Boggle {
public:
    Boggle(Lexicon& dictionary, string boardText = "");
    char getLetter(int row, int col);
    bool checkWord(string word);
    bool humanWordSearch(string word);
    Set<string> computerWordSearch();
    int getScoreHuman();
    int getScoreComputer();

    // Added member functions/variables
    friend ostream& operator<<(ostream& out, Boggle& boggle);
    Grid<char> getBoggleBoard();
    Set<string> humanWords;
    void countScore(string word);
    Lexicon getLex();


private:
    // Added other member functions/variables
    Grid<char> board;
    Lexicon lex;
    int humanScore = 0;
    int computerScore = 0;
    bool humanWordSearchHelper(string word, int row, int col);
    Set<string> computerWordSearchHelper(string soFar, int r, int c);
};

#endif // _boggle_h
