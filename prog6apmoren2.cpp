/*
 * A graphical presentation of points with which the user can interact to create a Steiner Tree.
 * Points can be added, and lines can be made to connect the points. The sum of the lengths of
 * all lines is displayed and compared to a somewhat optimally short solution.
 *
 * Class: CS141 Fall 2017
 * Lab: Tuesday 2pm
 * TA: Abhinav Kumar
 * System: Windows 10, Qt Creator 4.4.1
 * Author: Alex Moreno
 */

#include <iostream>
#include "gobjects.h"  // for all graphics objects
#include "gevents.h"   // for mouse events
#include <fstream>     // For file input
#include <cmath>
#include <ctime>
#include <vector>
#include <cmath>       // For using the pythagorean theorum to determine the length of a line
using namespace std;

// Global constants
const int PointRadius = 7;

// Create a graphics window, which will be used to draw graphical shapes.
// This can be a global variable, but you may not have any other global variables.
GWindow graphicsWindow;


//------------------------------------------------------------------------------------------
// Each point on the graphics window is of the type MyPoint. Used instead of the default QT creator
// Point class because it can store the actual circle on the graphics window and all corresponding attributes
// instead of just x and y coordinates.
class MyPoint {
public:
    MyPoint() {} // A default constructor which doesn't do anything
    MyPoint(int x, int y, string color, int radius, bool visibility); // A constructor that recieves all the values, making the correct MyPoint object
    int getX() {return x;} // Returns the x coordinate of the point
    int getY() {return y;} // Returns the y coordinate of the point
private:
    int x;            // The x coordinate of the center of the point
    int y;            // The y coordinate of the center of the point
    string color;     // The color of the point
    int radius;       // The radius of the point
    bool visibility;  // True when the point is visible on the graphics window
    GOval *theCircle; // The object for the circle that appears on the graphics window
};


//------------------------------------------------------------------------------------------
// This is the constructor for the MyPoint class. Takes in the various attributes of the point
// and creates a point and puts it on the graphics window by creating a new GOval.
MyPoint::MyPoint(int x, int y, string color, int radius, bool visibility) {
    // Sets the point's private variables to the passed in values
    this->x = x;
    this->y = y;
    this->color = color;
    this->radius = radius;
    this->visibility = visibility;

    // Makes a new circle to be drawn
    theCircle = new GOval(x - radius, y - radius, radius * 2, radius * 2); // The two 'radius * 2's are for the circle's height and width

    // Sets the circle to filled with the passed in color
    theCircle->setFilled(true);
    theCircle->setColor(color);

    // Adds the circle to the graphics window if it is supposed to be visible
    if (visibility) {
        graphicsWindow.add(theCircle);
    }
}


//------------------------------------------------------------------------------------------
// Each button on the graphics window is an instance of the class MyButton. Allows to make a button
// all at once, instead of making a rectangle and then adding the label over it.
class MyButton {
public:
    MyButton() {} // Default constructor which doesn't do anything
    MyButton(int x, int y, int width, int height, string color, string text); // A constructor that recieves all the values, making the correct MyButton object

    // Returns true if the passed in coordinates are within the button's rectangle, otherwise false
    bool contains(int x, int y) {
        return theRectangle->contains(x, y);
    }
private:
    int x;               // The x coordinate of the rectangle
    int y;               // The y coordinate of the rectangle
    int width;           // The width of the rectangle
    int height;          // The height of the rectangle
    string color;        // The color of the rectangle
    GRect *theRectangle; // Pointer to the rectangle object
    GLabel *theLabel;    // Pointer to the label object on the button
    string text;         // The text on the button
};


