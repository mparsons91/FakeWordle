/*************************************
 * File:    proj1.cpp
 * Project: CMSC 202 Project 1, Fall 2021
 * Author:  Matthew Parsons
 * Date:    02/28/21
 * Section:
 * E-mail:  mparson1@umbc.edu
 *
 * This file contains the main function (and all functions for this
 * first project) of a text-based version of Wordle.  It generates
 * a random 5 character string answer from a wordbank .txt file and
 * then displays a game board for the user to guess five-letter words
 * until they use up six guesses and lose, or guess the correct word.
 * It also has restart functionality to continue the game loop should
 * the user want to try again.  It adheres to the project requirements
 * but is horribly un-optimized.
 */


#include <iostream>
#include <ctime>
#include <cstdlib>
#include <string>
#include <fstream>
using namespace std;


//function prototypes
int randomNum();
void playGame(string, string[]);
void displayBoard(string[], string[]);
string compareWords(string, string);
bool validWord(string[], string);
bool winConditionCheck(string);
bool restart();

// globals for the word comparison results
const char CORRECT_PLACE_AND_LETTER = '!';
const char CORRECT_LETTER = '&';

// Runs the gameloop
int main() {

    bool gameLoop = true;
    string answer = "error!";
    cout << "Welcome to UMBC Wordle" << endl;

    // Loads the text file and builds an array with all possible word choices
    int numOfWords = 2315;
    string wordBank[2316];
    ifstream myFile("proj1_data.txt");
    if (myFile.is_open()) {
        cout << "Your file was imported!" << endl;
        for (int i = 0; i < 2316; i++) {
            myFile >> wordBank[i];
        }
        cout << numOfWords << " words imported." << endl;
    }else{
        //will display if the word bank is not in the correct directory, or name is changed
        cout << "Wordlist file could not be opened" << endl;
    }
    myFile.close();


    // main game loop, operates separately from the read in of the text file
    while(gameLoop) {
        int randomInt = randomNum();
        answer = wordBank[randomInt];
        // cout << answer << endl;  this line is for testing
        playGame(answer, wordBank);
        gameLoop = restart();}
    return 0;
}

//  randomNum 
//  Outputs a random int between 1 and 2315
int randomNum(){
    //uses the time() function to give a variable seed to the rand() function
    srand(time(NULL));

    //generates an int between 1 and 2315
    int randomNum = (rand() % 2315);
    return randomNum;
}


// playGame
// Runs the gameloop
// string answer must be a 5 character string, wordBank will be
// an array of strings of 5 characters each
void playGame(string answer, string wordBank[]) {
    // answer must be a string length 5 chars
    // wordBank will be an array of strings, all of 5 chars in length

    int attempt = 0;
    bool playing = true;
    string guesses[] = {"_____", "_____", "_____", "_____", "_____", "_____"};
    string comparisons[] = {"_____", "_____", "_____", "_____", "_____", "_____"};
    string userGuess = "error!";

    // prompt the user for a guess
    cout << "Ok. I am thinking of a word with five letters." << endl;

    // asks the user for a string and runs till turns are all used
    while (attempt < 6 and playing) {
        cout << "What word would you like to guess?" << endl;
        cin >> userGuess;

        // word validation to ensure the user enters a string 5 characters
        // long and that it is in the wordlist
        bool valid = validWord(wordBank, userGuess);
        while (not valid) {
            cout << "That word is not in the word list" << endl;
            cout << "What word would you like to guess?" << endl;
            cin >> userGuess;
            valid = validWord(wordBank, userGuess);
        }

        // prints the guess and compares the guess with the correct answer
        cout << "You guessed " << userGuess << endl;
        guesses[attempt] = userGuess;
        comparisons[attempt] = compareWords(userGuess, answer);
        playing = winConditionCheck(comparisons[attempt]);
        if (not playing) {
            cout << endl << "Congrats you won!" << endl;
            cout << "The word was: " << answer << endl << endl;
            return;
        } else {
            displayBoard(guesses, comparisons);
//        cout << playing << endl; test line for the game loop bool flag
            attempt += 1;
        }
    }
}


// displayBoard
// guesses is an array of strings of length 6
// comparisons is also an array of strings length 6
void displayBoard(string guesses[], string comparisons[]){
    for(int i = 0; i < 6; i++){
        cout << guesses[i] << endl;
        if(comparisons[i] != "_____"){
            cout << comparisons[i] << endl;
        }
    }
}


// compareWords
// guess and answer must both be 5 char strings
// the function will modify the element of the result array to display 
string compareWords(string guess, string answer){
    string result = "_____";
    char letter = ' ';
    for(int i = 0; i < 6; i++) {
        if (guess[i] == answer[i]) {
            result[i] = CORRECT_PLACE_AND_LETTER;
        } else {
            for (int j = 0; j < 6; j++) {
                if (guess[i] == answer[j]) {
                    result[i] = CORRECT_LETTER;
                }
            }
        }
    }
    if(result == "_____"){
        return "_____";
    }else {
        return result;
    }
}


//validWord
// wordbank is the array of strings of the word list, guess is a five character string
// the function checks the user's input to verify it is a real word
// and in the list
bool validWord(string wordBank[], string guess){
    for(int i = 0; i < 2315; i++){
        if(wordBank[i] == guess){
            return true;
        }
    }
    return false;
}


// winConditionCheck
// returns true if the result has been compared to be correct
bool winConditionCheck(string result){
    if(result == "!!!!!"){
        return false;
    }
    return true;
}


// restart
// runs within the gameloop function and allows the user to play again
//without the game loop ending
bool restart(){
    char response = ' ';
    cout << "Would you like to play again? ('y' for yes / 'n' for no): ";
    cin >> response;
    while(response != 'n' and response != 'y'){
        cout << "Invalid input.  Please enter 'y' to play again, or 'n' to quit: ";
        cin >> response;
    }
    if(response == 'n'){
        return false;
    }
    return true;
}
