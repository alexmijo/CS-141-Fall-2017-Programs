/*---------------------------------------------------------
 *
 *     Graphical version of the game RushHour, where the
 *     object is to move the vehicles such that the small
 *     car can exit the right of the board. User inputs
 *     moves by clicking on cars or buttons on the board.
 *
 * Note that the error message "User input is not one of the valid color or action items. Please retry."
 * does not appear in this program because with graphical input it is not possible to input a command
 * which is invalid in this way.
 * Note that the error message "Attempt is made to move a piece off the edge of the board. Please retry."
 * does not appear in this program because with graphical input it is not possible to input a command
 * which is invalid in this way.
 *
 * Class: Program #4 for CS 141, Fall 2017.
 * Lab: 2pm Tuesdays,
 * TA: Abhinav Kumar
 * System: Windows 10, Qt Creator
 * Author: Alex Moreno
 *---------------------------------------------------------
 */

#include <iostream>   // For textual input and output
#include "gobjects.h" // For all graphics objects
#include "gevents.h"  // Needed for graphical input, to tell where on the board the user clicks
#include <fstream>    // For file input
#include <string>     // For strings and their functions
using namespace std;

// Global constants
const string VALID_COLORS = "RBGMCYOP"; // Used for input checking
const int BOARD_SIZE      = 6; // Length of a side of the board in pieces
// Constants determining letters corresponding to car colors
const char GREEN_CHAR   = 'G';
const char MAGENTA_CHAR = 'M';
const char ORANGE_CHAR  = 'O';
const char PINK_CHAR    = 'P';
const char CYAN_CHAR    = 'C';
const char BLUE_CHAR    = 'B';
const char YELLOW_CHAR  = 'Y';
const char RED_CHAR     = 'R';
// Constants determining letters corresponding to car orientations
const char VERTICAL_CHAR   = 'V';
const char HORIZONTAL_CHAR = 'H';


//---------------------------------------------------------
// Each car on the board is an instance of the Car class
class Car {
public:
    Car(char theColor, char orientation, int rowNum, int colNum, int length, GWindow *pGraphicsWindow); // No default constructor, must supply all 5 parameters in constructor
    ~Car(); // Destructor for the Car class
    bool containsPosition(int xPos, int yPos); // Checks if the car contains the passed in position in the graphics window
    bool move(int rowNum, int colNum, Car *pCars[]); // Attempts to move the car to the passed in position on the board and returns if the move won the game

    // Getter functions
    int getRowNum() {return rowNum;}
    int getColNum() {return colNum;}
    int getLength() {return length;}
    char getOrientation() {return orientation;}
    string getColor() {return color;}

private:
    // Basic car information
    string color;     // The color of the car: 'R' = red, //TODO figure out car color codes
    char orientation; // The orientation of the car: 'V' = vertical, 'H' = horizontal
    int rowNum;       // The column number (1-6 from left to right) of the leftmost part of the car
    int colNum;       // The row number (1-6 from top to bottom) of the uppermost part of the car
    int length;       // The length of the car (2 or 3)

    // Car graphics
    GWindow *pGraphicsWindow; // Pointer to the GWindow in which the car is drawn
    GRect *carRectangle;      // A pointer to the actual rectangle graphically representing the car (a pointer to the GRect because GRect is protected in this context)

    // Helper functions
    void drawCar(); // Draws the car
    int getXPos();  // Returns the x position of the car's rectangle on the graphics window
    int getYPos();  // Returns the y position of the car's rectangle on the graphics window
    void populateBoardArray(char boardPieces[][BOARD_SIZE], Car *pCars[]); // Populates the boardPieces array with 'T's where this car is, 'O's where other cars are, and 'N's where no car is
    bool movePathIsOpen(char boardPieces[][BOARD_SIZE], int moveDistanceInPieces); // Returns true if only if the car won't run into other cars
    void moveXAnimation(int xDist); // Moves the car's rectangle on the x axis the passed in distance in an animation
    void moveYAnimation(int yDist); // Moves the car's rectangle on the y axis the passed in distance in an animation
};


