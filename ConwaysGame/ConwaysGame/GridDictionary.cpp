#include"stdafx.h"

#include"GridDictionary.h"
GridDictionary::GridDictionary() {
	gridDict.clear();
	Grid emptyG = Grid();
	update(emptyG.me(), emptyG, 0, "empty");
}
GridDictionary::~GridDictionary() {
	gridDict.clear();
}
UpdateState GridDictionary::stateOf(size_t me) {
	if (gridDict.count(me) > 0) {
		return gridDict[me].lastState;
	}
	else {
		printf("I DONT EXIST YET!\n");
		return FAILED;
	}

}

bool GridDictionary::exists(size_t me) {
	return gridDict.find(me) != gridDict.end();
}

bool GridDictionary::cDeathFound(size_t me) {
	if (gridDict.count(me) > 0) {
		return (gridDict[me].deathCycle != -1);
	}
	return false;
}

UpdateState GridDictionary::update(size_t me, Grid &g, int cycle, string origin) {
	static int numAdded = 1;
	if (gridDict.count(me) > 0) {
		gridDict[me].origin = origin;
		return gridDict[me].update(g, cycle);
	}
	else {
		gridDict.insert(make_pair(me, GameRecord()));
		gridDict[me].startCoords = g.getCoords();
		gridDict[me].origin = origin;
		return update(me, g, cycle, origin);
	}
}

/*
GameRecord* GridDictionary::record(size_t me) {
	if (gridDict.count(me) > 0) {
		return &gridDict[me];
	}
	else {
		printf("This record does not exist yet?");
		return &GameRecord();
	}
}
*/

int GridDictionary::deadCount() {
	int c = 0;
	map<size_t, GameRecord>::const_iterator it;
	for (it = gridDict.begin(); it != gridDict.end(); ++it) {
		if (it->second.lastState == DEAD)c++;
	}
	return c;
}
int GridDictionary::liveCount() {
	int c = 0;
	map<size_t, GameRecord>::const_iterator it;
	for (it = gridDict.begin(); it != gridDict.end(); ++it) {
		if (it->second.lastState == LIVING)c++;
	}
	return c;
}
int GridDictionary::totalCount() {
	return int(gridDict.size());
}
int GridDictionary::oscCount() {
	int c = 0;
	map<size_t, GameRecord>::const_iterator it;
	for (it = gridDict.begin(); it != gridDict.end(); ++it) {
		if (it->second.lastState == OSCILLATING)c++;
	}
	return c;
}
int GridDictionary::stagCount() {
	int c = 0;
	map<size_t, GameRecord>::const_iterator it;
	for (it = gridDict.begin(); it != gridDict.end(); ++it) {
		if (it->second.lastState == STAGNANT)c++;
	}
	return c;
}
string GridDictionary::stateString(int st) {
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