//------------------------------------------------------------------------------------------
// This is a constructor for the MyButton class. Takes in the various attributes of the button
// and creates a button and puts it on the graphics window by creating a new GRect and GLabel.
MyButton::MyButton(int x, int y, int width, int height, string color, string text) {
    // Sets the button's private variables to the passed in values
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
    this->text = text;

    // Makes a new rectangle and a new label
    theRectangle = new GRect(x, y, width, height);
    theLabel = new GLabel(text);

    theRectangle->setColor(color); // Sets the color of the rectangle
    theRectangle->setFilled(true); // Sets the Filled property of the rectangle to true

    // Find size of text, to determine placement relative to enclosing rectangle
    int xOffset = (width  - theLabel->getBounds().getWidth()) / 2;
    int yOffset = (height - theLabel->getBounds().getHeight()) + 5;
    theLabel->setLocation(x + xOffset, y + yOffset);

    // Adds the rectangle and the label to the graphics window
    graphicsWindow.add(theRectangle);
    graphicsWindow.add(theLabel);
}


//------------------------------------------------------------------------------------------
// Read in the sets of points information from the data file
// You must make the pointsArray Vector a reference parameter so that read-in values are
// reflected back to the caller.
void readInputFile(vector<MyPoint*> &pointsArray, int &idealScore)
{
    ifstream inputFileStream;  // declare the input file stream

    // Open input file and verify.
    // You must first:
    //    1. Outside of QtCreator go into your project's resources folder, copy readme.txt into data.txt
    //    2. Edit data.txt to have the datafile information as shown on the assignment page
    //    3. Now WITHIN QtCreator right-click on the "res" folder (under "Other files") and
    //       choose "add new" and add the new data.txt file so it shows up in your environment.
    inputFileStream.open("data.txt");
    if(!inputFileStream.is_open()) {
        cout << "Could not find input file data.txt.  Exiting..." << endl;
        exit(-1);
    }

    // Read sets of data from input file.  Each read operation reads the next set of data.
    // The first line in the input file indicates which set to use.
    int whichSetToUse;
    inputFileStream >> whichSetToUse;

    int howManyPoints;    // How many points are in the set of points
    int x, y;             // Store individual point x,y values from input file

    // Read in and discard some number of sets of points before we get the real set we will be using
    for( int i = 0; i < whichSetToUse-1; i++) {
        inputFileStream >> howManyPoints >> idealScore;
        for( int j = 0; j < howManyPoints; j++) {
            inputFileStream >> x >> y;    // Read in the points
        }
    }

    inputFileStream >> howManyPoints >> idealScore;
    pointsArray.resize(howManyPoints); // Allocates space in the vector of MyPoint pointers for the correct number of read in points

    // Read in and store the points information
    for( int i = 0; i < howManyPoints; i++) {
        inputFileStream >> x >> y; // Read in the points
        pointsArray.at(i) = new MyPoint(x, y, "black", PointRadius, true); // Takes in the points information and creates points on the graphics window
    }
}


//------------------------------------------------------------------------------------------
// Allows line endpoints to "click" to the nearest point. Takes in x and y coordinates by reference
// and vector of all points, and changes the passed in x and y coordinates to the center of the nearest point.
void clickToPoint(vector<MyPoint*> pointsArray, int &x, int &y) {
    int closestPointIndex = -1; // The index in the passed in array of points of the point closest to the passed in coordinates
    int distanceToClosestPoint = 1000000000; // Initialized to a high number so that the first point is closer to the coordinates and it gets updated
    int distanceToCurrPoint; // The distance from the current point in an iteration of the for loop to the coordinates

    // Loops through all the points and finds the closest one to the passed in coordinates
    for(int point = 0; point < pointsArray.size(); ++point) {
        // Gets the distance to the point using the pythagorean theorem
        distanceToCurrPoint = sqrt(pow(pointsArray[point]->getX() - x, 2) + pow(pointsArray[point]->getY() - y, 2));
        // If this point is the new closest, updates distanceToClosestPoint and closestPointIndex
        if(distanceToCurrPoint < distanceToClosestPoint) {
            distanceToClosestPoint = distanceToCurrPoint;
            closestPointIndex = point;
        }
    }

    // Changes the passed in coordinates to the center of the closest point
    x = pointsArray[closestPointIndex]->getX();
    y = pointsArray[closestPointIndex]->getY();
}