//--------------------------------------------------------------------
// Constructor for the car class, displays the car's rectangle on the passed in graphics window
Car::Car(char colorChar, char orientation, int rowNum, int colNum, int length, GWindow *pGraphicsWindow) {
    // Initializes the Car's private variables using passed in values
    this->orientation = orientation;
    this->rowNum = rowNum;
    this->colNum = colNum;
    this->length = length;
    this->pGraphicsWindow = pGraphicsWindow;

    // Initializes the Car's private variable color depending on the passed in colorChar
    switch(colorChar) {
        case GREEN_CHAR:   color = "GREEN"; break;
        case MAGENTA_CHAR: color = "MAGENTA"; break;
        case ORANGE_CHAR:  color = "ORANGE"; break;
        case PINK_CHAR:    color = "PINK"; break;
        case CYAN_CHAR:    color = "CYAN"; break;
        case BLUE_CHAR:    color = "BLUE"; break;
        case YELLOW_CHAR:  color = "YELLOW"; break;
        case RED_CHAR:     color = "RED"; break;
    }

    drawCar(); // Draws the car on the graphics window
}


//--------------------------------------------------------------------
// Destructor for the car class, removes the car's rectangle from the graphics window in which it resides
Car::~Car() {
    pGraphicsWindow->remove(carRectangle); // Undisplays the car's rectangle
    delete carRectangle; // Frees up the memory that was allocated for the car's rectangle
}


//--------------------------------------------------------------------
// Draws the car's rectangle on the graphics window in which it resides
void Car::drawCar() {
    int yPos; // The actual y coordinate of the car's rectangle
    int xPos; // The actual x coordinate of the car's rectangle
    int width; // The actual length of the car's rectangle
    int height; // The actual height of the car's rectangle

    // Gets the car's rectangle's x and y coordinates
    yPos = getYPos();
    xPos = getXPos();

    // Gets the car's height and width based on it's orientation and length
    if(orientation == VERTICAL_CHAR) {
        // For a vertical car the width is 30 and the height depends on the length
        width = 30; // A piece on the board is 30 by 30
        height = (40 * length) - 10; // The 40 comes from the fact that the car along its length must bridge the 10 pixel gap between adjacent pieces
    }
    else if(orientation == HORIZONTAL_CHAR) {
        // For a horizontal car the height is 30 and the width depends on the length
        height = 30; // A piece on the board is 30 by 30
        width = (40 * length) - 10; // The 40 comes from the fact that the car along its length must bridge the 10 pixel gap between adjacent pieces
    }

    // Draws the car's rectangle
    carRectangle = new GRect(xPos, yPos, width, height);
    carRectangle->setFilled(true);
    carRectangle->setColor(color);
    pGraphicsWindow->add(carRectangle);
}


//--------------------------------------------------------------------
// Returns the x position of the car's rectangle in the graphics window
int Car::getXPos() {
    int xPos; // The actual x position of the car's rectangle on the graphics window
    xPos = 30; // Initializes the x position to the x position of the leftmost column (30)
    xPos += 40 * (colNum - 1); // Shifts the x position rightward by the width of a column (40) for each column the car is over from the first column
    return xPos; // Returns the correct x position
}


//--------------------------------------------------------------------
// Returns the y position of the car's rectangle in the graphics window
int Car::getYPos() {
    int yPos; // The actual y position of the car's rectangle on the graphics window
    yPos = 30; // Initializes the y position to the y position of the uppermost row (30)
    yPos += 40 * (rowNum - 1); // Shifts the y position downwardward by the height of a row (40) for each row the car is down from the first row
    return yPos; // Returns the correct y position
}


//--------------------------------------------------------------------
// Returns true only if the position in the car's graphics window specified by the passed in x and y coordinates is within the car's rectangle
bool Car::containsPosition(int xPos, int yPos) {
    return carRectangle->contains(xPos, yPos);
}


