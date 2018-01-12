/* --------------------------------------------------------
 * Traffic.cpp
 *     Text-based version of the game RushHour, where the
 *     object is to move the vehicles such that the small
 *     car can exit the right of the board.
 *
 * Class: Program #2 for CS 141, Fall 2017.
 * Lab: 2pm Tuesdays,
 * TA: Abhinav Kumar
 * System: Cloud9
 * Author: Alex Moreno
 */

#include <iostream>
#include <cctype>      // for toupper()
using namespace std;

// Global variables and constants
// Variables to store the board pieces
char  p1, p2, p3, p4, p5, p6,
      p7, p8, p9,p10,p11,p12,
     p13,p14,p15,p16,p17,p18,
     p19,p20,p21,p22,p23,p24,
     p25,p26,p27,p28,p29,p30,
     p31,p32,p33,p34,p35,p36;


//--------------------------------------------------------------------
// Display ID info
void displayIDInfo()
{
    cout << "Author:  Alex Moreno" << endl;
    cout << "Lab:     Tuesday 2pm" << endl;
    cout << "TA:    Abhinav Kumar" << endl;
    cout << "Program: #2, Traffic" << endl;
    cout << endl;
}//end displayIDInfo()


//--------------------------------------------------------------------
// Display Instructions
void displayInstructions()
{
    cout << "Welcome to the traffic game!                          " << endl
         << "                                                      " << endl
         << "Move the vehicles so that the Red car (RR) can exit   " << endl
         << "the board to the right. Each move should be of the    " << endl
         << "of the form:   CDN   where:                           " << endl
         << "   C  is the vehicle to be moved                      " << endl
         << "   D  is the direction (u=up, d=down, l=left or r=right)" << endl
         << "   N  is the number of squares to move it             " << endl
         << "For example GR2  means move the G vehicle to the right" << endl
         << "2 squares.  Lower-case input such as   gr2  is also   " << endl
         << "accepted.  Enter '-' to reset board, or 'x' to exit.  " << endl << endl;
}//end displayInstructions()



//--------------------------------------------------------------------
// Display the board, using the current values in the global variables for the board pieces.
void displayBoard()
{
    cout << endl << "- - - - - - - -" << endl; // Displays a preceding blank line and then the top of the board outline
    cout << "| " << p1 << " " << p2 << " " << p3 << " " << p4 << " " << p5 << " " << p6 << " |" << endl; // Displays first row of board
    cout << "| " << p7 << " " << p8 << " " << p9 << " " << p10 << " " << p11 << " " << p12 << " |" << endl; // Displays second row of board
    cout << "| " << p13 << " " << p14 << " " << p15 << " " << p16 << " " << p17 << " " << p18 << "  " << endl; // Displays third row of board
    cout << "| " << p19 << " " << p20 << " " << p21 << " " << p22 << " " << p23 << " " << p24 << " |" << endl; // Displays fourth row of board
    cout << "| " << p25 << " " << p26 << " " << p27 << " " << p28 << " " << p29 << " " << p30 << " |" << endl; // Displays fifth row of board
    cout << "| " << p31 << " " << p32 << " " << p33 << " " << p34 << " " << p35 << " " << p36 << " |" << endl; // Displays sixth row of board
    cout << "- - - - - - - -" << endl; // Displays the bottom of the board outline

}//end displayBoard()



//--------------------------------------------------------------------
// Given a number 1..36, this would return the current character stored in the corresponding p1..p36 variable.
char getPieceAt(int pieceIndex) {
	switch(pieceIndex) {
		case 1:
			return p1;
			break;
		case 2:
			return p2;
			break;
		case 3:
			return p3;
			break;
		case 4:
			return p4;
			break;
		case 5:
			return p5;
			break;
		case 6:
			return p6;
			break;
		case 7:
			return p7;
			break;
		case 8:
			return p8;
			break;
		case 9:
			return p9;
			break;
		case 10:
			return p10;
			break;
		case 11:
			return p11;
			break;
		case 12:
			return p12;
			break;
		case 13:
			return p13;
			break;
		case 14:
			return p14;
			break;
		case 15:
			return p15;
			break;
		case 16:
			return p16;
			break;
		case 17:
			return p17;
			break;
		case 18:
			return p18;
			break;
		case 19:
			return p19;
			break;
		case 20:
			return p20;
			break;
		case 21:
			return p21;
			break;
		case 22:
			return p22;
			break;
		case 23:
			return p23;
			break;
		case 24:
			return p24;
			break;
		case 25:
			return p25;
			break;
		case 26:
			return p26;
			break;
		case 27:
			return p27;
			break;
		case 28:
			return p28;
			break;
		case 29:
			return p29;
			break;
		case 30:
			return p30;
			break;
		case 31:
			return p31;
			break;
		case 32:
			return p32;
			break;
		case 33:
			return p33;
			break;
		case 34:
			return p34;
			break;
		case 35:
			return p35;
			break;
		case 36:
			return p36;
			break;
		default:
			return '?';
	}
}//end getPieceAt()



