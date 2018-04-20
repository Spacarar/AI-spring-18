#pragma once
#include"GameEngine.h"
#include"GridDictionary.h"
#include<random>
#include<fstream>

const int CLEANRUNS = 50; //number of times it will clean up dictionary
const int PICKRUNS = 50; //new evaluations tried between cleanups
const double CUTOFF = 0.8;//grid dictionary average fitness % cutoff (cutoff*(average of dictionary) anything less is thrown out)
//to meet cleanup criteria, the grid must pass (cutoff*(average of dictionary) the average delta of the entire dictionary


class GridPlayer {
	int cycles; //how far the player will check the simulation
	int pieces; //the number of pieces the player can place
	double lowAverage; //the current cutoff number
	Grid grid;
	GridDictionary gd;
	int displayIt; //current index to display (used in gameEngine)

	//number of each type the GridPlayer has evaluated so far
	int deadChecked;
	int oscChecked;
	int stagChecked;
	int liveChecked;

	//picks a random cluster of pieces on the map.
	void pickPieces() {
		int tX, tY;
		int dX, dY;
		vector<pair<int, int> > chosenCoords;
		grid.clear();
		while (gd.exists(grid.me())) {
			tX = rand() % GRIDSIZE;
			tY = rand() % GRIDSIZE;
			grid.clear();
			for (int i = 0; i < pieces; i++) {
				//place a piece +- i distance away
				if (rand() % 2 == 0)
					dX = rand() % (i + 1);
				else dX = (-1)*rand() % (i + 1);
				if (rand() % 2 == 0)
					dY = rand() % (i + 1);
				else dY = (-1)*rand() % (i + 1);

				//dX = (rand() % 3 - 1)*(rand() % (int(i/2) + 1));
				//dY = (rand() % 3 - 1)*(rand() % (int(i/2) + 1));
				chosenCoords.push_back(make_pair(tX + dX, tY + dY));
			}
			grid.turnOnPixel(chosenCoords);
			grid.planMove();
			grid.update();
		}

	}

	//takes the complete union of two random grids
	void pickUnion() {
		grid.clear();
		int chosen1;
		int chosen2;
		map<size_t, GameRecord>::iterator it1;
		map<size_t, GameRecord>::iterator it2;
		vector<pair<int, int> > chosenCoords;
		while (gd.exists(grid.me())) {
			chosenCoords.clear();
			chosen1 = rand() % (gd.gridDict.size());
			chosen2 = rand() % (gd.gridDict.size());
			it1 = gd.gridDict.begin();
			it2 = gd.gridDict.begin();
			//cout << "picking union " << endl;
			for (int i = 0; i < chosen1&&it1 != gd.gridDict.end(); i++)++it1;
			for (int i = 0; i < chosen2&&it2 != gd.gridDict.end(); i++)++it2;
			if (it1 == gd.gridDict.end())return dPickPieces();
			if (it2 == gd.gridDict.end())return dPickPieces();
			chosenCoords.insert(chosenCoords.end(), it1->second.startCoords.begin(), it1->second.startCoords.end());
			chosenCoords.insert(chosenCoords.end(), it2->second.startCoords.begin(), it2->second.startCoords.end());
			grid.clear();
			grid.turnOnPixel(chosenCoords);
			grid.planMove();
			grid.update();
		}

	}

	//moves a random set of pixels around on an existing grid using a vector returned by mutation
	void pickMutation() {
		int chosen = rand() % (gd.gridDict.size()); //the board it will mutate over
		grid.clear();
		map<size_t, GameRecord>::iterator it = gd.gridDict.begin();
		for (int i = 0; i < chosen&&it!=gd.gridDict.end(); i++)++it;
		if (it == gd.gridDict.end())return dPickPieces();
		int tries = 0;
		while (gd.exists(grid.me())) {
			if (tries > 4) {
				//cout << "mutation already found" << endl;
				chosen = rand() % gd.gridDict.size();
				it = gd.gridDict.begin();
				for (int i = 0; i < chosen; i++)++it;
				tries = 0;
			}
			grid.clear();
			grid.turnOnPixel(mutation(it->second.startCoords));
			tries++;
		}
	}

