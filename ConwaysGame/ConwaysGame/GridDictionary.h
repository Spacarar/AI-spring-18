#pragma once
#include"Grid.h"
#include<map>
#include<vector>
#include<fstream>
#include<math.h>

using namespace std;

typedef enum UpdateState { LIVING, DEAD, OSCILLATING, STAGNANT, FAILED } UpdateState;
const int OSCDEPTH = 10;

class GameRecord {

	//count the repititions in the oscillating string array
	void countOsc(size_t me) {
		repeatedView = 0;
		for (int i = 0; i < OSCDEPTH; i++) {
			if (me == oscillationRecord[i]) {
				//cout << me << "==" << oscillationRecord[i] << endl;
				repeatedView++;
			}
		}
	}

	//returns true if believed to be stagnant data
	bool stagnantCheck() {
		return (lastAverage > 0.0 && lastAverage < .8);
	}

public:
	vector < pair<int, int> > startCoords;
	UpdateState lastState;
	string origin; //how the pattern was made
	//the max number of living pixels that occured
	int maxValue;
	//the min number of living pixels that occured
	int minValue;
	//Running average elements (last lifeValue, used to calculate current Delta)
	int lastLV;
	double lastAverage;

	size_t oscillationRecord[OSCDEPTH];
	unsigned int currOsc;
	unsigned int repeatedView;
	int deathCycle;
	GameRecord() {
		lastState = LIVING;
		maxValue = 0;
		minValue = GRIDSIZE*GRIDSIZE;
		for (int i = 0; i < 5; i++) {
			oscillationRecord[i] = 0;
		}
		startCoords.clear();
		currOsc = 0;
		repeatedView = 0;
		deathCycle = -1;
	}
	UpdateState update(Grid &g, int cycle) {
		int lifeValue = g.liveValue();
		//average update
		if (cycle == 0) {
			lastLV = lifeValue;
			lastAverage = 0.0;
		}
		else if (cycle == 1) {
			lastAverage = abs(lastLV - lifeValue);
			lastLV = lifeValue;
		}
		else {
			lastAverage = ((lastAverage*(cycle - 1)) + abs(lastLV - lifeValue) + (cycle*.1)) / cycle;
		}
		//min/max update
		minValue > lifeValue ? minValue = lifeValue : minValue;
		maxValue < lifeValue ? maxValue = lifeValue : maxValue;

		//oscillation check
		countOsc(g.me());
		oscillationRecord[currOsc] = g.me();
		currOsc = (currOsc + 1) % OSCDEPTH;

		//return values
		if (lifeValue == 0) {
			deathCycle = cycle;
			lastState = DEAD;
			return DEAD;
		}
		else if (repeatedView > 1) {
			lastState = OSCILLATING;
			deathCycle = cycle;
			return OSCILLATING;
		}
		else if (stagnantCheck()) {
			lastState = STAGNANT;
			deathCycle = cycle;
			return STAGNANT;
		}
		else {
			lastState = LIVING;
			return LIVING;
		}
	}
};

class GridDictionary {
private:
	//unsigned int check_interval;
	/*
	GameRecord is meant to store the # of living things on screen in a vector (index=cycle number/interval)
	deathcycle is the cycle in which no blocks are living or have become stagnant
	updates will be handled based on return type
	living updates will be allowed to continue (until limited by depth parameter)
	other updates will break free to start new pattern (gridplayer will be handling the starting pattern)
	*/


public:
	map<size_t, GameRecord> gridDict;
	GridDictionary() {
		gridDict.clear();
		Grid emptyG = Grid();
		update(emptyG.me(), emptyG, 0,"empty");
	}
	~GridDictionary() {
		gridDict.clear();
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

	//returns true if pattern is in dictionary
	bool exists(size_t me) {
		return gridDict.find(me)!=gridDict.end();
	}

	//the dictionary knows when this pattern dies off
	bool cDeathFound(size_t me) {
		if (gridDict.count(me) > 0) {
			return (gridDict[me].deathCycle != -1);
		}
		return false;
	}

	UpdateState update(size_t me, Grid &g, int cycle, string origin) {
		static int numAdded = 1;
		if (gridDict.count(me) > 0) {
			gridDict[me].origin = origin;
			return gridDict[me].update(g, cycle);
		}
		else {
			/*
			if (gridDict.size() != numAdded&&origin!="empty") {
				cout << origin << "  " << me << endl;
				cout << gridDict.size() << "VS" << numAdded<< endl;
				cout << "ERROR OCCURED HERE!!!!!!" << endl;
				cin.get();
				cin.get();
			}
			cout << "adding new " << numAdded << endl;
			if (origin != "empty")numAdded++;
			*/
			gridDict.insert(make_pair(me, GameRecord()));
			gridDict[me].startCoords = g.getCoords();
			gridDict[me].origin = origin;
			return update(me, g, cycle,origin);
		}
	}

	//possibly unnecessary
	GameRecord* record(size_t me) {
		if (gridDict.count(me) > 0) {
			return &gridDict[me];
		}
		else {
			printf("This record does not exist yet?");
			return &GameRecord();
		}
	}

	//Dictionary information things
	int deadCount() {
		int c = 0;
		map<size_t, GameRecord>::const_iterator it;
		for (it = gridDict.begin(); it != gridDict.end(); ++it) {
			if (it->second.lastState == DEAD)c++;
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
	string stateString(int st) {
		switch (st) {
		case LIVING:
			return "Living";
		case DEAD:
			return "Dead";
		case OSCILLATING:
			return "Oscillating";
		case STAGNANT:
			return "Stagnant";
		default:
			return "FAILED";
		}
	}
};
