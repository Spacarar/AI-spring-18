#include"stdafx.h"

#include"Grid.h"

int Grid::safeN(int n) {
	if (n < 0)return(GRIDSIZE - ((abs(n)) % GRIDSIZE));
	else return n%GRIDSIZE;
}


Grid::Grid() {
	blackRect.h = 0;
	blackRect.w = 0;
	blackRect.x = 0;
	blackRect.y = 0;
	pSize = 1;
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			pixel[i][j] = Pixel(i * 4, j * 4, 4, false);
		}
	}

}

Grid::Grid(int scr_width, int scr_height, int pixel_size) {
	Uint8 r, g, b; //currently set to be shades of blue-purple
	blackRect.x = 0;
	blackRect.y = 0;
	blackRect.w = scr_width;
	blackRect.h = scr_height;
	b = 255;
	pixel_size <= 0 ? pSize = 1 : pSize = pixel_size;

	//put the pixels where they belong
	for (int i = 0; i < GRIDSIZE; i++) {
		r = rand() % 206 + 50;
		g = rand() % r + 50;
		for (int j = 0; j < GRIDSIZE; j++) {
			pixel[i][j] = Pixel(i*pSize, j*pSize, pSize, false);
			//pixel[i][j] = Pixel(i*pSize, j*pSize, pSize, false, { r,g,b });
		}
	}
}
Grid::~Grid() {
	//delete pixel;
}
void Grid::drawGrid(SDL_Renderer *ren) {
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_RenderFillRect(ren, &blackRect);
	for (int i = 0; i < GRIDSIZE; i++) {
		for (int j = 0; j < GRIDSIZE; j++) {
			if (pixel[i][j].isAlive()) {
				pixel[i][j].drawMe(ren);
			}
		}
	}
}

void Grid::turnOnPixel(int x, int y) {
	pixel[safeN(x)][safeN(y)].born();
	pixel[safeN(x)][safeN(y)].update();
}
void Grid::turnOnPixel(std::vector<std::pair<int, int>> cordList) {
	if (cordList.size() < 2 || &cordList == nullptr)return;
	for (int i = 0; i < cordList.size(); i++) {
		turnOnPixel(cordList[i].first, cordList[i].second);
	}
}

//specialized function to draw a glider at the real pixel location of the mouse
void Grid::drawGlider(int offX, int offY) {
	offX = (offX) % (GRIDSIZE*pSize);
	offY = (offY) % (GRIDSIZE*pSize);
	turnOnPixel(offX + 1, offY);
	turnOnPixel(offX + 2, offY + 1);
	turnOnPixel(offX, offY + 2);
	turnOnPixel(offX + 1, offY + 2);
	turnOnPixel(offX + 2, offY + 2);
}

//one step to determine whether or not the pixels survive
void Grid::planMove() {
	int me = 0;
	int ee = 0;
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			me = pixel[x][y].isAlive();
			ee = neighbors(x, y);
			if (me) {
				if (ee < 2 || ee>3) {
					pixel[x][y].kill();
				}
			}
			else {
				if (ee == 3) {
					pixel[x][y].born();
				}
			}
		}
	}
}
//and one step to actually make the change to the pixels
void Grid::update() {
	int me = 0;
	int ee = 0;
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			pixel[x][y].update();
		}
	}
}

int Grid::neighbors(int x, int y) {
	int ee = 0;
	if (pixel[safeN(x - 1)][safeN(y)].isAlive())ee++;
	if (pixel[safeN(x - 1)][safeN(y - 1)].isAlive())ee++;
	if (pixel[safeN(x - 1)][safeN(y + 1)].isAlive())ee++;
	if (pixel[safeN(x + 1)][safeN(y)].isAlive())ee++;
	if (pixel[safeN(x + 1)][safeN(y - 1)].isAlive())ee++;
	if (pixel[safeN(x + 1)][safeN(y + 1)].isAlive())ee++;
	if (pixel[safeN(x)][safeN(y - 1)].isAlive())ee++;
	if (pixel[safeN(x)][safeN(y + 1)].isAlive())ee++;
	return ee;
}

//uses a hash value of the grid to store into the dictionary
size_t Grid::me() {
	static std::string representation = "";
	std::hash<string> hashThis;
	representation.clear();
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			if (pixel[x][y].isAlive())representation.append(to_string(x) + "," + to_string(y) + "|");
			//else representation.append("0");
		}
	}
	//cout << representation << endl;
	//unsigned char* rp = new unsigned char[representation.size()];
	//for (int i = 0; i > representation.size(); i++) {
	//rp[i] = representation[i];
	//}
	//cout << representation << endl;
	//cout << hashThis(representation) << endl;
	return hashThis(representation);
}

bool Grid::isEmpty() {
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			if (pixel[x][y].isAlive()) return false;
		}
	}
	return true;
}

//counts the living pixels on the board
int Grid::liveValue() {
	int count = 0;
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			if (pixel[x][y].isAlive()) {
				//cout << "pixel(" << x << "," << y << ") is alive" << endl;
				count++;
			}
		}
	}
	return count;
}

void Grid::clear() {
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			pixel[x][y].kill();
			pixel[x][y].update();
		}
	}
}

//returns the coordinates of all living pixels as a vector
vector<pair<int, int> > Grid::getCoords() {
	vector<pair<int, int> > coords;
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			if (pixel[x][y].isAlive())coords.push_back(make_pair(x, y));
		}
	}
	return coords;
}
bool Grid::isAlive(int x, int y) {
	return pixel[x][y].isAlive();
}

bool Grid::operator==(Grid g) {
	for (int x = 0; x < GRIDSIZE; x++) {
		for (int y = 0; y < GRIDSIZE; y++) {
			if (pixel[x][y].isAlive() && !g.pixel[x][y].isAlive())return false;
		}
	}
	return true;
}
//a test used to ensure safeN was working how I intended
void Grid::testSafeN() {
	for (int i = -4; i < GRIDSIZE + 5; i++) {
		printf("%d=%d", i, safeN(i));
		std::cin.get();
	}
}