//--------------------------------------------------------------------
// Given a number 1..36 and a character, this would store that character into the corresponding p1..p36 variable.
void setPieceAt(int pieceIndex, char settingCharacter) {
	switch(pieceIndex) {
		case 1:
			p1 = settingCharacter;
			break;
		case 2:
			p2 = settingCharacter;
			break;
		case 3:
			p3 = settingCharacter;
			break;
		case 4:
			p4 = settingCharacter;
			break;
		case 5:
			p5 = settingCharacter;
			break;
		case 6:
			p6 = settingCharacter;
			break;
		case 7:
			p7 = settingCharacter;
			break;
		case 8:
			p8 = settingCharacter;
			break;
		case 9:
			p9 = settingCharacter;
			break;
		case 10:
			p10 = settingCharacter;
			break;
		case 11:
			p11 = settingCharacter;
			break;
		case 12:
			p12 = settingCharacter;
			break;
		case 13:
			p13 = settingCharacter;
			break;
		case 14:
			p14 = settingCharacter;
			break;
		case 15:
			p15 = settingCharacter;
			break;
		case 16:
			p16 = settingCharacter;
			break;
		case 17:
			p17 = settingCharacter;
			break;
		case 18:
			p18 = settingCharacter;
			break;
		case 19:
			p19 = settingCharacter;
			break;
		case 20:
			p20 = settingCharacter;
			break;
		case 21:
			p21 = settingCharacter;
			break;
		case 22:
			p22 = settingCharacter;
			break;
		case 23:
			p23 = settingCharacter;
			break;
		case 24:
			p24 = settingCharacter;
			break;
		case 25:
			p25 = settingCharacter;
			break;
		case 26:
			p26 = settingCharacter;
			break;
		case 27:
			p27 = settingCharacter;
			break;
		case 28:
			p28 = settingCharacter;
			break;
		case 29:
			p29 = settingCharacter;
			break;
		case 30:
			p30 = settingCharacter;
			break;
		case 31:
			p31 = settingCharacter;
			break;
		case 32:
			p32 = settingCharacter;
			break;
		case 33:
			p33 = settingCharacter;
			break;
		case 34:
			p34 = settingCharacter;
			break;
		case 35:
			p35 = settingCharacter;
			break;
		case 36:
			p36 = settingCharacter;
			break;
	}
}//end setPieceAt()



//--------------------------------------------------------------------
// Reset the entire board using user entered values.
void resetBoard() {
	string usrInputString = ""; // Each row of the board that the user enters
	
	// Display board resetting instructions
	cout << "You have chosen to reset the board." << endl;
	cout << "Enter 36 characters for the new board." << endl;

	// Resets each of the six rows of the board
	for(int rowOffset = 0; rowOffset <= 30; rowOffset += 6) {
		cin >> usrInputString; // Reads in the user input string
		
		// Resets each character in the row
		for(int pieceInRow = 1; pieceInRow <= 6; ++pieceInRow) {
			setPieceAt(rowOffset + pieceInRow, usrInputString.at(pieceInRow - 1));
		}
	}
}//end resetBoard()



