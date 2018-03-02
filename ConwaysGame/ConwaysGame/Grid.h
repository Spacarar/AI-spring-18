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
	//safer pixel returning function
	Pixel pix(int x, int y) {
		int ogx;
		int ogy;
		if (x < 0)
			ogx = ((abs(x) - 1) % GRIDSIZE);
		else if (x >= GRIDSIZE)
			ogx = (x%GRIDSIZE);
		else
			ogx = x;
		if (y < 0)
			ogy = ((abs(y) - 1) % GRIDSIZE);
		else if (y >= GRIDSIZE)
			ogy = (y%GRIDSIZE);
		else
			ogy = y;
		return pixel[ogx][ogy];
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
		pix(x, y).born();
	}

	void drawGlider(int offX, int offY) {
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
		if (pix(x - 1, y).isAlive())ee++;
		if (pix(x - 1, y - 1).isAlive())ee++;
		if (pix(x - 1, y + 1).isAlive())ee++;
		if (pix(x + 1, y).isAlive())ee++;
		if (pix(x + 1, y - 1).isAlive())ee++;
		if (pix(x + 1, y + 1).isAlive())ee++;
		if (pix(x, y - 1).isAlive())ee++;
		if (pix(x, y + 1).isAlive())ee++;
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

};

