#pragma once
#include "stdafx.h"

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#include<iostream>
#include<string>
#include<math.h>
#include<vector>

#include "Pixel.h"

const int GRIDSIZE = 30;

using namespace std;
class Grid {
private:
	SDL_Rect blackRect; //the background black-screen
	Pixel pixel[GRIDSIZE][GRIDSIZE];
	
	//returns an assured safe range (used to loop around grid edges)
	int safeN(int n) {
		if (n < 0)return(GRIDSIZE - ((abs(n)) % GRIDSIZE));
		else return n%GRIDSIZE;
	}

	int pSize;//size of one 'pixel'

public:
	Grid() {
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

	Grid(int scr_width, int scr_height, int pixel_size) {
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
	
	void drawGrid(SDL_Renderer *ren) {
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

	void turnOnPixel(int x, int y) {
		pixel[safeN(x)][safeN(y)].born();
		pixel[safeN(x)][safeN(y)].update();
	}
	void turnOnPixel(std::vector<std::pair<int, int>> cordList) {
		for (int i = 0; i < cordList.size(); i++) {
			turnOnPixel(cordList[i].first, cordList[i].second);
		}
	}

	//specialized function to draw a glider at the real pixel location of the mouse
	void drawGlider(int offX, int offY) {
		offX = (offX) % (GRIDSIZE*pSize);
		offY = (offY) % (GRIDSIZE*pSize);
		turnOnPixel(offX + 1, offY);
		turnOnPixel(offX + 2, offY + 1);
		turnOnPixel(offX, offY + 2);
		turnOnPixel(offX + 1, offY + 2);
		turnOnPixel(offX + 2, offY + 2);
	}

	//one step to determine whether or not the pixels survive
	void planMove() {
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
	void update() {
		int me = 0;
		int ee = 0;
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y < GRIDSIZE; y++) {
				pixel[x][y].update();
			}
		}
	}

	int neighbors(int x, int y) {
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
	size_t me() {
		static std::string representation = "";
		std::hash<string> hashThis;
		representation.clear();
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y < GRIDSIZE; y++) {
				if (pixel[x][y].isAlive())representation.append(to_string(x)+","+to_string(y)+"|");
				//else representation.append("0");
			}
		}
		//cout << representation << endl;
		//unsigned char* rp = new unsigned char[representation.size()];
		//for (int i = 0; i > representation.size(); i++) {
			//rp[i] = representation[i];
		//}
		return hashThis(representation);
	}
	
	bool isEmpty() {
		for (int x = 0; x < GRIDSIZE; x++) {
			for(int y=0;y<GRIDSIZE;y++){
				if (pixel[x][y].isAlive()) return false;
			}
		}
		return true;
	}
	
	//counts the living pixels on the board
	int liveValue() {
		int count = 0;
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y<GRIDSIZE; y++) {
				if (pixel[x][y].isAlive()) {
					//cout << "pixel(" << x << "," << y << ") is alive" << endl;
					count++;
				}
			}
		}
		return count;
	}
	
	void clear() {
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y < GRIDSIZE; y++) {
				pixel[x][y].kill();
				pixel[x][y].update();
			}
		}
	}
	
	//returns the coordinates of all living pixels as a vector
	vector<pair<int, int> > getCoords() {
		vector<pair<int, int> > coords;
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y < GRIDSIZE; y++) {
				if (pixel[x][y].isAlive())coords.push_back(make_pair(x, y));
			}
		}
		return coords;
	}

	//a test used to ensure safeN was working how I intended
	void testSafeN() {
		for (int i = -4; i < GRIDSIZE + 5; i++) {
			printf("%d=%d", i, safeN(i));
			std::cin.get();
		}
	}
};

