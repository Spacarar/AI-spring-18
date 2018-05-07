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

const int GRIDSIZE = 80;

using namespace std;
class Grid {
private:
	SDL_Rect blackRect; //the background black-screen
	int pSize;//size of one 'pixel'
	Pixel pixel[GRIDSIZE][GRIDSIZE];

	//returns an assured safe range (used to loop around grid edges)
	int safeN(int n);


public:
	Grid();

	Grid(int scr_width, int scr_height, int pixel_size);
	~Grid();
	void drawGrid(SDL_Renderer *ren);

	void turnOnPixel(int x, int y);
	void turnOnPixel(std::vector<std::pair<int, int>> cordList);

	//specialized function to draw a glider at the real pixel location of the mouse
	void drawGlider(int offX, int offY);
	//one step to determine whether or not the pixels survive
	void planMove();
	//and one step to actually make the change to the pixels
	void update();

	int neighbors(int x, int y);

	//uses a hash value of the grid to store into the dictionary
	size_t me();

	bool isEmpty();

	//counts the living pixels on the board
	int liveValue();

	void clear();

	//returns the coordinates of all living pixels as a vector
	vector<pair<int, int> > getCoords();
	bool isAlive(int x, int y);

	bool operator==(Grid g);
	//a test used to ensure safeN was working how I intended
	void testSafeN();
};