	void verticalMesh() {
		grid.clear();
		int chosen1;
		int chosen2;
		map<size_t, GameRecord>::iterator it1;
		map<size_t, GameRecord>::iterator it2;
		vector<pair<int, int> > chosenCoords;
		while (gd.exists(grid.me())) {
			chosenCoords.clear();
			chosen1 = rand() % (gd.gridDict.size());
			chosen2 = rand() % (gd.gridDict.size());
			it1 = gd.gridDict.begin();
			it2 = gd.gridDict.begin();
			//cout << "picking union " << endl;
			for (int i = 0; i < chosen1&&it1!=gd.gridDict.end(); i++)++it1;
			for (int i = 0; i < chosen2&&it2 != gd.gridDict.end(); i++)++it2;
			if (it1 == gd.gridDict.end())return dPickPieces();
			if (it2 == gd.gridDict.end())return dPickPieces();
			Grid g1;
			Grid g2;
			g1.turnOnPixel(it1->second.startCoords);
			g2.turnOnPixel(it2->second.startCoords);
			for (int y = 0; y < GRIDSIZE; y++) {
				for (int x = 0; x < GRIDSIZE; x++) {
					if (x % 2 == 0) {
						if (g1.isAlive(x, y))chosenCoords.push_back(make_pair(x, y));
					}
					else {
						if (g2.isAlive(x, y))chosenCoords.push_back(make_pair(x, y));
					}
				}
			}
			grid.turnOnPixel(chosenCoords);
			grid.planMove();
			grid.update();
		}
	}
	void horizontalMesh() {
		grid.clear();
		int chosen1;
		int chosen2;
		map<size_t, GameRecord>::iterator it1;
		map<size_t, GameRecord>::iterator it2;
		vector<pair<int, int> > chosenCoords;
		while (gd.exists(grid.me())) {
			chosenCoords.clear();
			chosen1 = rand() % (gd.gridDict.size());
			chosen2 = rand() % (gd.gridDict.size());
			it1 = gd.gridDict.begin();
			it2 = gd.gridDict.begin();
			for (int i = 0; i < chosen1&&it1 != gd.gridDict.end(); i++)++it1;
			for (int i = 0; i < chosen2&&it2 != gd.gridDict.end(); i++)++it2;
			if (it1 == gd.gridDict.end())return dPickPieces();
			if (it2 == gd.gridDict.end())return dPickPieces();
			Grid g1;
			Grid g2;
			g1.turnOnPixel(it1->second.startCoords);
			g2.turnOnPixel(it2->second.startCoords);
			for (int y = 0; y < GRIDSIZE; y++) {
				for (int x = 0; x < GRIDSIZE; x++) {
					if (y % 2 == 0) {
						if (g1.isAlive(x, y))chosenCoords.push_back(make_pair(x, y));
					}
					else {
						if (g2.isAlive(x, y))chosenCoords.push_back(make_pair(x, y));
					}
				}
			}
			grid.turnOnPixel(chosenCoords);
			grid.planMove();
			grid.update();
		}
	}

	//randomly picks 2 clusters of pixels (d for double)
	void dPickPieces() {
		int tX1, tY1, tX2, tY2;
		int dX1, dY1, dX2, dY2;
		vector<pair<int, int> > chosenCoords;
		grid.clear();
		while (gd.exists(grid.me())) {
			tX1 = rand() % GRIDSIZE;
			tY1 = rand() % GRIDSIZE;
			tX2 = rand() % GRIDSIZE;
			tY2 = rand() % GRIDSIZE;
			grid.clear();
			for (int i = 0; i < pieces; i++) {
				//place a piece +- i distance away
				if (rand() % 2 == 0) {
					dX1 = rand() % (i + 1);
					dX2 = rand() % (i + 1);
				}
				else {
					dX1 = (-1)*rand() % (i + 1);
					dX2 = (-1)*rand() % (i + 1);
				}
				if (rand() % 2 == 0) {
					dY1 = rand() % (i + 1);
					dY2 = rand() % (i + 1);
				}
				else {
					dY1 = (-1)*rand() % (i + 1);
					dY2 = (-1)*rand() % (i + 1);
				}
				chosenCoords.push_back(make_pair(tX1 + dX1, tY1 + dY1));
				chosenCoords.push_back(make_pair(tX2 + dX2, tY2 + dY2));
			}
			grid.turnOnPixel(chosenCoords);
			grid.planMove();
			grid.update();
		}

	}

