#pragma once
#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "Pixel.h"

const int GRIDSIZE = 50;


class Grid {
private:
	SDL_Rect blackRect;
	Pixel pixel[GRIDSIZE][GRIDSIZE];
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
				pixel[i][j] = Pixel(i*4, j*4, 4, false);
			}
		}

	}
	Grid(int scr_width, int scr_height, int pixel_size) {
		blackRect.x = 0;
		blackRect.y = 0;
		blackRect.w = scr_width;
		blackRect.h = scr_height;
		pixel_size<=0 ?pSize = 1:pSize=pixel_size;
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
		if (x >= 0 && x <= GRIDSIZE) {
			if (y >= 0 && y <= GRIDSIZE) {
				pixel[x][y].born();
			}
		}
	}

	void drawGlider(int offX, int offY) {
		if (offX < 0)offX = 0;
		if (offY < 0)offY = 0;
		if (offX > blackRect.w)offX = blackRect.w;
		if (offY > blackRect.h)offY = blackRect.h;
		
		offX = int(offX / pSize);
		offY = int(offY / pSize);
		while (offX > GRIDSIZE) {
			offX -= GRIDSIZE;
		}
		while (offY > GRIDSIZE) {
			offY -= GRIDSIZE;
		}
		printf("offx: %d, offY: %d", offX, offY);
		//x+1 goes out of bounds
		if (offX + 1 >= GRIDSIZE) {
			if (offY + 1 >= GRIDSIZE) {
				turnOnPixel(0, offY);
				turnOnPixel(1, 0);
				turnOnPixel(offX, 1);
				turnOnPixel(0, 1);
				turnOnPixel(1, 1);
			}
			else if (offY + 2 >= GRIDSIZE) {
				turnOnPixel(0, offY);
				turnOnPixel(1, offY + 1);
				turnOnPixel(offX, 0);
				turnOnPixel(0, 0);
				turnOnPixel(1, 0);
			}
			//y is within range to fit shape
			else {
				turnOnPixel(0, offY);
				turnOnPixel(1, offY + 1);
				turnOnPixel(offX, offY + 2);
				turnOnPixel(0, offY + 2);
				turnOnPixel(1, offY + 2);
			}

		}
		//x+2 goes out of bounds
		else if (offX + 2 >= GRIDSIZE) {
			if (offY + 1 >= GRIDSIZE) {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(0, 0);
				turnOnPixel(offX, 1);
				turnOnPixel(offX + 1,1);
				turnOnPixel(0, 1);
			}
			else if (offY + 2 >= GRIDSIZE) {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(0, offY + 1);
				turnOnPixel(offX, 0);
				turnOnPixel(offX + 1,0);
				turnOnPixel(0, 0);
			}
			//y is within range to fit shape
			else {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(0, offY + 1);
				turnOnPixel(offX, offY + 2);
				turnOnPixel(offX + 1, offY + 2);
				turnOnPixel(0, offY + 2);
			}
		}
		//x is within range to fit shape
		else {
			if (offY + 1 >= GRIDSIZE) {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(offX + 2, 0);
				turnOnPixel(offX, 1);
				turnOnPixel(offX + 1, 1);
				turnOnPixel(offX + 2, 1);
			}
			else if (offY + 2 >= GRIDSIZE) {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(offX + 2, offY + 1);
				turnOnPixel(offX, 0);
				turnOnPixel(offX + 1,0);
				turnOnPixel(offX + 2, 0);
			}
			//y is within range to fit shape
			else {
				turnOnPixel(offX + 1, offY);
				turnOnPixel(offX + 2, offY + 1);
				turnOnPixel(offX, offY + 2);
				turnOnPixel(offX + 1, offY + 2);
				turnOnPixel(offX + 2, offY + 2);
			}
		}
		

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
		//everybody else
		int ee = 0;
		//left
		if (x - 1 < 0) {
			//up
			if (y - 1 < 0) {
				if (pixel[GRIDSIZE - 1][GRIDSIZE - 1].isAlive())ee++;
			}
			else {
				if (pixel[GRIDSIZE - 1][y - 1].isAlive())ee++;
			}
			//current
			if (pixel[GRIDSIZE - 1][y].isAlive())ee++;
			//down
			if (y + 1 >= GRIDSIZE) {
				if (pixel[GRIDSIZE - 1][0].isAlive())ee++;
			}
			else {
				if (pixel[GRIDSIZE - 1][y + 1].isAlive())ee++;
			}
			
		}
		//left
		else {
			//up
			if (y - 1 < 0) {
				if (pixel[x-1][GRIDSIZE - 1].isAlive())ee++;
			}
			else {
				if (pixel[x-1][y - 1].isAlive())ee++;
			}
			//current
			if (pixel[x - 1][y].isAlive())ee++;
			//down
			if (y + 1 >= GRIDSIZE) {
				if (pixel[GRIDSIZE - 1][0].isAlive())ee++;
			}
			else {
				if (pixel[x - 1][y + 1].isAlive())ee++;
			}
		}
		//right
		if (x + 1 >= GRIDSIZE) {
			//up
			if (y - 1 < 0) {
				if (pixel[0][GRIDSIZE - 1].isAlive())ee++;
			}
			else {
				if (pixel[0][y - 1].isAlive())ee++;
			}
			//current
			if (pixel[0][y].isAlive())ee++;
			//down
			if (y + 1 >= GRIDSIZE) {
				if (pixel[0][0].isAlive())ee++;
			}
			else {
				if (pixel[0][y + 1].isAlive())ee++;
			}
		}
		//right
		else {
			//up
			if (y - 1 < 0) {
				if (pixel[x+1][GRIDSIZE - 1].isAlive())ee++;
			}
			else {
				if (pixel[x+ 1][y - 1].isAlive())ee++;
			}
			//current
			if (pixel[x+ 1][y].isAlive())ee++;
			//down
			if (y + 1 >= GRIDSIZE) {
				if (pixel[x+ 1][0].isAlive())ee++;
			}
			else {
				if (pixel[x+ 1][y + 1].isAlive())ee++;
			}

		}
		//did x-1 then x+1 first. x does not require a surrounding if
		if (y - 1 < 0) {
			if (pixel[x][GRIDSIZE - 1].isAlive())ee++;
		}
		else {
			if (pixel[x][y - 1].isAlive())ee++;
		}
		if (y + 1 >= GRIDSIZE) {
			if (pixel[x][0].isAlive())ee++;
		}
		else {
			if (pixel[x][y + 1].isAlive())ee++;
		}
	return ee;
	}
};