//--------------------------------------------------------------------
// Populates the passed in boardPieces array with 'T's where this car is, 'O's where other cars are, and 'N's where no car is
// by using the passed in array of pointers to the cars on the board
void Car::populateBoardArray(char boardPieces[][BOARD_SIZE], Car *pCars[]) {
    // Initially records all pieces on the board as having no car on them
    for(int i = 0; i < BOARD_SIZE; ++i) {
        for(int j = 0; j < BOARD_SIZE; ++j) {
            boardPieces[i][j] = 'N';
        }
    }

    // Then overwrites all pieces which have any car on them as having other cars
    for(int currCar = 0; currCar < 8; ++currCar) {
        // Checks if the car exists
        if(pCars[currCar] != NULL) {
            // Iterates through the car's pieces along its length
            for(int pieceNum = 0; pieceNum < pCars[currCar]->getLength(); ++pieceNum) {
                if(pCars[currCar]->getOrientation() == HORIZONTAL_CHAR) {
                    boardPieces[pCars[currCar]->getRowNum() - 1][pCars[currCar]->getColNum() - 1 + pieceNum] = 'O'; // Records a piece of a horizontal car
                }
                else if(pCars[currCar]->getOrientation() == VERTICAL_CHAR) {
                    boardPieces[pCars[currCar]->getRowNum() - 1 + pieceNum][pCars[currCar]->getColNum() - 1] = 'O'; // Records a piece of a vertical car
                }
            }
        }
    }

    // Finally overwrites the pieces on which this car resides as being where this car is
    // Iterates through this car's pieces along its length
    for(int pieceNum = 0; pieceNum < length; ++pieceNum) {
        if(orientation == HORIZONTAL_CHAR) {
            boardPieces[rowNum - 1][colNum - 1 + pieceNum] = 'T'; // Records a piece of this car if it's horizontal
        }
        else if(orientation == VERTICAL_CHAR) {
            boardPieces[rowNum - 1 + pieceNum][colNum - 1] = 'T'; // Records a piece of this car if it's vertical
        }
    }
}


//--------------------------------------------------------------------
// Returns true if this car can move the passed in distance (negative for moving up or left) without hitting
// other cars as layed out in the passed in array of all the pieces on the board. Otherwise returns false.
bool Car::movePathIsOpen(char boardPieces[][BOARD_SIZE], int moveDistanceInPieces) {
    // If the move distance is positive, must iterate forward through the pieces
    if(moveDistanceInPieces > 0) {
        for(int piecesMoved = 1; piecesMoved <= moveDistanceInPieces; ++piecesMoved) {
            // Checks if the car has run into another car after moving piecesMoved pieces if it's horizontal
            // Note the - 2 is necessary because boardPieces is 0 indexed (requiring a - 1) and the right end of the car is offset by length - 1 from
            // the left end of the car, not by length (requiring a second - 1)
            if(orientation == HORIZONTAL_CHAR && boardPieces[rowNum - 1][colNum + length - 2 + piecesMoved] == 'O') {
                return false;
            }
            // Checks if the car has run into another car after moving piecesMoved pieces if it's vertical
            // Note the - 2 is necessary because boardPieces is 0 indexed (requiring a - 1) and the bottom end of the car is offset by length - 1 from
            // the bottom end of the car, not by length (requiring a second - 1)
            else if(orientation == VERTICAL_CHAR && boardPieces[rowNum + length - 2 + piecesMoved][colNum - 1] == 'O') {
                return false;
            }
        }
    }
    // If the move distance is negative, must iterate backwards through the pieces
    else if (moveDistanceInPieces < 0) {
        for(int piecesMoved = -1; piecesMoved >= moveDistanceInPieces; --piecesMoved) {
            // Checks if the car has run into another car after moving piecesMoved pieces if it's horizontal
            if(orientation == HORIZONTAL_CHAR && boardPieces[rowNum - 1][colNum - 1 + piecesMoved] == 'O') {
                return false;
            }
            // Checks if the car has run into another car after moving piecesMoved pieces if it's vertical
            else if(orientation == VERTICAL_CHAR && boardPieces[rowNum - 1 + piecesMoved][colNum - 1] == 'O') {
                return false;
            }
        }
    }
    return true; // Returns true if there are no other cars in the way of the move
}


