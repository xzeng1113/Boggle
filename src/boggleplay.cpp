/* Name: Xiaoxi Zeng
 * Section Leader: Semir Shafi
 * File: boggleplay.cpp
 * -------------------------
 * This program executes the play of the
 * boggle game.
*/

#include "lexicon.h"
#include "Boggle.h"
#include "simpio.h"
#include "console.h"
#include <cctype>
#include "bogglegui.h"

const int LEGAL_LENGTH = 16;

/*Function Prototype*/
string getUserString();
void humanTurn(Boggle &myBoggle);
void showMessage(Boggle &myBoggle, Stack<string> &typedInWords);
void computerTurn(Boggle &myBoggle);
void winOrLose(Boggle &myBoggle);

/* Function: playOneGame
 * purpose: play one boggle game
*/
void playOneGame(Lexicon& dictionary) {
    //Set up the game
    string boardText;
    if(!getYesOrNo("Do you want to generate a random board? ")){
        boardText = getUserString();
    }
    Boggle myBoggle = Boggle(dictionary, boardText);
    int row = myBoggle.getBoggleBoard().numRows();
    int col = myBoggle.getBoggleBoard().numCols();
    //Initialize boggle board
    if(row >= 1 && row <= 6 && col >=1 && col <=6){
        BoggleGUI::initialize(row, col);

    }else{
        throw("Illegal board.");
    }
    //Create boggle GUI
    if(BoggleGUI::isInitialized()){
        if(boardText == ""){
            for(int r = 0; r < row; r++){
                for(int c = 0; c < col; c++){
                    Grid<char> boggleBoard = myBoggle.getBoggleBoard();
                    char ch = boggleBoard[r][c];
                    if(isalpha(ch) || isspace(ch)){
                        BoggleGUI::labelCube(r, c, ch);
                    }else{
                        throw("Illegal.");
                    }
                }
            }
        }else{
            int elementNum = row * col;
            if(elementNum != (int)boardText.length()){
                throw("Illegal.");
            }else{
                BoggleGUI::labelAllCubes(boardText);
            }
        }
    }else{
        BoggleGUI::reset();
    }
    humanTurn(myBoggle);
    computerTurn(myBoggle);
    winOrLose(myBoggle);
}

/* Function: getUserString
 * purpose: get the boardText from user input
*/
string getUserString(){
    string boardText;
    while(true){
        string userInput = getLine("Type the 16 letters to appear on the board: ");
        if(userInput.length() == LEGAL_LENGTH){
            bool allAlpha = true;
            for(int i = 0; i < (int)userInput.length(); i ++){
                if(!isalpha(userInput[i])){
                    allAlpha = false;
                    break;
                }
            }
            if(allAlpha == true){
                boardText = userInput;
                break;
            }else{
                cout << "That is not a valid 16-letter board string. Try again." << endl;
            }
        }else{
            cout << "That is not a valid 16-letter board string. Try again." << endl;
        }
    }
    boardText = toUpperCase(boardText);
    return boardText;
}

/* Function: humanTurn
 * purpose: human user turn of the boggle game
*/
void humanTurn(Boggle &myBoggle){
    Stack<string> typedInWords; // to return the last input word
    while(true){
        BoggleGUI::clearHighlighting();
        clearConsole();
        int numTypedWords = myBoggle.humanWords.size();
        showMessage(myBoggle, typedInWords);
        cout << myBoggle << endl;
        cout << "Your words (" << numTypedWords << "): " << myBoggle.humanWords << endl;
        cout << "Your score: " << myBoggle.getScoreHuman() << endl;
        BoggleGUI::setScore(myBoggle.getScoreHuman(), BoggleGUI::HUMAN);
        string newWord = getLine("Type a word (or Enter to stop): ");
        newWord = toUpperCase(newWord);
        if(newWord == "") break;
        typedInWords.push(newWord);
        if(myBoggle.checkWord(newWord)){
            if(myBoggle.humanWordSearch(newWord)){
                myBoggle.humanWords.add(newWord);
                BoggleGUI::recordWord(newWord, BoggleGUI::HUMAN);
                myBoggle.countScore(newWord);
            }
        }
    }
}

/* Function: showMessage
 * purpose: show the message of the boggle game
*/
void showMessage(Boggle &myBoggle, Stack<string> &typedInWords){
    if(typedInWords.size() == 0){
        cout << "It's your turn!" << endl;
        BoggleGUI::setStatusMessage("It's your turn!");
    }else if((int)typedInWords.peek().length() < 4){
        cout << "You must enter an unfound 4+ letter word from the dictionary." << endl;
        BoggleGUI::setStatusMessage("You must enter an unfound 4+ letter word from the dictionary.");
    }else if(!myBoggle.humanWordSearch(typedInWords.peek()) || !myBoggle.getLex().contains(typedInWords.peek())){
        cout << "That word can't be formed on this board." << endl;
        BoggleGUI::setStatusMessage("That word can't be formed on this board.");
    }else if(myBoggle.humanWordSearch(typedInWords.peek())){
        cout << "You found a new word! " << "\"" << typedInWords.peek() << "\"" << endl;
        string toPrint = "You found a new word! \"" + typedInWords.peek() + "\"";
        BoggleGUI::setStatusMessage(toPrint);
    }
}

/* Function: computerTurn
 * purpose: computer turn of the boggle game
*/
void computerTurn(Boggle &myBoggle){
    cout << " " << endl;
    cout << "It's my turn!" << endl;
    Set<string> computerWords = myBoggle.computerWordSearch();
    for(string key: computerWords){
        BoggleGUI::recordWord(key, BoggleGUI::COMPUTER);
    }
    int numTypedWords = computerWords.size();
    cout << "My words (" << numTypedWords << "): " << computerWords << endl;
    int computerScore = myBoggle.getScoreComputer();
    cout << "My score: " << computerScore << endl;
    BoggleGUI::setScore(computerScore, BoggleGUI::COMPUTER);
}

/* Function: winOrLose
 * purpose: show the result of the human VS computer boggle game
*/
void winOrLose(Boggle &myBoggle){
    if(myBoggle.getScoreComputer() > myBoggle.getScoreHuman()){
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
        BoggleGUI::setStatusMessage("Ha ha ha, I destroyed you. Better luck next time, puny human!");
    }else{
        cout << "WOW, you defeated me! Congratulations!" << endl;
        BoggleGUI::setStatusMessage("WOW, you defeated me! Congratulations!");
    }
}
