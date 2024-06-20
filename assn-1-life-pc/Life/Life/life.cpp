/**
 * File: life.cpp
 * --------------
 * Implements the Game of Life.
 */

#include <iostream>  // for cout
#include <fstream>
using namespace std;

#include "console.h" // required of all files that contain the main function
#include "simpio.h"  // for getLine
#include "Grid.h"
#include "random.h"
#include "gevents.h"

#include "life-constants.h"  // for kMaxAge
#include "life-graphics.h"   // for class LifeDisplay


//It includes the prototypes.
#include "prototypes.h"


//global variables
int delay = 10; //runtime
bool requireEnter = false; //step by step simulation



//main method.
int main() {
	
	LifeDisplay display;
	while(true){
		display.setTitle("Game of Life");
		welcome();
		string userInput = getUserInput();
		if(userInput == ""){
			randomSimulate(display);
		}else{
			fileSimulate(display,userInput);
		}
		if(!cycle()) break;
	}
    return 0;
}


/*
 * It displays the welcome message on the console.
 * The message tells about the simulation process.
 */
static void welcome() {
	cout << "Welcome to the game of Life, a simulation of the lifecycle of a bacteria colony." << endl;
	cout << "Cells live and die by the following rules:" << endl << endl;
	cout << "\tA cell with 1 or fewer neighbors dies of loneliness" << endl;
	cout << "\tLocations with 2 neighbors remain stable" << endl;
	cout << "\tLocations with 3 neighbors will spontaneously create life" << endl;
	cout << "\tLocations with 4 or more neighbors die of overcrowding" << endl << endl;
	cout << "In the animation, new cells are dark and fade to gray as they age." << endl << endl;
	char ch = 7;
	cout << ch << endl;
    waitForEnter("Hit [enter] to continue....   ");
	cout << "You can start your colony with random cells or load from a prepared file." << endl;
}


/*
 * It waits for the user to press enter.
 */
static void waitForEnter(string message) {
    cout << message;
    (void) getLine();
}


/*
 * It gets an input string from the user.
 * return input string.
 */
string getUserInput(){
	
	string input = getLine("Enter name of colony file (or RETURN to seed randomly):");
	return input;
}


/*
 * It is the first kind of simulation.
 * It runs random simulation.
 * It make a grid of 60*40 grid and then fills it with random cells
 * and then calls runSimulation to run the simulation.
 */
void randomSimulate(LifeDisplay &display){
	
	display.setTitle("Random Simulation");
	Grid<int> grid(randomInteger(40,60),randomInteger(40,60));
	display.setDimensions(grid.numRows(),grid.numCols());
	fillRandomSeed(grid);
	displayGrid(display,grid);
	getPreferences();
	runSimulation(display,grid);
}


/*
 * It fills the given grid with random values with a 50% chance.
 */
void fillRandomSeed(Grid<int> &grid){
	
	for(int i = 0; i < grid.numRows(); i++){
		for(int j = 0; j < grid.numCols(); j++){
			if(randomChance(0.5))
				grid[i][j] = randomInteger(0,kMaxAge);
		}
	}
}



/*
 * It is the second kind of simulation.
 * It starts the simulation by reading in a file.
 */
void fileSimulate(LifeDisplay &display,string filename){
	
	Grid<int> grid;
	parseFile(filename,display,grid);
	display.setTitle(filename+" Simulation");
	displayGrid(display,grid);
	getPreferences();
	runSimulation(display,grid);
}


/*
 * It reads in a file and parses it and then gives it to fillGrid function
 * to fill the grid with the values from file.
 */
void parseFile(string filename,LifeDisplay &display,Grid<int> &grid){


	ifstream infile;
	string directory = "files//";
	while(true){
		infile.open((directory.append(filename)).c_str());
		if(infile.fail()){
			cerr << "File not found." << endl; 
			infile.clear();
			filename = getLine("Enter a filename: ");
		}else{
			break;
		}
	}
	string line = "";
	while(true){
		getline(infile,line);
		if(line != "" && line[0] != '#'){
			break;
		}
	}

	int row,col;
	row = stringToInteger(line);
	getline(infile,line);
	col = stringToInteger(line);
	display.setDimensions(row,col);
	grid.resize(row,col);
	fillGrid(infile,grid);
}


/*
 * It fills the grid with the values from the file.
 * char '-' is given a empty cell.
 * char 'X' is given a new cell with age 1.
 */
void fillGrid(ifstream &infile, Grid<int> &grid){
	
	string line = "";
	for(int i = 0; i < grid.numRows(); i++){
		getline(infile,line);
		for(int j = 0; j < line.length(); j++){
			if(line[j] == '-'){
				grid[i][j] = 0;
			}else if(line[j] == 'X'){
				grid[i][j] = 1;
			}
		}
	}
}


/*
 * It asks the user for their preferences weather they want to 
 * run the simulation fast or slow or others(see code its clear).
 */