//--------------------------------------------------------------------
// Moves the car's rectangle along the x axis the passed in distance in an animation
void Car::moveXAnimation(int xDist) {
    // Makes xDist small moves with 1 millisecond pauses between each move
    if(xDist > 0) {
        for(int i = 0; i < xDist; ++i) {
            carRectangle->move(1, 0); // Moves the rectangle 1 unit to the right
            pause(1); // Waits one millisecond
        }
    }
    else {
        for(int i = 0; i > xDist; --i) {
            carRectangle->move(-1, 0); // Moves the rectangle 1 unit to the left
            pause(1); // Waits one millisecond
        }
    }
}


//--------------------------------------------------------------------
// Moves the car's rectangle along the y axis the passed in distance in an animation
void Car::moveYAnimation(int yDist) {
    // Makes xDist small moves with 1 millisecond pauses between each move
    if(yDist > 0) {
        for(int i = 0; i < yDist; ++i) {
            carRectangle->move(0, 1); // Moves the rectangle 1 unit down
            pause(1); // Waits one millisecond
        }
    }
    else {
        for(int i = 0; i > yDist; --i) {
            carRectangle->move(0, -1); // Moves the rectangle 1 unit up
            pause(1); // Waits one millisecond
        }
    }
}


//--------------------------------------------------------------------
// Attempts to move the car to the passed in position on the board.
// Displays error message if it can't because the cars pointed to in the passed in array of car pointers being in the way,
// because the move is not in the right direction, or because the car was already on the destination. Returns true only if the move caused the game to be won
bool Car::move(int rowNum, int colNum, Car *pCars[]) {
    char boardPieces[BOARD_SIZE][BOARD_SIZE]; // Records what pieces on the board have this car or other cars on them
    int moveDistanceInPieces; // The distance (and direction, because it can be negative) the car is moving in pieces

    // Prints error message and returns false if a vertical car attempts to move to a position outside of its column
    if(orientation == VERTICAL_CHAR && colNum != this->colNum) {
        cout << "Attempt was made to move a car in the wrong direction. Please retry." << endl;
        return false;
    }
    // Prints error message and returns false if a horizontal car attempts to move to a position outside of its row
    else if(orientation == HORIZONTAL_CHAR && rowNum != this->rowNum) {
        cout << "Attempt was made to move a car in the wrong direction. Please retry." << endl;
        return false;
    }

    populateBoardArray(boardPieces, pCars); // Populates the boardPieces array with 'T's where this car is, 'O's where other cars are, and 'N's where no car is

    // Prints error message and returns false if the car is already on the piece it's trying to move to
    if(boardPieces[rowNum - 1][colNum - 1] == 'T') {
        cout << "The car is already on that piece. Please retry." << endl;
        return false;
    }

    if(orientation == VERTICAL_CHAR) {
        // Branch taken if the car is moving downward, in which case the length of the car must be subtracted from the move distance
        if(rowNum > this->rowNum) {
            moveDistanceInPieces = rowNum - this->rowNum + 1 - length; // Gets the minimum distance in pieces this car must move to get to the destination piece if its vertical and moving down
        }
        // If the car is moving upward, the length of the car musn't be subracted from the move distance
        else {
            moveDistanceInPieces = rowNum - this->rowNum; // Gets the minimum distance in pieces this car must move to get to the destination piece if its vertical and moving up
        }
    }
    else if(orientation == HORIZONTAL_CHAR) {
        // Branch taken if the car is moving rightward, in which case the length of the car must be subtracted from the move distance
        if(colNum > this->colNum) {
            moveDistanceInPieces = colNum - this->colNum + 1 - length; // Gets the minimum distance in pieces this car must move to get to the destination piece if its horizontal and moving right
        }
        // If the car is moving leftward, the length of the car musn't be subtracted from the move distance
        else {
            moveDistanceInPieces = colNum - this->colNum; // Gets the minimum distance in pieces this car must move to get to the destination piece if its horizontal and moving left
        }
    }

    // Checks if the car will move into other cars, if so prints an error message and returns false
    if(!movePathIsOpen(boardPieces, moveDistanceInPieces)) {
        cout << "Attempt is made to move a piece on top of another piece. Please retry." << endl;
        return false;
    }

    // If the function has gotten this far without returning, the move is valid, so it makes the move and returns true if the move won the game
    if(orientation == HORIZONTAL_CHAR) {
        moveXAnimation(moveDistanceInPieces * 40); // Each piece moved is 40 moved on the graphics window
        this->colNum += moveDistanceInPieces; // Updates colNum for the new position
        if(colNum == 6 && rowNum == 3) {return true;} // A horizontal piece moving to row 3 column 6 wins the game
    }
    else if(orientation == VERTICAL_CHAR) {
        moveYAnimation(moveDistanceInPieces * 40); // Each piece moved is 40 moved on the graphics window
        this->rowNum += moveDistanceInPieces; // Updates rowNum for the new position
        return false; // The game is not won upon moving a vertical piece
    }
    return false; // Need to make sure the function returns a value even if orientation isn't a valid character
}


