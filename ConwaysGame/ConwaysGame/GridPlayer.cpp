#include"stdafx.h"
#include"GridPlayer.h"
#include <chrono>

void GridPlayer::pickPieces() {
	int tX, tY;
	int dX, dY;
	vector<pair<int, int> > chosenCoords;
	grid.clear();
	while (gd.exists(grid.me())) {
		tX = rand() % GRIDSIZE;
		tY = rand() % GRIDSIZE;
		grid.clear();
		for (int i = 0; i < pieces; i++) {
			if (rand() % 2 == 0)
				dX = rand() % (i + 1);
			else dX = (-1)*rand() % (i + 1);
			if (rand() % 2 == 0)
				dY = rand() % (i + 1);
			else dY = (-1)*rand() % (i + 1);
			chosenCoords.push_back(make_pair(tX + dX, tY + dY));
		}
		grid.turnOnPixel(chosenCoords);
		grid.planMove();
		grid.update();
	}

}

void GridPlayer::pickUnion() {
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
		//chosenCoords.insert(chosenCoords.end(), it1->second.startCoords.begin(), it1->second.startCoords.end());
		//chosenCoords.insert(chosenCoords.end(), it2->second.startCoords.begin(), it2->second.startCoords.end());
		grid.clear();
		if (rand()% 2 == 0) {
			grid.turnOnPixel(it1->second.startCoords);
		}
		else {
			grid.turnOnPixel(rotateGrid(it1->second.startCoords));
		}
		if (rand() % 2 == 0) {
			grid.turnOnPixel(it2->second.startCoords);
		}
		else {
			grid.turnOnPixel(rotateGrid(it2->second.startCoords));
		}
		//grid.turnOnPixel(chosenCoords);
		grid.planMove();
		grid.update();
	}

}

