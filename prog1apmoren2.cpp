//============================================================================
// Program #1: Guess It
// Author: Alex Moreno
// UIC NetID: apmoren2
// Lab: Tuesday 2pm
// TA: Abhinav Kumar
// Date: September 11, 2017
// System:  C++ in Cloud 9
// Description : This program prints the required author information, then
//				 prints out a table of the numbers 0 to 99 and corresponding
//				 random uppercase and lowercase letters. It asks the user to
//				 then pick any two digit number from the table, subtract both
//				 of that number's digits from itself, and then look up the
//				 letter corresponding to that resulting number. The user is
//				 then prompted to press 'r', after which the program
//				 (correctly) guesses the letter. Running the program multiple
//				 times will result in a different randomly generated letter
//				 table. Running the program looks like this:
/*

Program #1: Guess It
Author: Alex Moreno
UIC NetID: apmoren2
Lab: Tuesday 2pm
TA: Abhinav Kumar
Date: September 11, 2017
System:  C++ in Cloud 9

 99:D 98:A 97:K 96:p 95:J 94:e 93:E 92:b 91:s 90:D
 89:l 88:a 87:y 86:e 85:C 84:w 83:C 82:R 81:D 80:z
 79:a 78:O 77:w 76:W 75:N 74:f 73:f 72:D 71:l 70:d
 69:K 68:m 67:j 66:z 65:v 64:L 63:D 62:E 61:b 60:s
 59:X 58:y 57:n 56:J 55:O 54:D 53:h 52:R 51:S 50:f
 49:I 48:c 47:P 46:C 45:D 44:z 43:t 42:y 41:j 40:S
 39:V 38:i 37:P 36:D 35:J 34:e 33:b 32:T 31:Y 30:B
 29:s 28:o 27:D 26:y 25:D 24:J 23:G 22:b 21:x 20:Q
 19:Z 18:D 17:a 16:u 15:O 14:G 13:t 12:s 11:y 10:R
  9:D  8:D  7:m  6:O  5:L  4:g  3:Y  2:D  1:Q  0:D

1. Choose any two-digit number in the table above (e.g. 73).
2. Subtract its two digits from itself (e.g. 73 - 7 - 3 = 63)
3. Find this new number (e.g. 63) and remember the letter next to it.
4. Now press 'r' and I'll read your mind...r

You selected the character: D

*/
//============================================================================

#include <iostream>
#include <cstdlib>
#include <ctime> // Enables use of the time() function
using namespace std;

int main() {
	// Declares and initializes variables
	char specialChar = '?'; // This is the character that is on multiples of 9 in the table, the correct guess
	char currChar = '?'; // This is the current character to be printed in the table
	char userChar = '?'; // This is the character the user enters, should be 'r'
	int randInt = 0; // Used as a random integer to pick letters in the table

	srand(time(0)); // Seeds the pseudo-random number generator with a unique number (the system's time)

	// Randomly picks the special character
	randInt = rand() % (26 * 2); // 26 letters in the alphabet, even results in capital letter, odd lowercase
	(randInt % 2 == 0) ? specialChar = static_cast<char>(65 + randInt / 2) : specialChar = static_cast<char>(97 + randInt / 2); // Assigns specialChar, randInt / 2 is some integer 0 to 25

	// Prints required author information
	cout << "Program #1: Guess It" << endl;
	cout << "Author: Alex Moreno" << endl;
	cout << "UIC NetID: apmoren2" << endl;
	cout << "Lab: Tuesday 2pm" << endl;
	cout << "TA: Abhinav Kumar" << endl;
	cout << "Date: September 11, 2017" << endl;
	cout << "System:  C++ in Cloud 9" << endl << endl; // endl twice to create a blank line after author info

	// Loops through the integers 99 down to 0, printing the table as it goes
	for (int i = 99; i >= 0; --i) {
		//Picks the character to be printed
		if (i % 9 == 0) { // Sets the character to be printed to the special character if the current number is a multiple of 9
			currChar = specialChar;
		}
		else { // Randomly picks the current character to be printed if the current number isn't a multiple of 9
			randInt = rand() % (26 * 2); // 26 letters in the alphabet, even results in capital letter, odd lowercase
			(randInt % 2 == 0) ? currChar = static_cast<char>(65 + randInt / 2) : currChar = static_cast<char>(97 + randInt / 2); // Assigns currChar, randInt / 2 is some integer 0 to 25
		}

		// Prints the number and letter
		if (i <= 9) { // One digit numbers need two preceding spaces to maintain spacing of the table
			cout << "  " << i << ":" << currChar;
		}
		else { // Two digit numbers only need one space
			cout << " " << i << ":" << currChar;
		}
		if (i % 10 == 0) { // Starts new row of table every ten numbers
			cout << endl;
		}
	}

	// Gives the user instructions
	cout << endl; // Creates a blank line after the table
	cout << "1. Choose any two-digit number in the table above (e.g. 73)." << endl;
	cout << "2. Subtract its two digits from itself (e.g. 73 - 7 - 3 = 63)" << endl;
	cout << "3. Find this new number (e.g. 63) and remember the letter next to it." << endl;

	// Repeatedly prompts the user to enter 'r' until they actually enter 'r'
	do {
		cout << "4. Now press 'r' and I'll read your mind...";
		cin >> userChar; // Gets the user entered character, note that if the user enters a multi-character string, bad things happen since userChar is a char variable
	} while (userChar != 'r');

	cout << endl << "You selected the character: " << specialChar << endl; // Correctly guesses the user's character

	return 0;
}