void getPreferences(){
	
	cout << "You choose how fast to run the simulation.\n";
	cout << "\t1 = As fast as this chip can go!" << endl;
	cout << "\t2 = Not too fast, this is a school zone."<< endl;
	cout << "\t3 = Nice and slow so I can watch everything that happens."<< endl;
	cout << "\t4 = Require enter key be pressed before advancing to next generation."<< endl;
	while(true){
		int value = getInteger("Your Choice: ");
		if(value == 1){
			delay = 10;
			break;
		}else if(value == 2){
			delay = 200;
			break;
		}else if(value == 3){
			delay = 300;
			break;
		}else if(value == 4){
			requireEnter = true;
			break;
		}else if(value < 1 || value > 4){
			cout << "Please enter a number between 1 and 4" << endl;
		}
	}
}


/*
 * It runs the simulation by calling various functions.
 * If the user clicks the window with the mouse the simulation ends
 */
void runSimulation(LifeDisplay &display,Grid<int> &grid){
	
	int row = grid.numRows();
	int col = grid.numCols();
	while(true){
		GMouseEvent me;
		if (getNextEvent(me)) {
			if (me.getEventType() == MOUSE_CLICKED) break;
		}else{
			displayGrid(display,grid);
			Grid<int> newGeneration(row,col);
			for(int i = 0; i < grid.numRows(); i++){
				for(int j = 0; j < grid.numCols(); j++){
					checkNeighbors(grid,newGeneration,i,j);
				}
			}
			if(remainsSame(grid,newGeneration)) break;
			grid = newGeneration;
			pause(delay);
			if(requireEnter)
				waitForEnter("");
		}
	}
}


/*
 * It checks to see how many neighibours are present and the updates the next generation
 * accordingly.
 */
void checkNeighbors(Grid<int> &oldGeneration,Grid<int> &newGeneration,int row,int col){
	
	int numNeighbors = 0;
	if(row >= 0 && col > 0){
		if(oldGeneration[row][col-1] != 0)numNeighbors++; //left
	}
	if(row >= 0 && col < oldGeneration.numCols()-1){
		if(oldGeneration[row][col+1] != 0)numNeighbors++; //right
	}
	if(row > 0 && col >= 0){
		if(oldGeneration[row-1][col] != 0)numNeighbors++; //top
	}
	if(row < oldGeneration.numRows()-1 && col >= 0){
		if(oldGeneration[row+1][col] != 0)numNeighbors++; //bottom
	}
	if(row > 0 && col > 0){
		if(oldGeneration[row-1][col-1] != 0)numNeighbors++; //topLeft
	}
	if(row < oldGeneration.numRows()-1 && col > 0){
		if(oldGeneration[row+1][col-1] != 0)numNeighbors++; //bottomleft
	}
	if(row > 0 && col < oldGeneration.numCols()-1){
		if(oldGeneration[row-1][col+1] != 0)numNeighbors++; //topright
	}
	if(row < oldGeneration.numRows()-1 && col < oldGeneration.numCols()-1){
		if(oldGeneration[row+1][col+1] != 0)numNeighbors++; //bottomright
	}
	updateStatus(numNeighbors,oldGeneration,newGeneration,row,col);
}


/*
 * It updates the status of the cell in the next generation by using numNeigbours.
 */
void updateStatus(int numNeighbors,Grid<int> &oldGeneration,Grid<int> &newGeneration,int row,int col){
	
	if(numNeighbors == 1 || numNeighbors == 0){
		newGeneration[row][col] = 0;
	}else if(numNeighbors == 2){
		if(oldGeneration[row][col] > 0 &&  oldGeneration[row][col] <= kMaxAge)
			newGeneration[row][col] = oldGeneration[row][col]+1;
		else
			newGeneration[row][col] = oldGeneration[row][col];
	}else if(numNeighbors == 3){
		if(oldGeneration[row][col] == 0)
			newGeneration[row][col] = 1;
		else if(oldGeneration[row][col] <= kMaxAge)
			newGeneration[row][col] = oldGeneration[row][col]+1;
		else
			newGeneration[row][col] = oldGeneration[row][col];
	}else if(numNeighbors >= 4){
		newGeneration[row][col] = 0;
	}
}



/*
 * It requests the user weather the user wants to re-run a new simulation or exit.
 */
bool cycle(){
	while(true){
		string line = getLine("Would you like to run another? ");
		if(line == "yes")
			return true;
		else if(line == "no")
			return false;
		else
			cout << "please enter 'yes' or 'no'." << endl;
	}
}



/*
 * It takes the grid and displays it on the graphics window.
 */
void displayGrid(LifeDisplay &display,Grid<int> &grid){
	
	for(int i = 0; i < grid.numRows(); i++){
		for(int j = 0; j < grid.numCols(); j++){
			display.drawCellAt(i, j, grid[i][j]);
		}
	}
}



/*
 * It is a function where it checks to see if the old generation and new generation are same
 * because it helps to stop the simulation cycling forever.
 */
bool remainsSame(Grid<int> &oldGeneration,Grid<int> &newGeneration){
	
	for(int i = 0; i < oldGeneration.numRows(); i++){
		for(int j = 0; j < oldGeneration.numCols(); j++){
			if(oldGeneration[i][j] != newGeneration[i][j]) return false;
		}
	}
	return true;
}