	//returns a muated version of the original grid provided.
	vector<pair<int, int> >  mutation(vector<pair<int, int> > og) {
		int randDirection = 0;
		int randDistance = 0;
		vector<pair<int, int> > newPieces;
		vector<pair<int, int> > finalPieces;
		for (int i = 0; i < og.size(); i++) {
			//only mutate a percentage of pieces
			if (rand() % 100 <= 40) {
				//pick a direction they are mutated in 
				randDirection = rand() % 80 + 1;
				randDistance = rand() % 4;
				if (randDirection <= 10) newPieces.push_back(make_pair(og[i].first - randDistance, og[i].second));
				else if (randDirection <= 20) newPieces.push_back(make_pair(og[i].first + randDistance, og[i].second));
				else if (randDirection <= 30) newPieces.push_back(make_pair(og[i].first, og[i].second - randDistance));
				else if (randDirection <= 40) newPieces.push_back(make_pair(og[i].first, og[i].second + randDistance));
				else if (randDirection <= 50) newPieces.push_back(make_pair(og[i].first - randDistance, og[i].second - randDistance));
				else if (randDirection <= 60) newPieces.push_back(make_pair(og[i].first - randDistance, og[i].second + randDistance));
				else if (randDirection <= 70) newPieces.push_back(make_pair(og[i].first + randDistance, og[i].second - randDistance));
				else newPieces.push_back(make_pair(og[i].first + randDistance, og[i].second + randDistance));
			}
			//if they are not mutated we want the original piece
			else newPieces.push_back(og[i]);
		}
		return newPieces;
	}

public:
	/*
	GridPlayer() {
		srand(unsigned int(time(NULL)));
		cycles = 10;
		pieces = 1;
		grid = Grid();
		gd = GridDictionary();
		lowAverage = 0;
		displayIt = 0;
		deadChecked = oscChecked = stagChecked = liveChecked = 0;
	}
	*/
	GridPlayer(int cyc=0, int pie=0) {
		srand(unsigned int(time(NULL)));
		cycles = cyc;
		pieces = pie;
		grid = Grid();
		gd = GridDictionary();
		lowAverage = 0;
		displayIt = 0;
		deadChecked = oscChecked = stagChecked = liveChecked = 0;
	}

	//runs through the pickruns*cleanruns and then sets the display iterator to 0
	void start() {
		string origin = "random";
		size_t me = 1;
		size_t prevMe = 1;
		for (int c = 0; c < CLEANRUNS; c++) {
			for (int i = 0; i < PICKRUNS; i++) {
				//randomly select for first run, then introduce new data every so often
					if (c == 0) {
						pickPieces();
					}
					else {
						int choice = rand() % 5;
						if (gd.gridDict.size() < 2)choice = 0;
						switch (choice) {
						case 0:
							//cout << "dpick" << endl;
							dPickPieces();
							origin = "random";
							break;
						case 1:
							//cout << "union" << endl;
							pickUnion();
							origin = "union";
							break;
						case 2:
							//cout << "mutation" << endl;
							pickMutation();
							origin = "mutation";
							break;
						case 3:
							//cout << "vmesh" << endl;
							verticalMesh();
							origin = "vmesh";
							break;
						case 4:
							//cout << "hmesh" << endl;
							horizontalMesh();
							origin = "hmesh";
							break;
						}
					}
					me = grid.me();
				if(me==prevMe)cout << prevMe << " &&  " << me << endl;
				prevMe = me;
				//evaluate futures
				for (int f = 0; f < cycles; f++) {
					gd.update(me, grid, f, origin);
					switch (gd.stateOf(me)) {
					case LIVING:
						//printf("%d working...\n", i);
						grid.planMove();
						grid.update();
						break;
					case OSCILLATING:
						//printf("OSCILLATION PATTERN ASSUMED %d ", gd.record(me).repeatedView);
						oscChecked++;
						f = cycles + 1;
						break;
					case STAGNANT:
						//printf("STAGNANT PATTERN ASSUMED");
						stagChecked++;
						f = cycles + 1;
						break;
					case DEAD:
						//printf("All life has died at cycle:%d", gd.record(me).deathCycle);
						deadChecked++;
						f = cycles + 1;
						break;
					default:
						printf("Failed or unknown cycle!\n");
					}
					if (f == cycles - 1) {
						liveChecked++;
					}
				}
				//printf("\n DONE EVALUATING ONE PATTERN \n");
			}
			//cout << "D: " << deadChecked << "  O: " << oscChecked << " S: " << stagChecked << " L: " << liveChecked << endl;
			//printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
			//cout << "Cleaning (" << c + 1 << "): " << gd.gridDict.size();
			cleanupDictionary(c + 1);
			//cout << " Now: " << gd.gridDict.size() << endl;
			printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
		}
		displayIt = 0;
	}