//--------------------------------------------------------------------
// Displays required author ID info
void displayIDInfo()
{
    cout << "Author:  Alex Moreno" << endl;
    cout << "Lab:     Tuesday 2pm" << endl;
    cout << "TA:    Abhinav Kumar" << endl;
    cout << "Program: #4, Graphical Traffic" << endl;
    cout << endl;
}


//--------------------------------------------------------------------
// Display the game's instructions
void displayInstructions()
{
    cout << "Welcome to the traffic game!                             " << endl
         << "                                                         " << endl
         << "Move the vehicles so that the Red car can exit           " << endl
         << "the board to the right. To move a car, first click on    " << endl
         << "the car you'd like to move, then click on the            " << endl
         << "square you'd like the car to move to.                    " << endl
         << "Click the Reset button to enter a new board              " << endl
         << "configuration, or click the Exit button to quit the game." << endl << endl;
}


//---------------------------------------------------------
// Draws a rectangle that does not already exist given pointers to it and the graphics window it
// is drawn in and its size, location and color
void drawRectangle(GRect *pRectangle, GWindow *pGraphicsWindow, int xPos, int yPos, int width, int height, string color) {
    pRectangle = new GRect(xPos, yPos, width, height); // Makes a new rectangle (at the pointer) with the correct dimensions and location
    pRectangle->setFilled(true);
    pRectangle->setColor(color);
    pGraphicsWindow->add(pRectangle); // Draws the correctly formatted rectangle in the graphics window
}


//--------------------------------------------------------------------
// Uses the file "board.txt" to set up the initial board configuration by populating the passed in cars array, drawing the cars on the passed in graphics window
void setupBoardFromFile(Car *pCars[], GWindow *pGraphicsWindow) {
    ifstream inputFileStream;  // declare the input file stream
    int numCars; // The number of cars on the board
    // Data from the input file used to set up initial board
    char colorChar, orientation;
    int colNum, rowNum, length;

    // open input file and verify
    inputFileStream.open("board.txt");
    if(!inputFileStream.is_open()) {
        cout << "Could not find input file board.txt. Exiting..." << endl;
        exit(-1); // Quits the program and returns -1 from main indicating there was an error if the file couldn't be found
    }

    inputFileStream >> numCars;  // Gets the number of cars

    // Uses the data in the input file to set up the initial board
    for(int i = 0; i < numCars; ++i) {
        inputFileStream >> colorChar >> orientation >> rowNum >> colNum >> length; // Reads in a line of the file
        pCars[i] = new Car(colorChar, orientation, rowNum, colNum, length, pGraphicsWindow); // Creates a car, which also draws it on the board
    }

    // If numCars is less than 8, sets the remaining uninitialized pointers in numCars to NULL so that other parts of the program can know they don't point to cars
    for(int i = numCars; i < 8; ++i) {
        pCars[i] = NULL;
    }
}