void GridPlayer::pickMutation() {
	int chosen = rand() % (gd.gridDict.size());
	grid.clear();
	map<size_t, GameRecord>::iterator it = gd.gridDict.begin();
	for (int i = 0; i < chosen&&it != gd.gridDict.end(); i++)++it;
	if (it == gd.gridDict.end())return dPickPieces();
	int tries = 0;
	while (gd.exists(grid.me())) {
		if (tries > 4) {
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

void GridPlayer::verticalMesh() {
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
		if (rand() % 2 == 0) {
			g1.turnOnPixel(it1->second.startCoords);
		}
		else {
			g1.turnOnPixel(rotateGrid(it1->second.startCoords));
		}
		if (rand() % 2 == 0) {
			g2.turnOnPixel(it2->second.startCoords);
		}
		else {
			g2.turnOnPixel(rotateGrid(it2->second.startCoords));
		}
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
void GridPlayer::horizontalMesh() {
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
		if (rand() % 2 == 0) {
			g1.turnOnPixel(it1->second.startCoords);
		}
		else {
			g1.turnOnPixel(rotateGrid(it1->second.startCoords));
		}
		if (rand() % 2 == 0) {
			g2.turnOnPixel(it2->second.startCoords);
		}
		else {
			g2.turnOnPixel(rotateGrid(it2->second.startCoords));
		}
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
vector<pair<int, int> > GridPlayer::rotateGrid(vector<pair< int, int> > og) {
	vector<pair<int, int> > newCoords;
	for (int i = 0; i < og.size(); i++) {
		newCoords.push_back(make_pair(og[i].second, og[i].first));
	}
	return newCoords;
}

void GridPlayer::dPickPieces() {
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

vector<pair<int, int> >  GridPlayer::mutation(vector<pair<int, int> > og) {
	int randDirection = 0;
	int randDistance = 0;
	vector<pair<int, int> > newPieces;
	vector<pair<int, int> > finalPieces;
	for (int i = 0; i < og.size(); i++) {
		if (rand() % 100 <= 40) {
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
		else newPieces.push_back(og[i]);
	}
	return newPieces;
}

GridPlayer::GridPlayer(int cyc, int pie) {
	srand(unsigned int(time(NULL)));
	cycles = cyc;
	pieces = pie;
	grid = Grid();
	gd = GridDictionary();
	lowAverage = 0;
	displayIt = 0;
	deadChecked = oscChecked = stagChecked = liveChecked = 0;
	KeepSearching = true;
}

void GridPlayer::start() {
	string origin = "random";
	size_t me = 1;
	size_t prevMe = 1;
	int c = 0;
	while (KeepSearching) {
		for (int i = 0; i < PICKRUNS; i++) {
			if (KeepSearching == false) {
				i = PICKRUNS;
				break;
			}
			//randomly select for first run, then introduce new data every so often
			if (c == 0) {
				pickPieces();
			}
			else {
				int choice = rand() % 5;
				if (gd.gridDict.size() < 2)choice = 0;
				switch (choice) {
				case 0:
					dPickPieces();
					origin = "random";
					break;
				case 1:
					pickUnion();
					origin = "union";
					break;
				case 2:
					pickMutation();
					origin = "mutation";
					break;
				case 3:
					verticalMesh();
					origin = "vmesh";
					break;
				case 4:
					horizontalMesh();
					origin = "hmesh";
					break;
				}
			}
			me = grid.me();
			if (me == prevMe)cout << prevMe << " &&  " << me << endl;
			prevMe = me;
			for (int f = 0; f < cycles; f++) {
				gd.update(me, grid, f, origin);
				switch (gd.stateOf(me)) {
				case LIVING:
					grid.planMove();
					grid.update();
					break;
				case OSCILLATING:
					oscChecked++;
					f = cycles + 1;
					break;
				case STAGNANT:
					stagChecked++;
					f = cycles + 1;
					break;
				case DEAD:
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
		}
		if (KeepSearching == false)break;
		cout << "Cleaning (" << c + 1 << "): " << gd.gridDict.size();
		cleanupDictionary(c + 1);
		if (c < 8)lookupSimilarity();
		printf("Final Count: %d  D:%d  O:%d  S:%d  L:%d\n", gd.totalCount(), gd.deadCount(), gd.oscCount(), gd.stagCount(), gd.liveCount());
		c++;
	}
}
void GridPlayer::halt() {
	KeepSearching = false;
}
vector<pair<int, int> > GridPlayer::nextFound() {
	int c = 0;
	while (true) {
		c = 0;
		for (map<size_t, GameRecord>::iterator it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
			if (displayIt < c) {
				displayIt = c;
				cout << "Display # " << displayIt << endl;
				printf("{L,D,O,S}:%d %s  %s  ", it->second.deathCycle, gd.stateString(it->second.lastState).c_str(), it->second.origin.c_str());
				printf("min: %d max: %d \n", it->second.minValue, it->second.maxValue);
				return it->second.startCoords;
			}
			c++;
		}
		displayIt = 0;
	}
}
vector<pair<int, int> > GridPlayer::previousFound() {
	size_t c = gd.gridDict.size();
	while (true) {
		c = gd.gridDict.size();
		for (map<size_t, GameRecord>::iterator it = gd.gridDict.end(); it != gd.gridDict.begin(); --it) {
			if (displayIt > c) {
				displayIt = c;
				cout << "Display # " << displayIt << endl;
				printf("{L,D,O,S}: %d %s %s  ", it->second.deathCycle, gd.stateString(it->second.lastState).c_str(), it->second.origin.c_str());
				printf("min: %d max: %d \n", it->second.minValue, it->second.maxValue);
				return it->second.startCoords;
			}
			c--;
		}
		displayIt = gd.gridDict.size();
	}

}

void GridPlayer::exportLiving() {
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
void GridPlayer::cleanupDictionary(int lowLimit) {
	map<size_t, GameRecord>::iterator it;
	double avgSum = 0;
	for (it = gd.gridDict.begin(); it != gd.gridDict.end(); ++it) {
		if (it->second.lastAverage != 0.0)avgSum += it->second.lastAverage;
	}
	lowAverage = (avgSum / gd.gridDict.size() - 1)*CUTOFF;
	cout << "\n Low Average: " << lowAverage << endl;
	for (it = gd.gridDict.begin(); it != gd.gridDict.end() && gd.gridDict.size() > 1; ++it) {
		if ((it->second.lastAverage < lowAverage)) {
			gd.gridDict.erase(it);
			it = gd.gridDict.begin();
		}
	}
}
void GridPlayer::lookupSimilarity() {
	map<size_t, GameRecord>::iterator origin;
	map<size_t, GameRecord>::iterator check;
	int values[5];
	int similarityCount = 0;
	for (origin = gd.gridDict.begin(); origin != gd.gridDict.end(); origin++) {
		grid.clear();
		grid.turnOnPixel(origin->second.startCoords);
		for (int i = 0; i < 5; i++) {
			values[i] = grid.liveValue();
			grid.planMove();
			grid.update();
		}
		grid.clear();
		for (check = origin; check != gd.gridDict.end(); check++) {
			if (check == origin&&check != gd.gridDict.end())check++;
			if (check == gd.gridDict.end())break;
			similarityCount = 0;
			grid.turnOnPixel(check->second.startCoords);
			for (int c = 0; c < 5; c++) {
				if (values[c] == grid.liveValue() || values[(c + 1) % 5] == grid.liveValue() || values[(c + 2) % 5] == grid.liveValue()) {
					//cout << values[c] << ",";
					similarityCount++;
				}
				grid.planMove();
				grid.update();
			}
			if (similarityCount >= 3) {
				//cout << "guessing at similar grids\n";
				//cout << "origin";
				//for (int j = 0; j < origin->second.startCoords.size(); j++) {
					//cout << " (" << origin->second.startCoords[j].first << ',' << origin->second.startCoords[j].second << ") ";
				//}
				//cout << endl;
				//cin.get();
				//cout << "check";
				//for (int j = 0; j < check->second.startCoords.size(); j++) {
					//cout << " (" << check->second.startCoords[j].first << ',' << check->second.startCoords[j].second << ") ";
				//}
				//cout << endl;
				//cin.get();
				gd.gridDict.erase(check);
				check = origin;
				if (check != gd.gridDict.end())check++;
				if (check == gd.gridDict.end())break;
			}
		}
	}
}

