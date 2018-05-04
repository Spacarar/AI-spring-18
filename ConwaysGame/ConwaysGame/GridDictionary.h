#pragma once
#include"Grid.h"
#include<map>
#include<vector>
#include<fstream>
#include<math.h>

using namespace std;

typedef enum UpdateState { LIVING, DEAD, OSCILLATING, STAGNANT, FAILED } UpdateState;
const int OSCDEPTH = 3;

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
	/*
	GameRecord is meant to store the # of living things on screen in a vector (index=cycle number/interval)
	deathcycle is the cycle in which no blocks are living or have become stagnant
	updates will be handled based on return type
	living updates will be allowed to continue (until limited by depth parameter)
	other updates will break free to start new pattern (gridplayer will be handling the starting pattern)
	*/


public:
	map<size_t, GameRecord> gridDict;
	GridDictionary();
	~GridDictionary();
	UpdateState stateOf(size_t me);

	//returns true if pattern is in dictionary
	bool exists(size_t me);

	//the dictionary knows when this pattern dies off
	bool cDeathFound(size_t me);

	UpdateState update(size_t me, Grid &g, int cycle, string origin);

	//possibly unnecessary
	//GameRecord* record(size_t me);

	//Dictionary information things
	int deadCount();
	int liveCount();
	int totalCount();
	int oscCount();
	int stagCount();
	string stateString(int st);
};