//--------------------------------------------------------------------
// Resets the entire board using user entered values and displays the new
// board on the passed in graphics window by repopulating the passed in array of car pointers
void resetBoard(Car *pCars[], GWindow *pGraphicsWindow) {
    int numCars; // The new number of cars on the board
    // Data from the user used to set up the new cars on the board
    char colorChar, orientation;
    int colNum, rowNum, length;

    // Frees up the memory that was allocated to the cars pointed to in pCars and undisplays those cars's rectangles
    for(int i = 0; i < 8; ++i) {
        // Deletes a car pointed to by a pointer in pCars, only if that pointer does actually point to a car
        if(pCars[i] != NULL) {
            delete pCars[i];
        }
    }

    // Gets the new number of cars from the user
    cout << "You've clicked on the reset button." << endl << "Enter new number of cars (max 8): ";
    cin >> numCars;

    // Creates and displays all of the new cars by getting their info from the user
    for(int i = 0; i < numCars; ++i) {
        // Gets the car's color letter from the user
        cout << "Color letters: 'R' = red, 'B' = blue, 'Y' = yellow, 'G' = green, 'M' = magenta, 'O' = orange," << endl
             << "'P' = pink, 'C' = cyan" << endl
             << "Enter the color letter for car #" << i + 1 << ": ";
        cin >> colorChar;

        // Gets the car's orientation from the user
        cout << "Orientation letters: 'V' = vertical, 'H' = horizontal" << endl
             << "Enter the orientation letter for car #" << i + 1 << ": ";
        cin >> orientation;

        // Gets the car's row number from the user
        cout << "Enter the row number of the uppermost piece of car #" << i + 1 << ": ";
        cin >> rowNum;

        // Gets the car's column number from the user
        cout << "Enter the column number of the uppermost piece of car #" << i + 1 << ": ";
        cin >> colNum;

        // Gets the car's length from the user
        cout << "Enter the length (2 or 3) of car #" << i + 1 << ": ";
        cin >> length;

        pCars[i] = new Car(colorChar, orientation, rowNum, colNum, length, pGraphicsWindow); // Creates the new car and displays it
    }

    // If numCars is less than 8, sets the remaining uninitialized pointers in numCars to NULL so that other parts of the program can know they don't point to cars
    for(int i = numCars; i < 8; ++i) {
        pCars[i] = NULL;
    }
}


//--------------------------------------------------------------------
// Draws the background, lines and buttons of the empty board on the passed in graphics window
void drawEmptyBoard(GWindow *pGraphicsWindow) {
    GRect *rectangle = NULL; // The GRect that is used to draw all the rectangles in this function
    GLabel *exitButtonText  = new GLabel("Exit", 35, 298);  // The text that will be overlayed on the exit button
    GLabel *resetButtonText = new GLabel("Reset", 100, 298); // The text that will be overlayed on the reset button

    // Used for drawing the lines between pieces
    int xPos;
    int yPos;
    int width;
    int height;

    // Draws the exit button
    drawRectangle(rectangle, pGraphicsWindow, 20, 285, 50, 15, "RED");
    pGraphicsWindow->add(exitButtonText);

    // Draws the reset button
    drawRectangle(rectangle, pGraphicsWindow, 90, 285, 50, 15, "RED");
    pGraphicsWindow->add(resetButtonText);

    // Draws the outline around the board by overlaying two differently sized squares
    drawRectangle(rectangle, pGraphicsWindow, 10, 10, 270, 270, "DARKGRAY"); // Draws the board outline
    drawRectangle(rectangle, pGraphicsWindow, 20, 20, 250, 250, "GRAY"); // Draws the inner playing surface

    drawRectangle(rectangle, pGraphicsWindow, 270, 100, 10, 50, "LIGHTGRAY"); // Adds an opening on the right side of the outline for a piece to exit to win the game

    // Draws the vertical lines
    xPos = 20;
    yPos = 20;
    width = 10;
    height = 250;
    // Iterates through the vertical lines
    for(int i = 0; i < BOARD_SIZE + 1; ++i) {
        drawRectangle(rectangle, pGraphicsWindow, xPos, yPos, width, height, "LIGHTGRAY"); // Draws the current line
        xPos += 40; // Changes the x position so that the next line is shifted rightward
    }

    // Draws the horizontal lines
    xPos = 20; // Resets the x position which was changed in making the vertical lines, y position was unchanged (still 20)
    // Width and height are swapped so that the lines are now horizontal instead of vertical
    width = 250;
    height = 10;
    // Iterates through the horizontal lines
    for(int i = 0; i < BOARD_SIZE + 1; ++i) {
        drawRectangle(rectangle, pGraphicsWindow, xPos, yPos, width, height, "LIGHTGRAY"); // Draws the current line
        yPos += 40; // Changes the y position so that the next line is shifted downward
    }
}