//--------------------------------------------------------------------
// Takes in the character corresponding to the car to move, the direction to move in
// (l for left, r for right, u for up, d for down), and the number of spaces to move,
// then moves the car by updating board pieces.
void moveCar(char carChar, char moveDirection, char moveDistanceChar) {
	int carEnd1Loc; // Location (index 1 through 36) of the upper leftmost end of the car
	int carEnd2Loc; // Location (index 1 through 36) of the bottom rightmost end of the car
	int maxMoveDistance; // The farthest the car can move in the specified direction before hitting another car or the wall
	int carLength; // The length of the car
	int moveDistance; // The move distance input as an integer
	
	// Turns the character move distance input into an integer
	switch(moveDistanceChar) {
	case '0':
	    moveDistance = 0;
	    break;
	case '1':
		moveDistance = 1;
		break;
	case '2':
		moveDistance = 2;
		break;
	case '3':
		moveDistance = 3;
		break;
	case '4':
		moveDistance = 4;
		break;
	case '5':
		moveDistance = 5;
		break;
	default: // Moves cannot be longer than 5, so any move longer than 5 is equivelent to a move of 6
	    moveDistance = 6;
	}

	// Makes sure the car character and direction character are upper case
	char upperCarChar = toupper(carChar);
	char upperMoveDirection = toupper(moveDirection);

	// The following for loop goes through all 36 pieces in order until it finds the upper leftmost end of the car
	for(int checkPiece = 1; checkPiece <= 36; ++checkPiece) {
		if(getPieceAt(checkPiece) == upperCarChar) {
			carEnd1Loc = checkPiece;
			break;
		}
	}

	carLength = 0; // carLength is initialized to zero so that it can count the car pieces as it encounters them by incrementing
	
	// If the move is up or down, the car must be vertical, and if the move is left or right, the car must be horizontal. This matters
	// for finding the length of the car and the lower leftmost end of the car
	switch(upperMoveDirection) {
		case 'U':
		case 'D':
		    // When the car is vertical, the following loop checks each piece below the upper end of the car until it finds the end of the car
			for(int checkPiece = carEnd1Loc; checkPiece <= 36; checkPiece += 6) {
				if(getPieceAt(checkPiece) == upperCarChar) {
					carEnd2Loc = checkPiece; // The updated lower end of the car
					++carLength; // The updated car length
				}
				else {
				    break; // End of car found
				}
			}
			break;
		case 'L':
		case 'R':
		    // When the car is horizontal, the following loop checks each piece to the right of the leftmost end of the car until it finds the end of the car
			carEnd2Loc = carEnd1Loc;
			for(int checkPiece = carEnd1Loc; checkPiece <= carEnd1Loc - ((carEnd1Loc - 1) % 6) + 5; ++checkPiece) {
				if(getPieceAt(checkPiece) == upperCarChar) {
					carEnd2Loc = checkPiece; // The updated rightmost end of the car
					++carLength; // The updated car length
				}
				else {
				    break; // End of car found
				}
			}
			break;
	}

	maxMoveDistance = 0; // maxMoveDistance is initialized to zero so that it can count the empty pieces in the move direction as it encounters them by incrementing
	
	// In the following switch statment, the maximum distance the car can move is found for whichever of the four directions the move is in
	switch(upperMoveDirection) {
	    case 'U':
	        // When the move is upwards, the following for loop checks how many empty spaces there are above the car
		    for(int checkPiece = carEnd1Loc - 6; checkPiece >= 1; checkPiece -= 6) {
			    if (getPieceAt(checkPiece) == '.') {
				    ++maxMoveDistance;
			    }
			    else {
				    break;
			    }
		    }
		    break;
	    case 'D':
	        // When the move is upwards, the following for loop checks how many empty spaces there are below the car
		    for(int checkPiece = carEnd2Loc + 6; checkPiece <= 36; checkPiece += 6) {
			    if (getPieceAt(checkPiece) == '.') {
				    ++maxMoveDistance;
			    }
			    else {
				    break;
			    }
		    }
		    break;
	    case 'L':
	        // When the move is leftwards, the following for loop checks how many empty spaces there are to the left of the car
		    for(int checkPiece = carEnd1Loc - 1; checkPiece % 6 != 0; --checkPiece) {
			    if (getPieceAt(checkPiece) == '.') {
				    ++maxMoveDistance;
			    }
			    else {
				    break;
			    }
		    }
		    break;
	    case 'R':
	        // When the move is rightwards, the following for loop checks how many empty spaces there are to the right of the car
		    for(int checkPiece = carEnd2Loc + 1; checkPiece % 6 != 1; ++checkPiece) {
			    if (getPieceAt(checkPiece) == '.') {
				    ++maxMoveDistance;
			    }
			    else {
				    break;
			    }
		    }
		    break;
	}

	// If the requested move distance is greater than the maximum allowed, the move distance is reduced to the maximum allowed and a message is printed
	if(maxMoveDistance < moveDistance) {
		cout << "*** Move was attempted, but couldn't be completed. ***" << endl;
		moveDistance = maxMoveDistance;
	}
	
	// Moves the car in the correct direction
	switch(upperMoveDirection) {
	case 'U':
	    // Clears each piece of the car from bottom to top
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd2Loc - 6 * pieceToMove, '.');
		}
		
		// Remakes each piece of the car from bottom to top in the car's new position
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd2Loc - 6 * pieceToMove - 6 * moveDistance, upperCarChar);
		}
		break;
	case 'D':
	    // Clears each piece of the car from top to bottom
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd1Loc + 6 * pieceToMove, '.');
		}
		
		// Remakes each piece of the car from top to bottom in the car's new position
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd1Loc + 6 * pieceToMove + 6 * moveDistance, upperCarChar);
		}
		break;
	case 'L':
	    // Clears each piece of the car from right to left
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd2Loc - pieceToMove, '.');
		}
		
		// Remakes each piece of the car from right to left in the car's new position
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd2Loc - pieceToMove - moveDistance, upperCarChar);
		}
		break;
	case 'R':
	    // Clears each piece of the car from left to right
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd1Loc + pieceToMove, '.');
		}
		
		// Remakes each piece of the car from left to right in the car's new position
		for(int pieceToMove = 0; pieceToMove < carLength; ++pieceToMove) {
			setPieceAt(carEnd1Loc + pieceToMove + moveDistance, upperCarChar);
		}
		break;
	}
}//end moveCar()



