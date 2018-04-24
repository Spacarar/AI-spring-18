#pragma once
#include"GridDictionary.h"
#include<random>
#include<fstream>

const int CLEANRUNS = 50; //number of times it will clean up dictionary
const int PICKRUNS = 50; //new evaluations tried between cleanups
const double CUTOFF = 1.0;//grid dictionary average fitness % cutoff (cutoff*(average of dictionary) anything less is thrown out)
//to meet cleanup criteria, the grid must pass (cutoff*(average of dictionary) the average delta of the entire dictionary


class GridPlayer {
	int cycles; //how far the player will check the simulation
	int pieces; //the number of pieces the player can place
	double lowAverage; //the current cutoff number
	Grid grid;
	GridDictionary gd;
	int displayIt; //current index to display (used in gameEngine)
	bool KeepSearching;

	//number of each type the GridPlayer has evaluated so far
	int deadChecked;
	int oscChecked;
	int stagChecked;
	int liveChecked;

	//picks a random cluster of pieces on the map.
	void pickPieces();

	//takes the complete union of two random grids
	void pickUnion();

	//moves a random set of pixels around on an existing grid using a vector returned by mutation
	void pickMutation();

	void verticalMesh();
	void horizontalMesh();

	//randomly picks 2 clusters of pixels (d for double)
	void dPickPieces();

	//returns a muated version of the original grid provided.
	vector<pair<int, int> >  mutation(vector<pair<int, int> > og);

public:
	GridPlayer(int cyc = 0, int pie = 0);

	//runs through the pickruns*cleanruns and then sets the display iterator to 0
	void start();
	void halt();
	vector<pair<int, int> > nextFound();
	vector<pair<int, int> > previousFound();

	//exports the coordinates from grids that survived. less useful than expected.
	void exportLiving();
	//throws out any grids that did not meet cutoff criteria.
	void cleanupDictionary(int lowLimit);


};
