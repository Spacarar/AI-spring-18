#pragma once
#include"GameEngine.h"
#include"GridDictionary.h"
#include<random>
#include<fstream>

const int CLEANRUNS = 16; //number of times it will clean up dictionary
const int PICKRUNS = 500; //evaluations between cleanups 


class GridPlayer {
	int cycles; //how far the player will check the simulation
	int pieces; //the number of pieces the player can place
	Grid grid;
	GridDictionary gd;
	int displayIt;
	int deadChecked;
	int oscChecked;
	int stagChecked;
	int liveChecked;
public:
	GridPlayer() {
		srand(time(NULL));
		cycles = 10;
		pieces = 1;
		grid = Grid();
		gd = GridDictionary();
	}
	GridPlayer(int cyc, int pie) {
		srand(time(NULL));
		cycles = cyc;
		pieces = pie;
		grid = Grid();
		gd = GridDictionary();
	}
	void start() {
		// evaluate cycles
		//While (morePatterns)
		//	while(gd.updateState(grid.me())==LIVING){
		//		gd.update(grid.me(),&grid);
		//		grid.planMove();
		//		grid.update();
		//	}
		//	tell user about pattern
		//	getNextPattern()
		//}
		//output some stuff.
		//D: 8021  O: 1791 S: 165 L: 23
		//D: 8007  O : 1780 S : 169 L : 44
		//D: 7991  O: 1808 S: 173 L: 28
		//D: 7984  O: 1813 S: 172 L: 31
		for (int c = 0; c < CLEANRUNS; c++) {
			for (int i = 0; i < PICKRUNS; i++) {
				//cout << endl << "Begin Evaluation:";
				if(c%4==0||c==0)pickPieces();
				else(pickMutation());
				string me = grid.me();
				for (int i = 0; i < cycles; i++) {
					gd.update(me, grid, i);
					switch (gd.stateOf(me)) {
					case LIVING:
						//printf("%d working...\n", i);
						grid.planMove();
						grid.update();
						break;
					case OSCILLATING:
						//printf("OSCILLATION PATTERN ASSUMED %d ", gd.record(me).repeatedView);
						//cout << "Final: " << grid.me() << " (" << i << ")\n";
						oscChecked++;
						i = cycles + 1;
						break;
					case STAGNANT:
						//printf("STAGNANT PATTERN ASSUMED");
						//cout << "Final: " << grid.me() << " (" << i << ")\n";
						stagChecked++;
						i = cycles + 1;
						break;
					case DEAD:
						//printf("All life has died at cycle:%d", gd.record(me).deathCycle);
						deadChecked++;
						i = cycles + 1;
						break;
					default:
						printf("Failed or unknown cycle!\n");
					}
					if (i == cycles-1) {
						//cout << "Final: " << grid.me() << " (" << i << ")\n";
						//cout << "counting living grid: " << gd.gridDict[me].lastState << ":0"<<endl;
						liveChecked++;
						//cin.get();
					}
				}
				//printf("\n DONE EVALUATING ONE PATTERN \n");
			}
			//cout << "D: " << deadChecked << "  O: " << oscChecked << " S: " << stagChecked << " L: " << liveChecked << endl;
			//printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
			cout << "Cleaning : " << gd.gridDict.size();
			cleanupDictionary(c+1);
			cout << " Now: " << gd.gridDict.size() << endl;
			printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
		}
		//cout << "D: " << deadChecked << "  O: " << oscChecked << " S: " << stagChecked << " L: " << liveChecked << endl;
		//printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
		exportLiving();
		displayIt = 0;
	}
	vector<pair<int, int> > nextLivingFound() {
		int c = 0;
		int looped = 0;
		while (looped<3) {
			c = 0;
			for (map<string, GameRecord>::iterator it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
				if (it->second.lastState == LIVING) {
					if (displayIt < c) {
						displayIt = c;
						return it->second.startCoords;
					}
				}
				c++;
			}
			looped++;
			displayIt = 0;
		}
		vector<pair<int, int>> none;
		return none;
	}
	void exportLiving() {
		map<string, GameRecord>::iterator it;
		ofstream fout;
		fout.open("LivingExport.txt");
		for (it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
			if (it->second.lastState == LIVING)fout << it->first << endl;
		}
		fout.close();
	}
	void pickPieces() {
		int tX, tY;
		vector<pair<int, int> > chosenCoords;
		grid.clear();
		while (gd.exists(grid.me())) {
			grid.clear();
			for (int i = 0; i < pieces; i++) {
				tX = rand() % GRIDSIZE;
				tY = rand() % GRIDSIZE;
				chosenCoords.push_back(make_pair(tX, tY));
			}
			grid.turnOnPixel(chosenCoords);
		}
	
	}

	void pickMutation() {
		int chosen = rand() % gd.gridDict.size();
		grid.clear();
		map<string, GameRecord>::iterator it = gd.gridDict.begin();
		for (int i = 0; i < chosen; i++)++it;
		int tries = 0;
		while (gd.exists(grid.me())) {
			if (tries > 5) {
				cout << "mutation already found" << endl;
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

	void cleanupDictionary(int lowLimit) {
		map<string, GameRecord>::iterator it;
		for (it = gd.gridDict.begin(); it != gd.gridDict.end()&&gd.gridDict.size()>10; ++it) {
			//lasted too short of a period.
			if (it->second.deathCycle>0 && it->second.deathCycle < lowLimit &&it->second.lastState!=LIVING) {
				//cout << "erasing LC: "<<it->second.deathCycle<<"  " << it->first << endl;
				gd.gridDict.erase(it);
				it = gd.gridDict.begin();
			}
		}
	}

	vector<pair<int, int> >  mutation(vector<pair<int, int> > og) {
		int randDirection = 0;
		vector<pair<int, int> > newPieces;
		for (int i = 0; i < og.size(); i++) {
			if (rand() % 100 < 20) {
				randDirection = rand() % 80 + 1;
				if (randDirection <= 10) newPieces.push_back(make_pair(og[i].first - 1, og[i].second));
				else if (randDirection <= 20) newPieces.push_back(make_pair(og[i].first + 1, og[i].second));
				else if (randDirection <= 30) newPieces.push_back(make_pair(og[i].first, og[i].second-1));
				else if (randDirection <= 40) newPieces.push_back(make_pair(og[i].first, og[i].second+1));
				else if (randDirection <= 50) newPieces.push_back(make_pair(og[i].first - 1, og[i].second-1));
				else if (randDirection < 60) newPieces.push_back(make_pair(og[i].first - 1, og[i].second+1));
				else if (randDirection < 70) newPieces.push_back(make_pair(og[i].first + 1, og[i].second-1));
				else newPieces.push_back(make_pair(og[i].first + 1, og[i].second+1));
			}
			else newPieces.push_back(og[i]);
		}
		return newPieces;
	}
};
