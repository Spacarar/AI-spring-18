#pragma once
#include"GameEngine.h"
#include<map>
#include<vector>

using namespace std;

typedef enum UpdateState {LIVING,DEAD,OSCILLATING,STAGNANT,FAILED} UpdateState;

class GameRecord {
	
	//count the repititions in the oscillating string array
	void countOsc(size_t me) {
		repeatedView = 0;
		for (int i = 0; i < 5; i++) {
			if (me == oscillationRecord[i]) {
				//cout << me << "==" << oscillationRecord[i] << endl;
				repeatedView++;
			}
		}
	}
	
	//returns true if believed to be stagnant data
	bool stagnantCheck(int lValue) {
		int count = 0;
		//int startingP;
		//liveValues.size() < 5 ? startingP = 0 : startingP = liveValues.size() - 14;
		for (int i = 0; i < 5; i++) {
			if (lValue == liveValues[i])count++;
		}
		return count > 7;
	}

public:
	vector < pair<int, int> > startCoords;
	UpdateState lastState;
	int liveValues[5];
	//FIXME add a running average a=(1-p)(last a) +(p)(liveValue); p=1/99 for 99 elements
	size_t oscillationRecord[5]; //string record of last 5 grids, kept to check for cycles
	unsigned int currOsc; //current index to be inserted for oscillation check
	int repeatedView; //the number of repeated grids seen
	int deathCycle; //the cycle the pattern was determined to have "stopped"
	GameRecord() {
		lastState = LIVING;
		for (int i = 0; i < 5; i++) {
			liveValues[i] = 0;
			oscillationRecord[i] = 0;
		}
		startCoords.clear();
		currOsc = 0;
		repeatedView = 0;
		deathCycle = -1;
	}
	UpdateState update(Grid &g, int cycle) {
		//liveValues.push_back(g.liveValue());
		liveValues[currOsc] = g.liveValue();
		countOsc(g.me());
		oscillationRecord[currOsc] = g.me();
		currOsc = (currOsc + 1) % 5;
		if (g.liveValue() == 0) {
			deathCycle = cycle;
			lastState = DEAD;
			return DEAD;
		}
		else if (repeatedView > 1) {
			lastState = OSCILLATING;
			deathCycle = cycle;
			return OSCILLATING;
		}
		//else if (stagnantCheck(g.liveValue())) {
			//lastState = STAGNANT;
			//deathCycle = cycle;
			//return STAGNANT;
		//}
		else {
			lastState = LIVING;
			return LIVING;
		}
	}
};

class GridDictionary {
private:
	unsigned int check_interval;
	/*
	GameRecord is meant to store the # of living things on screen in a vector (index=cycle number/interval)
	deathcycle is the cycle in which no blocks are living or have become stagnant
	updates will be handled based on return type
	living updates will be allowed to continue (until limited by recursion)
	other updates will break free to start new pattern (gridplayer will be handling the starting pattern)
	*/


public:
	map<size_t, GameRecord> gridDict;
	GridDictionary(int interval=1) {
		interval > 0 ? check_interval = interval : check_interval = 1;
		gridDict.clear();
		Grid emptyG = Grid();
		update(emptyG.me(), emptyG,0);
	}
	UpdateState stateOf(size_t me) {
		if (gridDict.count(me) > 0) {
			return gridDict[me].lastState;
		}
		else {
			printf("I DONT EXIST YET!\n");
			return FAILED;
		}
		
	}

	bool exists(size_t me) {
		return gridDict.count(me);
	}

	bool cDeathFound(size_t me) {
		if (gridDict.count(me) > 0) {
			return (gridDict[me].deathCycle != -1);
		}
		return false;
	}

	UpdateState update(size_t me, Grid &g,int cycle) {
		if (gridDict.count(me) > 0) {
			return gridDict[me].update(g,cycle);
		}
		else {
			if (gridDict.size() < gridDict.max_size()) {
				gridDict.insert(make_pair(me, GameRecord()));
				gridDict[me].startCoords = g.getCoords();
				return update(me, g,cycle);
			}
			else return FAILED;
		}
	}
	GameRecord record(size_t me) {
		if (gridDict.count(me) > 0) {
			return gridDict[me];
		}
		else {
			printf("This record does not exist yet?");
			return GameRecord();
		}
	}

	int deadCount() {
		int c = 0;
		map<size_t, GameRecord>::const_iterator it;
		for (it = gridDict.begin(); it != gridDict.end(); ++it) {
			if(it->second.lastState==DEAD)c++;
		}
		return c;
	}
	int liveCount() {
		int c = 0;
		map<size_t, GameRecord>::const_iterator it;
		for (it = gridDict.begin(); it != gridDict.end(); ++it) {
			if (it->second.lastState == LIVING)c++;
		}
		return c;
	}
	int totalCount() {
		return int(gridDict.size());
	}
	int oscCount() {
		int c = 0;
		map<size_t, GameRecord>::const_iterator it;
		for (it = gridDict.begin(); it != gridDict.end(); ++it) {
			if (it->second.lastState == OSCILLATING)c++;
		}
		return c;
	}
	int stagCount() {
		int c = 0;
		map<size_t, GameRecord>::const_iterator it;
		for (it = gridDict.begin(); it != gridDict.end(); ++it) {
			if (it->second.lastState == STAGNANT)c++;
		}
		return c;
	}
	
};
