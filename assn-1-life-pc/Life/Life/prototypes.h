#ifndef _PROTOTYPES_H
#define _PROTOTYPES_H


static void welcome();
static void waitForEnter(string message);
string getUserInput();


void randomSimulate(LifeDisplay &display);
void fillRandomSeed(Grid<int> &grid);


void fileSimulate(LifeDisplay &display,string userInput);
void parseFile(string filename,LifeDisplay &display,Grid<int> &grid);


void fillGrid(ifstream &infile, Grid<int> &grid);
void displayGrid(LifeDisplay &display,Grid<int> &grid);


void runSimulation(LifeDisplay &display,Grid<int> &grid);
void checkNeighbors(Grid<int> &oldGeneration,Grid<int> &newGeneration,int row,int col);
void updateStatus(int numNeighbors,Grid<int> &oldGeneration,Grid<int> &newGeneration,int row,int col);
bool cycle();
void getPreferences();
bool remainsSame(Grid<int> &oldGeneration,Grid<int> &newGeneration);


#endif