//--------------------------------------------------------------------
/*  Board and corresponding global variable values are:
           - - - - - - - -
         1 | G G . . . Y |
         7 | P . . B . Y |
        13 | P R R B . Y >
        19 | P . . B . . |
        25 | O . . . T T |
        31 | O . F F F . |
           - - - - - - - -
 */
int main()
{
    string usrInputString = ""; // The users input string every turn
    int moveNumber = 1; // The current move number

    displayIDInfo();        // Display ID info
    displayInstructions();  // Display game instructions

    // Set the board values
     p1='G'; p2='G'; p3='.'; p4='.'; p5='.'; p6='Y';
     p7='P'; p8='.'; p9='.';p10='B';p11='.';p12='Y';
    p13='P';p14='R';p15='R';p16='B';p17='.';p18='Y';
    p19='P';p20='.';p21='.';p22='B';p23='.';p24='.';
    p25='O';p26='.';p27='.';p28='.';p29='T';p30='T';
    p31='O';p32='.';p33='F';p34='F';p35='F';p36='.';

    // Display the initial board
    displayBoard();

    // Loops through the moves until the user quits or wins
    while (true) {
    	cout << moveNumber << ". Your move -> "; // Displays move number and prompt
    	cin >> usrInputString; // Takes in the user's move\
    	
    	// Quits game if user inputs x
    	if (usrInputString == "x") {
    		break;
    	}
    	
    	// Resets board if user inputs -
    	if (usrInputString == "-") {
    		resetBoard();
    		moveNumber = 1; // Move number resets to 1
    		displayBoard(); // Displays new board
    		continue; // Starts first turn on new board
    	}
    	
    	++moveNumber; // Increments the move number
    	
    	moveCar(usrInputString.at(0), usrInputString.at(1), usrInputString.at(2)); // Carries out the user's move
    	
    	displayBoard(); // Displays the board after the move
    	
    	// Checks if the user has won
    	if (p18 == 'R') {
    		cout << endl << "*** Congratulations, you did it! ***" << endl;
    		break; // Quits game if win found
    	}
    }

    cout << endl;
    cout << "Thank you for playing.  Exiting..." << endl;

    return 0;
}//end main()



