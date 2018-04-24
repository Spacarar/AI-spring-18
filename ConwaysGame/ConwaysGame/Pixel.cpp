#include"stdafx.h"
#include"Pixel.h"

Pixel::Pixel(int _x, int _y, int _size, bool alive, SDL_Color c) {
	rect.x = _x;
	rect.y = _y;
	rect.h = _size;
	rect.w = _size;
	status = alive;
	syncStatus = status;
	color = c;
}

void Pixel::live() {
	syncStatus = true;
}
void Pixel::kill() {
	syncStatus = false;
}
void Pixel::born() {
	syncStatus = true;
}
void Pixel::update() {
	status = syncStatus;
}
void Pixel::drawMe(SDL_Renderer *ren) {
	SDL_SetRenderDrawColor(ren, color.r, color.g, color.b, 255);
	SDL_RenderFillRect(ren, &rect);
}
bool Pixel::isAlive() {
	return status;
}