	vector<pair<int, int> > nextFound() {
		int c = 0;
		while (true) {
			c = 0;
			for (map<size_t, GameRecord>::iterator it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
				if (displayIt < c) {
					displayIt = c;
					printf("Display #: %d \n", displayIt);
					printf("{L,D,O,S}: %d  %s  ", it->second.lastState, it->second.origin.c_str());
					printf("min: %d max: %d \n", it->second.minValue, it->second.maxValue);
					return it->second.startCoords;
				}
				c++;
			}
			displayIt = 0;
		}
	}
	vector<pair<int, int> > previousFound() {
		size_t c = gd.gridDict.size();
		while (true) {
			c = gd.gridDict.size();
			for (map<size_t, GameRecord>::iterator it = gd.gridDict.end(); it != gd.gridDict.begin(); --it) {
				//cout << displayIt << endl;
				if (displayIt > c) {
					displayIt = c;
					printf("Display #: %d \n", displayIt);
					printf("{L,D,O,S}: %d  %s  ", it->second.lastState, it->second.origin.c_str());
					printf("min: %d max: %d \n", it->second.minValue, it->second.maxValue);
					return it->second.startCoords;
				}
				c--;
			}
			displayIt = gd.gridDict.size();
		}

	}

	//exports the coordinates from grids that survived. less useful than expected.
	void exportLiving() {
		map<size_t, GameRecord>::iterator it;
		ofstream fout;
		fout.open("LivingExport.txt");
		for (it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
			if (it->second.lastState == LIVING) {
				for (int i = 0; i < it->second.startCoords.size(); i++) {
					fout << "(" << it->second.startCoords[i].first << "," << it->second.startCoords[i].second << ") ";
					if (i != 0 && i % 5 == 0)fout << endl;
				}
			}
		}
		fout.close();
	}
	//throws out any grids that did not meet cutoff criteria.
	void cleanupDictionary(int lowLimit) {
		map<size_t, GameRecord>::iterator it;
		double avgSum = 0;
		for (it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
			//cout << it->second.lastAverage << " ";
			if (it->second.lastAverage != 0.0)avgSum += it->second.lastAverage;
		}
		lowAverage = (avgSum / gd.gridDict.size() - 1)*CUTOFF;
		cout << "\n Low Average: " << lowAverage << endl;
		for (it = gd.gridDict.begin(); it != gd.gridDict.end() && gd.gridDict.size() > 1; ++it) {
			//lasted too short of a period.
			//if ((it->second.lastAverage<lowAverage)||(it->second.deathCycle>0 && it->second.deathCycle < lowLimit) &&(it->second.lastState!=LIVING)) {
			if ((it->second.lastAverage < lowAverage)) {
				//cout << "erasing LC: "<<it->second.deathCycle<<"  " << it->first << endl;
				gd.gridDict.erase(it);
				it = gd.gridDict.begin();
			}
		}
	}


};