//------------------------------------------------------------------------------------------
int main()
{
    // Prints out required author information
    cout << "Author: Alex Moreno" << endl;
    cout << "Lab: Tuesday 2pm" << endl;
    cout << "TA: Abhinav Kumar" << endl;
    cout << "Program 6: Steiner Trees" << endl;

    graphicsWindow.setCanvasSize(400, 400);   // Set the size
    graphicsWindow.setWindowTitle("apmoren2 QtCreator Program 6");

    vector<MyPoint*> pointsArray;  // Store all points that are created.
    vector<GLine*> linesArray;     // Store all lines that are created

    int idealScore = 0;        // Precomputed ideal score for each puzzle
    int totalLength = 0;       // Used to display the total length of all the lines on the graphics window
    int currTotalLength = 0;   // Used to display the total length when a line is in the middle of being drawn

    char message[81]; // Used to construct message to be displayed at top of window

    // Get the set of data to use from the input file
    readInputFile(pointsArray, idealScore);

    // Create the Control and message components.
    // First create the messages label at the top of the window
    GLabel *messagesLabel = new GLabel();
    messagesLabel->setText("Find the minimal spanning tree, using additional points.");
    messagesLabel->setLocation(30, 20);
    graphicsWindow.add(messagesLabel);  // As with all components, after creating them you must also
                                        // add them to the graphics window, otherwise they will
                                        // exist but won't show up.

    // Creates the label labeling the total length at the bottom of the window
    GLabel *totalLengthLabel = new GLabel();
    totalLengthLabel->setText("Length:");
    totalLengthLabel->setLocation(320, 360);
    graphicsWindow.add(totalLengthLabel);

    // Creates the label displaying the total length at the bottom of the window
    GLabel *currentLineLengthLabel = new GLabel();
    sprintf(message, "%d", totalLength);  // print into a string
    currentLineLengthLabel->setText(message);
    currentLineLengthLabel->setLocation(320, 375);
    graphicsWindow.add(currentLineLengthLabel);

    // Creates the buttons along the bottom of the window
    MyButton addPointsButton(10, 350, 70, 30, "green", "Add Points");     // Creates the Add Points button
    MyButton drawLinesButton(90, 350, 70, 30, "lightgray", "Draw Lines"); // Creates the Draw Lines button
    MyButton exitButton(250, 350, 50, 30, "red", "Exit");                 // Creates the Exit button

    // Make the graphics window have focus, since all interaction will be through the window
    graphicsWindow.requestFocus();

    // Set values to use for keeping track of program modes
    const int Default = 0;
    const int DrawPoints = 1;
    const int DrawLines = 2;
    int programMode = Default;  // Modes are Default, DrawPoints, DrawLines. Start in Default mode
                                // Change to the other modes when those buttons are pressed.
    bool buttonWasPressed = false;    // Set to true when a button is pressed.  Used to avoid drawing on buttons
                                      // immediately when they are pressed.
    bool lineBeingDrawn = false; // Set to true when the user has started drawing a line but isn't finished (hasn't released the mouse) yet

    int mouseX = -1; // Will store mouse position
    int mouseY = -1;

    GLine *pLine = new GLine(0, 0, 0, 0);  // Line to be drawn on the screen.  It starts out as a single unseen point.
    pLine->setLineWidth(2);
    graphicsWindow.add(pLine);

    GMouseEvent e; // Stores the mouse event each time through the event loop

    // Main event loop
    while(true) {
        // Wait for a mouse event, then get the x,y coordinates
        e = waitForEvent(MOUSE_EVENT);
        // Get the current mouse x,y location
        mouseX = e.getX();
        mouseY = e.getY();

        //  -------- Perform actions for different kinds of mouse events --------
        if(e.getEventType() == MOUSE_PRESSED) {
            // See if Exit button was clicked by seeing if mouse press location is inside the button
            if(exitButton.contains(mouseX, mouseY)) {
                messagesLabel->setLabel("Exiting...");
                break;
            }
            // See if Add Points button was clicked by seeing if mouse press location is inside the button
            else if(addPointsButton.contains(mouseX, mouseY)) {
                messagesLabel->setLabel("Click to add points.");
                programMode = DrawPoints;
                buttonWasPressed = true;
            }
            // See if Draw Lines button was clicked by seeing if mouse press location is inside the button
            else if(drawLinesButton.contains(mouseX, mouseY)) {
                messagesLabel->setLabel("Click and drag between two points to draw a line.");
                programMode = DrawLines;
                buttonWasPressed = true;
            }

            if(programMode == DrawPoints && !buttonWasPressed) {
                // Makes a blue point at the spot the user clicked on and adds it to the pointsArray vector
                pointsArray.push_back(new MyPoint(mouseX, mouseY, "blue", PointRadius, true));
            }
            else if(programMode == DrawLines && !buttonWasPressed) {
                // Starts drawing a new line by setting pLine's start and endpoints both to the location of the nearest point
                clickToPoint(pointsArray, mouseX, mouseY); // Changes mouseX and mouseY to the coordinates of the nearest point
                pLine->setStartPoint(mouseX, mouseY);
                pLine->setEndPoint(mouseX, mouseY);

                lineBeingDrawn = true; // Sets lineBeingDrawn to true since the user has started drawing a new line but not finished yet
            }
        }
        else if(e.getEventType() == MOUSE_RELEASED) {
            if(programMode == DrawLines && !buttonWasPressed) {
                // Set the end point of the line to the nearest point
                clickToPoint(pointsArray, mouseX, mouseY); // Changes mouseX and mouseY to the coordinates of the nearest point
                pLine->setEndPoint(mouseX, mouseY);

                // Make a new line with the same begin and end points as pLine and add it to the linesArray vector
                linesArray.push_back(new GLine(pLine->getStartPoint().getX(), pLine->getStartPoint().getY(), pLine->getEndPoint().getX(), pLine->getEndPoint().getY()));

                // Sets the new line's width and adds it to the graphics window
                linesArray.back()->setLineWidth(2);
                graphicsWindow.add(linesArray.back());

                totalLength += sqrt(pow(pLine->getHeight(), 2) + pow(pLine->getWidth(), 2)); // Uses the pythagorean theorum to get the length of the line and add it to the total length
                sprintf(message, "%d", totalLength);  // print into a string
                currentLineLengthLabel->setLabel(message); // Updates the total length label
                // If the total length is less than or within 5% of the ideal solution, displays a message saying so
                if(totalLength - idealScore < idealScore * 0.05) {
                    sprintf(message, "YES! %d is less than or close to goal of %d", totalLength, idealScore);  // print into a string
                    messagesLabel->setLabel(message);
                }
                // Otherwise, displays a different message
                else {
                    sprintf(message, "Sorry, %d is a lot longer than %d", totalLength, idealScore);  // print into a string
                    messagesLabel->setLabel(message);
                }

                lineBeingDrawn = false; // The line has finished being drawn
            }

            buttonWasPressed = false; // Sets buttonWasPressed to false, so that next time through the event loop the user now can place a point or draw a line
        }
        else if(e.getEventType() == MOUSE_DRAGGED && lineBeingDrawn) {
            // Moves the endpoint of the line being drawn and displays the new total line length at the bottom right
            pLine->setEndPoint(mouseX, mouseY);
            currTotalLength = totalLength + sqrt(pow(pLine->getHeight(), 2) + pow(pLine->getWidth(), 2)); // Updates the current total line length
            sprintf(message, "%d", currTotalLength); // print into a string
            currentLineLengthLabel->setLabel(message); // Doesn't actually change the totalLength variable until the line is finished
        }
    }

    // Close the windows, first the graphics window
    graphicsWindow.requestFocus();
    graphicsWindow.close();
    exitGraphics();

    return 0;
}