//--------------------------------------------------------------------
// Returns true if the position on the graphics window specified by the
// passed in x and y coordinates is within the exit button, otherwise false
bool positionInExitButton(int xPos, int yPos) {
    return (xPos > 20 && xPos <= 70) && (yPos > 285 && yPos <= 300);
}


//--------------------------------------------------------------------
// Returns true if the position on the graphics window specified by the
// passed in x and y coordinates is within the reset button, otherwise false
bool positionInResetButton(int xPos, int yPos) {
    return (xPos > 90 && xPos <= 140) && (yPos > 285 && yPos <= 300);
}


//--------------------------------------------------------------------
// Returns the row of the board in which the position specified by the passed in
// y coordinate resides. If the position isn't in any row of the board, returns -1
int positionToRow(int yPos) {
    int rowNum = -1; // The row in which the position resides, remains -1 if the position isn't in any row

    // Checks for each of the BOARD_SIZE rows if the position is in that row
    for(int i = 0; i < BOARD_SIZE; ++i) {
        // The first row spans y coordinates 31 to 60, and subsequent rows are offset by 40
        if(yPos > (30 + 40 * i) && yPos <= (60 + 40 * i)) {
            rowNum = i + 1; // Sets rowNum to the row in which the y coordinate resides
        }
    }

    return rowNum;
}


//--------------------------------------------------------------------
// Returns the column of the board in which the position specified by the passed in
// x coordinate resides. If the position isn't in any column of the board, returns -1
int positionToCol(int xPos) {
    int colNum = -1; // The column in which the position resides, remains -1 if the position isn't in any column

    // Checks for each of the BOARD_SIZE columns if the position is in that column
    for(int i = 0; i < BOARD_SIZE; ++i) {
        // The first column spans x coordinates 31 to 60, and subsequent columns are offset by 40
        if(xPos > (30 + 40 * i) && xPos <= (60 + 40 * i)) {
            colNum = i + 1; // Sets colNum to the column in which the x coordinate resides
        }
    }

    return colNum;
}


//--------------------------------------------------------------------
// Returns the index of the car in the passed in array of car pointers which contains the location specified by the passed in x and y coordinates.
// If there is no such car, returns -1
int whichCar(int xPos, int yPos, Car *pCars[]) {
    int carIndex; // The index of the car in pCars which contains the location specified by the passed in x and y coordinates

    carIndex = -1; // Remains -1 if no car that contains the specified position is found
    for(int i = 0; i < 8; ++i) {
        // Checks if the car exists and contains the specified position
        // Note that the second statement in the if condition won't execute if the car doesn't exist due to short circuit evaluation of the && operator
        if(pCars[i] != NULL && pCars[i]->containsPosition(xPos, yPos)) {
            carIndex = i;
        }
    }

    return carIndex;
}


