/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: Boggle.cpp
 * -------------------------
 * This program further illustrates all the functions that
 * Boggle class has, also includes the recursive call of
 * human and computer search.
*/

#include "Boggle.h"
#include "shuffle.h"
#include "random.h"
#include <cctype>
#include "grid.h"
#include "bogglegui.h"

// letters on all 6 sides of every cube
static string CUBES[16] = {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};
const int BOGGLE_SIZE = 16;
const int LEGAL_LENGTH = 4;
const int NUM_ROWS = 4;
const int NUM_COLS = 4;
const int DELAY = 100;

// letters on every cube in 5x5 "Big Boggle" version (extension)
static string BIG_BOGGLE_CUBES[25] = {
   "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
   "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
   "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
   "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
   "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

/* Function: Boggle
 * purpose: constructor of boggle class
*/
Boggle::Boggle(Lexicon& dictionary, string boardText) {
    lex = dictionary;
    Grid<char> boggleBoard(NUM_ROWS, NUM_COLS);
    if(boardText == ""){
        //Generates random grid
        shuffle(CUBES, BOGGLE_SIZE);
        for(int i = 0; i < BOGGLE_SIZE; i++){
            string element = CUBES[i];
            int eleIndex = randomInteger(0, element.length() - 1);
            char ch = element[eleIndex];
            boardText += ch;
        }
    }
    boardText = toUpperCase(boardText); //Case insensitive
    for(int i = 0; i < BOGGLE_SIZE; i++){
        boggleBoard[i / NUM_ROWS][i % NUM_COLS] = boardText[i];
    }
    board = boggleBoard;
}

/* Function: getLetter
 * purpose: get the letter in the certain row and col of the
 * boggle grid
*/
char Boggle::getLetter(int row, int col) {
    if(board.inBounds(row, col)){
        return board[row][col];
    }else{
        throw("Out of bounds.");
    }

}

/* Function: checkWord
 * purpose: check if the word put is valid
*/
bool Boggle::checkWord(string word) {
    if(word.length() >= LEGAL_LENGTH){
        if(lex.contains(word)){
            if(!humanWords.contains(word)){
                return true;
            }
        }
    }
    return false;
}

/* Function: humanWordSearch
 * purpose: check is the user input can be formed from the grid
*/
bool Boggle::humanWordSearch(string word) {
    char localCh = word[0];
    for(int r = 0; r < board.numRows(); r++){
        for(int c = 0; c < board.numCols(); c++){
            if(board[r][c] == localCh){
                board[r][c] = 'X';
                BoggleGUI::setAnimationDelay(DELAY);
                BoggleGUI::setHighlighted(r, c, true);
                if(humanWordSearchHelper(word.substr(1), r, c)){
                    board[r][c] = localCh;
                    return true;
                }
                board[r][c] = localCh;
                BoggleGUI::setAnimationDelay(DELAY);
                BoggleGUI::setHighlighted(r, c, false);
            }
        }
    }
    return false;
}

/* Function: getScoreHuman
 * purpose: get the human user score
*/
int Boggle::getScoreHuman() {
    // TODO: implement
    return humanScore;
}

/* Function: computerWordSearch
 * purpose: return all the valid remainig words that has not been found
 * by the human user
*/
Set<string> Boggle::computerWordSearch() {
    // TODO: implement
    Set<string> computerWords;
    for(int r = 0; r < board.numRows(); r++){
        for(int c = 0; c < board.numCols(); c++){
            string soFar = "";
            char localCh = board[r][c];
            soFar += board[r][c];
            board[r][c] = 'X';
            computerWords += computerWordSearchHelper(soFar, r, c);
            board[r][c] = localCh;
        }
    }
    return computerWords;
}

/* Function: getSComputerHuman
 * purpose: get the computer score
*/
int Boggle::getScoreComputer() {
    // TODO: implement
    Set<string> computerWords = computerWordSearch();
    for(string word: computerWords){
        int wordScore = 1 + word.length() - LEGAL_LENGTH;
        computerScore += wordScore;
    }
    return computerScore;   // remove this
}

ostream& operator<<(ostream& out, Boggle& boggle) {
    // TODO: implement
    Grid<char> toPrint = boggle.board;
    string toPrintStr;
    for(int r = 0; r < toPrint.numRows(); r++){
        for(int c = 0; c < toPrint.numCols(); c++){
            toPrintStr += boggle.board[r][c];
        }
        toPrintStr += '\n';
    }
    out << toPrintStr;
    return out;
}

/* Function: getBoggleBoard
 * purpose: return the boggle board
*/
Grid<char> Boggle::getBoggleBoard(){
    return board;
}

/* Function: countScore
 * purpose: count the score of each valid input word
*/
void Boggle::countScore(string word){
    int wordScore = 1 + word.length() - LEGAL_LENGTH;
    humanScore += wordScore;
}

/* Function: humanWordSearchHelper
 * purpose: recursive call, check is the user input can be formed from the grid
*/
bool Boggle::humanWordSearchHelper(string word, int row, int col){
    //basic case
    if(word == ""){
        return true;
    }
    //Recursive call
    for(int subRow = row - 1; subRow <= row + 1; subRow ++){
        for(int subCol = col - 1; subCol <= col + 1; subCol ++){
            if(board.inBounds(subRow, subCol) && board[subRow][subCol] != 'X'){
                char currCh = board[subRow][subCol];
                if(word[0] == currCh){
                    board[subRow][subCol] = 'X';
                    BoggleGUI::setAnimationDelay(DELAY);
                    BoggleGUI::setHighlighted(subRow, subCol, true);
                    if(humanWordSearchHelper(word.substr(1), subRow, subCol)){
                        board[subRow][subCol] = currCh;
                        return true;
                    }
                    board[subRow][subCol] = currCh;
                    BoggleGUI::setAnimationDelay(DELAY);
                    BoggleGUI::setHighlighted(subRow, subCol, false);
                }
            }
        }
    }
    return false;
}

/* Function: computerWordSearchHelper
 * purpose: recursive call, return all the valid remainig words that has not been found
 * by the human user
*/
Set<string> Boggle::computerWordSearchHelper(string soFar, int r, int c){
    Set<string> searchedWords;
    //Basic case
    if(!lex.containsPrefix(soFar)){
        return searchedWords;
    }
    if(lex.contains(soFar) && checkWord(soFar)){
        searchedWords.add(soFar);
    }
    //Recursive call
    for(int subRow = r - 1; subRow <= r + 1; subRow++){
        for(int subCol = c - 1; subCol <= c + 1; subCol++){
            if(board.inBounds(subRow, subCol) && board[subRow][subCol] != 'X'){
                char currCh = board[subRow][subCol];
                soFar += currCh;
                board[subRow][subCol] = 'X';
                searchedWords += computerWordSearchHelper(soFar, subRow, subCol);
                soFar.erase(soFar.length() - 1, 1);
                board[subRow][subCol] = currCh;
            }
        }
    }
    return searchedWords;
}

/* Function: getLex
 * purpose: return the lexicon
*/
Lexicon Boggle::getLex(){
    return lex;
}
