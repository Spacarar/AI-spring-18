#pragma once
#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<iostream>
#include<string>
#include<math.h>
#include "Pixel.h"

const int GRIDSIZE = 50;


class Grid {
private:
	SDL_Rect blackRect;
	Pixel pixel[GRIDSIZE][GRIDSIZE];
	//returns an assured safe range (used to loop around grid edges)
	int safeN(int n) {
		if (n < 0)return(GRIDSIZE - ((abs(n)) % GRIDSIZE));
		else return n%GRIDSIZE;
	}
	int pSize;
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
		blackRect.x = 0;
		blackRect.y = 0;
		blackRect.w = scr_width;
		blackRect.h = scr_height;
		pixel_size <= 0 ? pSize = 1 : pSize = pixel_size;
		for (int i = 0; i < GRIDSIZE; i++) {
			for (int j = 0; j < GRIDSIZE; j++) {
				pixel[i][j] = Pixel(i*pSize, j*pSize, pSize, false);
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
	}

	void drawGlider(int offX, int offY) {
		offX = (offX) % (GRIDSIZE*pSize);
		offY = (offY) % (GRIDSIZE*pSize);
		turnOnPixel(offX + 1, offY);
		turnOnPixel(offX + 2, offY + 1);
		turnOnPixel(offX, offY + 2);
		turnOnPixel(offX + 1, offY + 2);
		turnOnPixel(offX + 2, offY + 2);
	}

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

	std::string me() {
		std::string representation = "";
		for (int x = 0; x < GRIDSIZE; x++) {
			for (int y = 0; y < GRIDSIZE; y++) {
				if (pixel[x][y].isAlive())representation += '1';
				else representation += '0';
			}
		}
	}
	void testSafeN() {
		for (int i = -4; i < GRIDSIZE + 5; i++) {
			printf("%d=%d", i, safeN(i));
			std::cin.get();
		}
	}
};

