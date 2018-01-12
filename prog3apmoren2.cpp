/* --------------------------------------------------------
 *     Text based guessing game where the computer tries to predict the human's guess
 *     based on previous guess patterns. The score starts at 0 and if the computer
 *     guesses correctly, the score decreases by one. If the computer guesses incorrectly
 *     the score increases by one. When the score reaches 10 the human wins, or when the
 *     score reaches -10 the computer wins.
 *
 *
 * Class: Program #3 for CS 141, Fall 2017.
 * Lab: Tuesday 2pm,
 * TA: Abhinav Kumar
 * System: Windows 10
 * Author: Alex Moreno

   Running the program looks like:

Author:  Alex Moreno
Lab:     Tuesday 2pm
Program: #3, 0/1 Guessing Game

Welcome to the 0/1 guessing game!

Enter 0 or 1, trying to outsmart the computer, which is going to
attempt to forecast your guesses.  On each move the score indicator
moves to the right if the computer guesses correctly, and moves
to the left if the computer does not.
    Enter 't' to toggle displaying the data table.
    Enter 'r' to reset the board.
    Enter 'x' to exit.
Good luck, you'll need it!


Computer          Human
x---------x+++++++++x
          ^
1. Your input: 0
   Human gets a point.

Computer          Human
x---------x+++++++++x
           ^
2. Your input: 0
   Human gets a point.

Computer          Human
x---------x+++++++++x
            ^
3. Your input: 1
   Computer gets a point.

Computer          Human
x---------x+++++++++x
           ^
4. Your input: 0
   Computer gets a point.

Computer          Human
x---------x+++++++++x
          ^
5. Your input: t

Computer          Human
x---------x+++++++++x
          ^
         0   1
       --- ---
   000   2   1
   001   1   0
   010   0   0
   011   0   0
   100   0   0
   101   0   0
   110   0   0
   111   0   0
   Previous three: 010.  Forecast: 1
5. Your input: t

Computer          Human
x---------x+++++++++x
          ^
5. Your input: x
Thanks for playing!

 */

#include <iostream>
#include <iomanip> // For setw() in displayTable()
using namespace std;

//--------------------------------------------------------------------
// Displays start of program info
void displayProgramStartInfo()
{
	// Display required author info
    cout << "Author:  Alex Moreno" << endl;
    cout << "Lab:     Tuesday 2pm" << endl;
    cout << "Program: #3, 0/1 Guessing Game" << endl << endl;

    // Display the game's instructions
    cout << "Welcome to the 0/1 guessing game!" << endl << endl;
    cout << "Enter 0 or 1, trying to outsmart the computer, which is going to" << endl;
    cout << "attempt to forecast your guesses.  On each move the score indicator" << endl;
    cout << "moves to the right if the computer guesses correctly, and moves" << endl;
	cout << "to the left if the computer does not." << endl;
	cout << "    Enter 't' to toggle displaying the data table." << endl;
	cout << "    Enter 'r' to reset the board." << endl;
	cout << "    Enter 'x' to exit." << endl;
	cout << "Good luck, you'll need it!" << endl << endl;
} // End displayProgramStartInfo()


//--------------------------------------------------------------------
// Score input varies from -10 to 10, and is represented using ASCII graphics.
void displayScoreBar(int score)
{
    // Display the score bar
    cout << endl;
    cout << "Computer          Human" << endl;
    cout << "x---------x+++++++++x" << endl;

    // For loop prints spaces so that the pointer ends up in the correct position.
    for(int i = 0; i < score + 10; ++i) {
        cout << " ";
    }
    cout << "^" << endl; // Display the pointer to reflect the current score
} // End displayScoreBar()


//--------------------------------------------------------------------
// Takes in the last three moves as an integer array and returns a single
// integer which is equal in value to the binary number formed using the
// last three moves as digits, with the most recent move as the rightmost digit.
int lastThreeMovesToInt(int lastThreeMoves[]) {
	// To get binary value, sum the first digit multiplied by 2^2 (4), the
	// second digit multiplied by 2^1 (2) and the third digit multiplied by 2^0 (1)
	return (lastThreeMoves[0] * 4) + (lastThreeMoves[1] * 2) + lastThreeMoves[2];
} // End lastThreeMovesToInt()


//--------------------------------------------------------------------
// Displays the table of move history (passed in)
void displayTable(int moveHistoryTable[][2]) {
	// Display the header
	cout << "         0   1" << endl;
	cout << "       --- ---" << endl;

	// Display the 8 rows of the table
	for(int row = 0; row < 8; ++row) {
		cout << "   ";
		cout << row / 4 << (row % 4) / 2 << row % 2; // Displays the row number as a binary number digit by digit using integer division
		cout << setw(4) << moveHistoryTable[row][0] << setw(4) << moveHistoryTable[row][1] << endl; // Displays the actual number of each move for the row, keeping width at four for multi-digit numbers
	}
} // End displayTable()


//--------------------------------------------------------------------
// Resets the move history table (passed by reference) to 16 space separated user entered values
void resetTable(int moveHistoryTable[][2]) {
	// Prompts user
	cout << "Please enter 16 integers (separated by single spaces) to be the new" << endl;
	cout << "move history table values: ";

	// Sets the table values to the user's integers as they are entered in
	for(int i = 0; i < 8; ++i) {
		// cin stops at a space just like it stops at a newline
		cin >> moveHistoryTable[i][0];
		cin >> moveHistoryTable[i][1];
	}
} // End resetTable()


