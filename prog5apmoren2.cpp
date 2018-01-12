/* -------------------------------------------------------------------------------------------------
 *		Text based version of the game Boggle, where the user is given a 4 by 4 board of letters and
 *		must enter as many words as possible before the clock runs out. Valid words to enter must be
 *		able to be made out of a contiguous (including diagonal bordering) in order line of letters
 *		on the board, without using a specific letter on the board twice.
 *
 * Class: CS141 Fall 2017
 * Lab: Tuesday 2pm,
 * TA: Abhinav Kumar
 * System: Windows 10, Eclipse Oxygen IDE
 * Author: Alex Moreno
 */

#include <iostream> // For Input and Output
#include <fstream>  // For file input and output
#include <cassert>  // For the assert statement
#include <cctype>   // Allows using the tolower() function
#include <ctime>    // Timer functions
#include <cstring>  // For strlen
#include <cstdlib>  // For exit() and MAX_RAND
using namespace std;

// Global constants
const char DictionaryFileName[] = "dictionary.txt";
const char ResetWord[] = "r";		 // Word or char user must enter to reset the board
const char TimerToggleWord[] = "t";  // Word or char user must enter to toggle the timer on or off
const char DisplayAllWord[] = "s";	 // Word or char user must enter to display all possible words on the boggle board
const char ExitWord[] = "x";	 	 // Word or char user must enter to quit the game
const int MaxNumberOfWords = 263533; // Number of dictionary words
const int MinWordLength = 3;         // Minimum dictionary word size to be stored
const int MaxWordLength = 16;	     // Max word size.  Add 1 for null
const int MaxUserInputLength = 81;   // Max user input length
const int NumberOfLetters = 26;      // Letters in the alphabet
const int TotalSecondsToPlay = 60;   // Total number of seconds to play per board

// -------------------------------------------------------------------------------------------------
// A 4 by 4 board for playing boggle on
class BoggleBoard {
public:
	BoggleBoard(char letters[]); // The constructor for the BoggleBoard class
	bool containsWord(char word[]); // Returns true if the board contains the passed in word
	void display(); // Prints the board, without the border
	void reset(); // Prompts the user for 16 chars to reset the board with
private:
	char board[36]; // The letters on the board, with a border. Every six indices is a new row of the board
	bool containsWordRecursive(char word[], int pos, char availableLetters[]); // Helps containsWord()
};


