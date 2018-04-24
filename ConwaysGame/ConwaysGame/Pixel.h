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
	Pixel(int _x = 0, int _y = 0, int _size = 4, bool alive = false, SDL_Color c = { 200, 255, 255 });

	void live();
	void kill();
	void born();
	void update();
	void drawMe(SDL_Renderer *ren);
	bool isAlive();

};