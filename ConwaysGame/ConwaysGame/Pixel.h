#pragma once
#include "stdafx.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>


//CLASSES (WILL BE MOVED SOON)
class Pixel {
private:
	bool status;
	bool syncStatus;
	SDL_Color color; //make it more interesting
	SDL_Rect rect;
public:
	Pixel(int _x = 0, int _y = 0, int _size = 4, bool alive = false, SDL_Color c = { 200, 255, 255 }) {
		rect.x = _x;
		rect.y = _y;
		rect.h = _size;
		rect.w = _size;
		status = alive;
		syncStatus = status;
		color = c;
	}

	void live() {
		syncStatus = true;
	}
	void kill() {
		syncStatus = false;
	}
	void born() {
		syncStatus = true;
	}
	void update() {
		status = syncStatus;
	}
	void drawMe(SDL_Renderer *ren) {
		SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);
		SDL_RenderFillRect(ren, &rect);
	}
	bool isAlive() {
		return status;
	}

};