//--------------------------------------------------------------------
int main()
{
    Car *pCars[8];       // Pointers to all of the cars on the board
    int userClickXPos;   // The x position of where the user clicks in the graphics window
    int userClickYPos;   // The y position of where the user clicks in the graphics window
    int userClickRow;    // The row of the board in which the user clicked
    int userClickCol;    // The column of the board in which the user clicked
    int clickedCarIndex; // Index in pCars of the car that the user clicked on
    bool userHasWon = false; // True once the game has been won

    displayIDInfo();        // Display ID info
    displayInstructions();  // Display game instructions

    // Setup and display the initial board
    GWindow graphicsWindow(300,300);                                   // Makes a 300 by 300 graphics window to display the board in
    graphicsWindow.setWindowTitle("CS 141: Prog 4 Graphical Traffic"); // Sets the window title
    GWindow *pGraphicsWindow = &graphicsWindow;                        // A pointer to the graphics window is needed so that the graphics window can be used in functions
    drawEmptyBoard(pGraphicsWindow);                                   // Draws the board with no cars on it
    setupBoardFromFile(pCars, pGraphicsWindow);                        // Creates the cars and draws the initial layout of cars on the board

    // Loops through the moves until the user quits or wins
    while (true) {
        GMouseEvent firstClick = waitForClick(); // Halts program execution until the user clicks in the graphical window
        userClickXPos = firstClick.getX();       // Gets the x position of where the user clicked
        userClickYPos = firstClick.getY();       // Gets the y position of where the user clicked

        if(positionInExitButton(userClickXPos, userClickYPos)) {break;} // Breaks from the while loop if the user clicked on the exit button

        // Resets the board and then goes back to start of while loop if the user clicked on the reset button
        if(positionInResetButton(userClickXPos, userClickYPos)) {
            resetBoard(pCars, pGraphicsWindow);
            continue;
        }

        clickedCarIndex = whichCar(userClickXPos, userClickYPos, pCars); // Gets the index of car that the user clicked on (-1 if no such car)
        if(clickedCarIndex == -1) {continue;} // Returns to the beginning of the while loop if the user didn't click on a car the board
        cout << "You've selected the " << pCars[clickedCarIndex]->getColor() << " car for moving." << endl;

        GMouseEvent secondClick = waitForClick(); // Halts program execution until the user clicks in the graphical window
        userClickXPos = secondClick.getX();       // Gets the x position of where the user clicked
        userClickYPos = secondClick.getY();       // Gets the y position of where the user clicked

        if(positionInExitButton(userClickXPos, userClickYPos)) {break;} // Breaks from the while loop if the user clicked on the exit button

        // Resets the board and then goes back to start of while loop if the user clicked on the reset button
        if(positionInResetButton(userClickXPos, userClickYPos)) {
            resetBoard(pCars, pGraphicsWindow);
            continue;
        }

        userClickRow = positionToRow(userClickYPos); // Gets the row of the board that the user clicked in (-1 if not in any row)
        userClickCol = positionToCol(userClickXPos); // Gets the column of the board that the user clicked in (-1 if not in any column)
        if(userClickRow == -1 || userClickCol == -1) {continue;} // Returns to the beginning of the while loop if the user didn't click on a piece on the board

        // Attempts to move the car specified by the first click to the spot specified by the second click
        userHasWon = pCars[clickedCarIndex]->move(userClickRow, userClickCol, pCars); // Returns true only if the move caused the game to be won

        // Prints out a message and exits the while loop if the game has been won
        if(userHasWon) {
            cout << "Congratulations, you won!" << endl;
            break;
        }
    }

    cout << "Thank you for playing.  Exiting..." << endl;
    pause(1000); // Pauses for a second so that the player can see the win message and/or the exiting message

    // Closes the graphics window
    graphicsWindow.requestFocus();
    graphicsWindow.close();

    exitGraphics(); // Closes the console
    return 0;
}