// -------------------------------------------------------------------------------------------------
// The constructor for the BoggleBoard class. Initializes the private board char array variable to
// the passed in array of letters, but adds a border. Note there is no default constructor for this class. The passed
// in array must be a char array of length 16
BoggleBoard::BoggleBoard(char letters[]) {
	int lettersIndex = 0; // The index in the letters array of the next letter to be added to the board
	// Loops through the entire board, including the border
	for(int boardIndex = 0; boardIndex < 36; ++boardIndex) {
		// Makes the top and bottom rows of the border
		if(boardIndex < 6 || boardIndex >= 30) {
			board[boardIndex] = '*';
		}
		// Makes the right and left columns of the border
		else if(boardIndex % 6 == 0 || boardIndex % 6 == 5) {
			board[boardIndex] = '*';
		}
		// Makes the middle of the board, where the letters are
		else {
			// Transfers a letter from the passed in array to the board and then increments the letter index
			board[boardIndex] = letters[lettersIndex];
			++lettersIndex;
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Returns true if there is a contiguous group of letters on the boggle board which contains all the
// letters in the passed in word. The passed in word can be at most 16 letters long, since the board
// only has 16 letters. The passed in word must be a null terminated c-string.
bool BoggleBoard::containsWord(char word[]) {
	char availableBoard[36]; // A version of the board array passed to containsWordRecursive()

	// The board is defined to not contain the null word
	if(word[0] == '\0') {return false;}

	// Checks for each position on the board (the int pos) if the word can be formed on the board starting from
	// that position using the helper function containsWordRecursive()
	for(int pos = 0; pos < 36; ++pos) {
		// Branch taken if the first letter of the word is at this spot on the board
		if(board[pos] == word[0]) {
			memcpy(availableBoard, board, sizeof(board)); // Copies the contents of board into availableBoard
			availableBoard[pos] = '*'; // Marks the found first letter of the word as off limits to containsWordRecursive

			// Returns true if containsWordRecursive says the rest of the word can be made from adjacent letters
			if(containsWordRecursive(&(word[1]), pos, availableBoard)) {return true;}
		}
	}
	return false; // If the first letter of the word is not on the board, returns false
}


// -------------------------------------------------------------------------------------------------
// Determines if the passed in null terminated c-string is adjacent to the passed in position (pos) in the
// passed in array of available board spaces (availableBoard, must be length 36). Each level of the recursion checks only the next
// letter in the word, which is the first letter of the word at that level since each successive recursive call
// passes in a shorter version of the word without the first letter.
bool BoggleBoard::containsWordRecursive(char word[], int pos, char availableBoard[]) {
	char updatedAvailableBoard[36]; // The new version of available board positions passed to next recursion call
	int adjacentSpots[8] = {-1, -7, -6, -5, 1, 7, 6, 5}; // The index offsets from the current spot for all 8 adjacent spots in the board array

	// If the first character in word is the null character, we've reached the end of the word
	// without returning false in an earlier recursive call, so the word is on the board and true is returned
	if(word[0] == '\0') {
		return true;
	}

	// Checks each adjacent spot on the board for the next letter in the word
	for(int i = 0; i < 8; ++i) {
		// Branch taken if the next letter in the word is directly above the passed in position (the last letter of the word)
		if(availableBoard[pos + adjacentSpots[i]] == word[0]) {
			memcpy(updatedAvailableBoard, availableBoard, 36); // Copies the contents of availableBoard into updatedAvailableBoard
			updatedAvailableBoard[pos + adjacentSpots[i]] = '*'; // updatedAvailableBoard is now availableBoard but with one more letter off limits
			if(containsWordRecursive(&(word[1]), pos + adjacentSpots[i], updatedAvailableBoard)) {
				return true; // Returns true if the rest of the word is adjacent to the above letter, because then the entire word is on the board
			}
		}
	}
	return false; // Returns false if all adjacent spot options are exhausted
}


// -------------------------------------------------------------------------------------------------
// Prints the letters on the board (not the border), in a 4 by 4 square
void BoggleBoard::display() {
	// Loops through the letters on the board, 7 is the index of the top left letter, 28 the index of the bottom right letter
	for(int i = 7; i <= 28; ++i) {
		// Branch taken only if the current index isn't on the border
		if(i % 6 != 0 && i % 6 != 5) {
			cout << board[i] << " "; // Prints the letter on the board
			if(i % 6 == 4) {cout << endl;} // Makes a new row every 4 letters printed
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Prompts the user for 16 chars, and then uses them to reset the board from the left to right, top to bottom
void BoggleBoard::reset() {
	char userInput[MaxUserInputLength]; // The characters used to reset the board

	cout << "Enter 16 characters to be used to set the board: ";
	cin >> userInput;

	int inputIndex = 0; // The index in the userInput array of the next letter to be added to the board
	// Loops through the letters on the board, 7 is the index of the top left letter, 28 the index of the bottom right letter
	for(int boardIndex = 7; boardIndex <= 28; ++boardIndex) {
		// Branch taken only if the current index isn't on the border
		if(boardIndex % 6 != 0 && boardIndex % 6 != 5) {
			// Puts a letter of the input on the board then increments the input index
			board[boardIndex] = userInput[inputIndex];
			++inputIndex;
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Displays required author ID info
void displayIDInfo() {
	cout << "Program: #5, Boggle" << endl;
    cout << "Author: Alex Moreno" << endl;
    cout << "TA: Abhinav Kumar"   << endl;
    cout << "Lab: Tuesday 2pm" 	  << endl << endl;
}


// -------------------------------------------------------------------------------------------------
// Display the game's instructions
void displayInstructions() {
    cout << "Welcome to the game of Boggle, where you play against the clock"  	<< endl;
    cout << "to see how many words you can find using adjacent letters on the" 	<< endl;
    cout << "board.  Each letter can be used only once for a given word." 	 	<< endl << endl;

    cout << "When prompted to provide input you may also:" 						<< endl;
    cout << "     Enter 'r' to reset the board to user-defined values." 		<< endl;
    cout << "     Enter 's' to solve the board and display all possible words." << endl;
    cout << "     Enter 't' to toggle the timer on/off." 						<< endl;
    cout << "     Enter 'x' to exit the program." 								<< endl << endl;
}


// -------------------------------------------------------------------------------------------------
// Reads in the dictionary. First dynamically allocates space for the dictionary. Then reads in
// words from the file. Note that the '&' is needed so that the new array address is passed back as
// a reference parameter.
void readInDictionary(char **&dictionary,    // Dictionary of words
					  long &numberOfWords) { // The number of words stored
	// Allocates space for a large array of C-style strings
	dictionary = new char*[MaxNumberOfWords];

    // For each array entry, allocates space for the word (C-string) to be stored there
    for(int i = 0; i < MaxNumberOfWords; ++i) {
        dictionary[i] = new char[MaxWordLength+1];
        // Just to be safe, initializes C-strings to all null characters
        for(int j = 0; j < MaxWordLength; ++j) {
            dictionary[i][j] = '\0';
        }
    }

    ifstream inStream; 				   // Declare an input file stream to use
    inStream.open(DictionaryFileName); // Opens the file for reading
    assert(!inStream.fail()); 		   // Makes sure the file open was OK

    // Keeps adding words to the dictionary while input from the file yields a word
    numberOfWords = 0; // Row of the file for the current word
    char theWord[81];  // Declares input space to be clearly larger than largest word
    while(inStream >> theWord) {
        int wordLength = strlen(theWord);
        if(wordLength >= MinWordLength && wordLength <= MaxWordLength) {
            strcpy(dictionary[numberOfWords], theWord); // Adds the current word to the dictionary
            numberOfWords++; 							// Increments the number of words
        }
    }

    cout << "The dictionary total number of words is: " << MaxNumberOfWords << endl;
    cout << "Number of words of the right length is:  " << numberOfWords << endl;

    inStream.close(); // Closes the file
}


// -------------------------------------------------------------------------------------------------
// Uses binary search to look up the search word in the dictionary array, returning index if found,
// -1 otherwise
int binarySearch(const char searchWord[MaxWordLength+1], // Word to be looked up
                 char **dictionary) {              	  	 // The dictionary of words
    int low, mid, high; // Array indices for binary search
    int searchResult;   // Tells where the word is in relation to the currently checked spot during binary search

    // Uses binary search to find the word
    low = 0;
    high = MaxNumberOfWords - 1;
    while(low <= high)  {
        mid = (low + high) / 2;
        // SearchResult negative value means word is to the left, positive value means word is to
        // the right, value of 0 means word was found
        searchResult = strcmp(searchWord, dictionary[mid]);
        if(searchResult == 0) {
            return mid; // Word IS in dictionary, so return the index where the word was found
        }
        else if(searchResult < 0) {
            high = mid - 1; // Word should be located prior to mid location
        }
        else {
            low = mid + 1; // Word should be located after mid location
        }
    }

    return -1; // Word was not found
}


// -------------------------------------------------------------------------------------------------
// Finds a random character using a table of letter frequency counts. Iterates through the array and
// finds the first position where the random number is less than the value stored. The resulting
// index position corresponds to the letter to be generated (0='a', 1='b', etc.)
char getRandomCharacter() {

	// The following table of values came from the frequency distribution of letters in the dictionary
    float letterPercentTotals[NumberOfLetters] = {
                0.07680,  //  a
                0.09485,  //  b
                0.13527,  //  c
                0.16824,  //  d
                0.28129,  //  e
                0.29299,  //  f
                0.32033,  //  g
                0.34499,  //  h
                0.43625,  //  i
                0.43783,  //  j
                0.44627,  //  k
                0.49865,  //  l
                0.52743,  //  m
                0.59567,  //  n
                0.66222,  //  o
                0.69246,  //  p
                0.69246,  //  q
                0.76380,  //  r
                0.86042,  //  s
                0.92666,  //  t
                0.95963,  //  u
                0.96892,  //  v
                0.97616,  //  w
                0.97892,  //  x
                0.99510,  //  y
                1.000001}; //  z (changed to 1.000001 so that if rand() returns RAND_MAX the function still works)

    // Generates a random number between 0..1
    // Multiplies by 1.0 because otherwise integer division truncates remainders
    float randomNumber = 1.0 * rand() / RAND_MAX;

    // Finds the first position where our random number is less than the value stored.  The index
    // corresponds to the letter to be returned, where 'a' is 0, 'b' is 1, and so on.
    for(int i = 0; i < NumberOfLetters; ++i) {
        if(randomNumber < letterPercentTotals[i]) {
            // We found the spot. Returns the corresponding letter
            return (char) 'a' + i;
        }
    }

    // Sanity check
    cout << "No alphabetic character generated.  This should not have happened. Exiting program.\n";
    exit(-1);
    return ' '; // Should never get this
}


// -------------------------------------------------------------------------------------------------
// Prompts the user for a min and max word length, and sets an element of the passed in boolean array (of length numberOfWords) corresponding to the word in the
// passed in dictionary (containing the passed in number of words) at the same index to true if that word is on the passed in Boggle board, and is the right length
void findAllWords(char **dictionary, 	  	  // The dictionary of words to look through
					 int numberOfWords, 	  // The length of dictionary and foundWords
					 bool foundWords[], 	  // An element of foundWords will be set to true if the word at the same index in dictionary is on the board (and the right length)
					 BoggleBoard gameBoard) { // The board on which words are looked for
	int maxLen; // The length of the longest words to find
	int minLen; // The length of the shortest words to find

	cout << "Enter min and max word lengths to display: ";
	cin >> minLen;
	cin >> maxLen;

	cout << "Words between " << minLen << " and " << maxLen << " are:" << endl;
	// Loops through all numberOfWords words in the dictionary
	for(int i = 0; i < numberOfWords; ++i) {
		// Updates corresponding element in foundWords if the current word is the right length and on the boggle board
		if(strlen(dictionary[i]) >= (unsigned)minLen && strlen(dictionary[i]) <= (unsigned)maxLen && gameBoard.containsWord(dictionary[i])) {
			foundWords[i] = true;
		}
	}
}


// -------------------------------------------------------------------------------------------------
// Displays the words in the passed in dictionary of words that were also marked as true in the passed in boolean array.
// Both arrays must be of length numberOfWords. Displays words in order from shortest to longest, and in alphabetical order within each length group
// Will only display words of length 3 to 16 inclusive. Ends with a new line.
void displayFoundWords(char **dictionary, int numberOfWords, bool foundWords[]) {
	// Each iteration of the loop prints out all words of that length in alphabetical order
	for(int wordLength = 3; wordLength <= 16; ++wordLength) {
		// Loops through all the words in the dictionary in alphabetical order
		for(int i = 0; i < numberOfWords; ++i) {
			// Prints the word if it is set to true in foundWords and the word is the right length
			if(foundWords[i] && strlen(dictionary[i]) == (unsigned)wordLength) {
				cout << dictionary[i] << " ";
			}
		}
	}
	cout << endl; // Prints new line
}


// -------------------------------------------------------------------------------------------------
int main()
{
    // Declares variables
    char **dictionary;  				// 2d array of dictionary words, dynamically allocated
    long numberOfWords; 				// How many words actually found in dictionary
    char userInput[MaxUserInputLength]; // The user's input string each turn
    char randomLetters[16];             // The length 16 array of random letters used to make the board
    int turnNum;						// The turn number, increments by one every turn
    int score;							// The game score
    int elapsedSeconds;					// Used to end the game once the time to play is up
    int wordPosInDictionary;			// The index in the dictionary of a user entered word
    bool timerOff;						// If set to true, the game won't end from running out of time

    srand(time(NULL)); // Uses the system time as a seed for the random number generator

    time_t startTime = time(NULL); // Declares a variable to hold a time, and gets the current time

    displayIDInfo();
    displayInstructions();

    // Reads in the dictionary. numberOfWords returns the actual number of words found
    readInDictionary(dictionary, numberOfWords);

    bool foundWords[numberOfWords]; // For each index, tells whether or not the word at that same index in the dictionary has been found on the board
    // Initializes foundWords to all false
    for(int i = 0; i < numberOfWords; ++i) {
    	foundWords[i] = false;
    }

    // Makes an array of random letters for creating the board
    for(int pos = 0; pos < 16; ++pos) {
    	randomLetters[pos] = getRandomCharacter();
    }

    BoggleBoard gameBoard(randomLetters); // Makes the Boggle board on which the game is played, using the generated letters

    // Loop through turns while there is still time left
    elapsedSeconds = 0;
    turnNum = 1;
    score = 0;
    timerOff = false;
    while(elapsedSeconds < TotalSecondsToPlay || timerOff) {
    	// Shows the user the time remaining, the board and their current score
    	cout << endl << "  " << TotalSecondsToPlay - elapsedSeconds << " seconds remaining." << endl;
    	gameBoard.display();
    	cout << "   Score: " << score << endl;

        // Prompt for and get user input
        cout << turnNum << ". Enter a word: ";
        cin >> userInput;

        // Resets the board if the user enters 'r'
        if(strcmp(userInput, ResetWord) == 0) {
        	gameBoard.reset();
        }
        // Quits the game by exiting the while loop if the user enters 'x'
        else if(strcmp(userInput, ExitWord) == 0) {
        	break;
        }
        // Displays all the possible words on the board between a min and max length and then exits the game by exiting the while loop if the user enters 's'
        else if(strcmp(userInput, DisplayAllWord) == 0) {
        	// Resets foundWords to all false so that previously found words of the wrong length aren't displayed
        	for(int i = 0; i < numberOfWords; ++i) {
				foundWords[i] = false;
			}

        	findAllWords(dictionary, numberOfWords, foundWords, gameBoard); // Updates foundWords
        	displayFoundWords(dictionary, numberOfWords, foundWords); // Displays the updated found words
        	break;
        }
        // Toggles whether or not the game ends from time running out (doesn't stop the timer) if the user enters 't'
        else if(strcmp(userInput, TimerToggleWord) == 0) {
        	timerOff = !timerOff; // Toggles timerOff
        	if(timerOff) {
        		cout << "Timer now won't end game, can ignore seconds left." << endl;
        	}
        }
        // If none of these control codes were entered, the user must have entered a word
        else {
        	wordPosInDictionary = binarySearch(userInput, dictionary); // Finds the index of the word in the dictionary

        	// If the word is the wrong length, outputs an error message
			if(strlen(userInput) < (unsigned)3 || strlen(userInput) > (unsigned)16) {
				cout << " isn't between " << MinWordLength << " and " << MaxWordLength << " letters (inclusive)." << endl;
			}
        	// If the word isn't in the dictionary, outputs an error message
			else if(wordPosInDictionary == -1) {
			   cout << " was not found in the dictionary." << endl;
			}
        	// If the word isn't on the board, outputs an error message
			else if(!gameBoard.containsWord(userInput)) {
				cout << " cannot be formed on this board." << endl;
			}
        	// If the word was already found, outputs an error message and shows the previously found words
			else if(foundWords[wordPosInDictionary] == true) {
				cout << "Sorry, that word was already previously found." << endl;
				cout << "Words so far are: ";
				displayFoundWords(dictionary, numberOfWords, foundWords);
				cout << endl;
			}
        	// Branch taken if the user found a new dictionary word of the right length on the board
			else {
				foundWords[wordPosInDictionary] = true; // Marks that the word has been found
				if(strlen(userInput) == 3) {
					score += 1; // Words of length 3 are worth one point
					cout << "    Worth 1 point." << endl;
				}
				else if(strlen(userInput) == 4) {
					score += 2; // Words of length 4 are worth two points
					cout << "    Worth 2 points." << endl;
				}
				else if(strlen(userInput) == 5) {
					score += 4; // Words of length 5 are worth 4 points
					cout << "    Worth 4 points." << endl;
				}
				else {
					score += strlen(userInput); // Words longer than 5 letters are worth their length in points
					cout << "    Worth " << strlen(userInput) << " points.";
				}

				// Shows the updated list of words that have been found on the board
				cout << "Words so far are: ";
				displayFoundWords(dictionary, numberOfWords, foundWords);

				++turnNum; // Increments the turn number
			}
        }

        // Calculates how many seconds have elapsed since we started the timer
        elapsedSeconds = difftime(time(NULL), startTime);
    }
    cout << "Final score: " << score << endl;
    cout << "Exiting the program." << endl;

    return 0;
}