//--------------------------------------------------------------------
int main() {
	int score = 0; // The current game score
	char userInput = '?'; // The user's input each turn
	char computerGuess = '?'; // The computer's prediction each turn (should be '1' or '0')
	int moveNum = 1; // The current move's number, increments each move
	// Each row corresponds to a last three moves (8 = 2^3 rows) and each column corresponds to the resulting user input ('0' or '1', so two columns)
	int moveHistoryTable[8][2]; // Used to make the computer's predictions
	bool tableDisplayed = false; // Toggles displaying the table is displayed each turn, is diplayed if true
	int lastThreeMoves[3] = {0, 0, 0}; // Most recent move in third (rightmost) position

	// Initializes the 8 rows of the move history table to all 0s
	for(int i=0; i<8; i++) {
		moveHistoryTable[i][0] = 0;
        moveHistoryTable[i][1] = 0;
    }

	displayProgramStartInfo(); // Displays author info and game instructions

	// Each cycle of this loop is a turn. Exits loop when a player wins by checking the score.
	while(score < 10 && score > -10) {
		displayScoreBar(score); // Displays the score with ASCII graphics

		// Generates the computer's prediction
		// If not yet the 4th move, predicts the opposite of the last move (for the first turn it predicts '1' since the last move in lastThreeMoves is initialized to '0')
		if(lastThreeMoves[2] == 0 && moveNum <= 3) {
			computerGuess = '1';
		}
		else if(lastThreeMoves[2] == 1 && moveNum <= 3) {
			computerGuess = '0';
		}
		// If 4th move or beyond, uses moveHistoryTable to make prediction
		else {
			// Branch taken if in previous situations with the same last three moves, the user picked '0' more often than '1'
			if(moveHistoryTable[lastThreeMovesToInt(lastThreeMoves)][0] > moveHistoryTable[lastThreeMovesToInt(lastThreeMoves)][1]) {
				computerGuess = '0';
			}
			// Branch taken if in previous situations with the same last three moves, the user picked '0' less often than '1'
			else if(moveHistoryTable[lastThreeMovesToInt(lastThreeMoves)][0] < moveHistoryTable[lastThreeMovesToInt(lastThreeMoves)][1]) {
				computerGuess = '1';
			}
			// Branch taken if in previous situations with the same last three moves, the user picked '0' and '1' equally often
			else {
				// When the move history table has a tie, the computer guesses the opposite of the last move
				computerGuess = (lastThreeMoves[2] == 0) ? '1' : '0'; // Sets computerGuess to '1' if the user's last move was '0', otherwise sets computerGuess to '0'
			}
		}

		// If the table being displayed is toggled on, displays the move history table, last three moves and computer's prediction for current move
		if(tableDisplayed) {
			displayTable(moveHistoryTable);
			cout << "   Previous three: " << lastThreeMoves[0] << lastThreeMoves[1] << lastThreeMoves[2] << ".  ";
			cout << "Forecast: " << computerGuess << endl;
		}

		// Gets the user's input
		cout << moveNum << ". Your input: ";
		cin >> userInput;

		// If the user enters 'r' or 'R', resets the move history table, move number and last three moves
		if(userInput == 'r' || userInput == 'R') {
			resetTable(moveHistoryTable); // Prompts the user for values and resets moveHistoryTable to those values
			moveNum = 1;
			lastThreeMoves[0] = 0;
			lastThreeMoves[1] = 0;
			lastThreeMoves[2] = 0;
			continue;
		}

		// If the user enters 'x' or 'X' the game quits
		if(userInput == 'x' || userInput == 'X') {
			cout << "Thanks for playing!" << endl;
			break; // Exits while loop
		}

		// If the user enters 't' or 'T' the table display is toggled and the next turn is started without increasing the move number
		if(userInput == 't' || userInput == 'T') {
			tableDisplayed = !tableDisplayed; // Toggle tableDisplayed
			continue;
		}

		// Branch taken if user enters a legitimate move
		if(userInput == '0' || userInput == '1') {
			++moveNum; // Increments the move number

			// Updates the move history table according to what the last three moves are (first index) and what the user inputed (second index)
			++moveHistoryTable[lastThreeMovesToInt(lastThreeMoves)][static_cast<int>(userInput - '0')]; // In the second index '0' is converted to 0 or '1' is converted to 1

			// Updates the last three moves
			lastThreeMoves[0] = lastThreeMoves[1];
			lastThreeMoves[1] = lastThreeMoves[2];
			lastThreeMoves[2] = static_cast<int>(userInput - '0'); // The most recent move is set to the integer version of the user input (i.e. 0 for '0' or 1 for '1')

			// If the computer guessed the human's choice correctly this branch is taken
			if(computerGuess == userInput) {
				--score; // Decrements the score since the computer got a point

				cout << "   Computer gets a point." << endl;

				// Prints a warning message when the score reaches -5
				if(score == -5) {
					cout << ">>> You're going to lose! <<<" << endl;
				}
			}
			// If the computer guessed the human's choice incorrectly this branch is taken
			else {
				++score; // Increments the score since the human got a point

				cout << "   Human gets a point." << endl;

				// Prints a message of human superiority when the score reaches 5
				if(score == 5) {
					cout << ">>> The robots will never take over! <<<" << endl;
				}
			}
		}
	}

	// Prints out the winner of the game (if there is a winner and the game wasn't just quit with 'x')
	if(score == -10) {
		cout << "Computer wins!";
	}
	else if(score == 10) {
		cout << "Human wins!";
	}

    return 0;
} // End main()
