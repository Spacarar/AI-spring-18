#pragma once
#include "stdafx.h"
#include<iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include<chrono>
#include "Pixel.h"
#include "Grid.h"
typedef std::chrono::steady_clock mClock; 


//using namespace std;

const int SCR_WIDTH = 1080;
const int SCR_HEIGHT = 720;
const int PSIZE = 10;
const int MILLIS_PER_SEC = 1000;
const int FPS = 2;

bool isRunning = true;


class GameEngine {
	//debugging tools
	int totalFrames;
	mClock::time_point sTime;

	SDL_Thread *updateThread, *drawThread;
	//SDL STUFF
	SDL_Window *gwindow;
	SDL_Renderer *ren;
	mClock::time_point lUpdate, lDraw;

	//MY STUFF
	Grid grid;
	void init();
	void initSDL();
	void update();

	//THREAD HANDLERS
	static int updateGame(void* self) {
		GameEngine *g = (GameEngine*)self;
		while (isRunning) {
			if (g->updateRequired()) {
				g->update();
			}
		}
		return 0;
	}
	//runs unlimited updates/sec (doesnt call updateRequired)
	static int updateGameUnlimited(void* self) {
		GameEngine *g = (GameEngine*)self;
		while (isRunning) {
			g->update();
		}
		return 0;
	}
	
	static int renderGame(void* self) {
		GameEngine *g = (GameEngine*)self;
		while (isRunning) {
			//if (g->drawRequired()) {
				g->draw();
				//printf("drew\n");
			//}
		}
		return 0;
	}

	bool updateRequired();
	bool drawRequired();
	void handleEvent(SDL_Event e);
	void draw();
	void quit();


public:
	GameEngine() {
		initSDL();
		totalFrames = 0;
		grid = Grid(SCR_WIDTH, SCR_HEIGHT, PSIZE);
		grid.turnOnPixel(1, 0);
		grid.turnOnPixel(1, 1);
		grid.turnOnPixel(1, 2);
		grid.drawGlider(500, 500);
		sTime = lUpdate = lDraw = mClock::now();
	}

	void run();
};

void GameEngine::init() {
	initSDL();
	grid = Grid(SCR_WIDTH, SCR_HEIGHT, PSIZE);
	grid.turnOnPixel(1, 1);
	grid.turnOnPixel(1, 2);
	grid.turnOnPixel(1, 3);
	sTime = lUpdate = lDraw = mClock::now();
	//printf("init called");
}
void GameEngine::initSDL() {
	if ((SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) == -1)) {
		printf("Could not initialize SDL: %s.\n", SDL_GetError());
		exit(-1);
	}
	gwindow = SDL_CreateWindow("Conway's Game of Life", 50, 50, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
	if (gwindow == nullptr) {
		printf("Could not create window!");
	}
	ren = SDL_CreateRenderer(gwindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		SDL_DestroyWindow(gwindow);
		printf("SDL_CreateRenderer Error: %s\n", SDL_GetError());
		SDL_Quit();
	}
	//Initialize SDL_mixer
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
		printf("SDL2_mixer init error\n CLOSING...");
		SDL_Quit();
	}
	if (TTF_Init() == -1) {
		printf("SDL2_ttf init error\n CLOSING...");
		SDL_Quit();
	}
}

void GameEngine::update() {
	totalFrames++;
	grid.planMove();
	grid.update();
}
void GameEngine::draw() {
	SDL_RenderClear(ren);
	grid.drawGrid(ren);
	SDL_RenderPresent(ren);
}

//fps limitation and update limitation (useful to look at)
bool GameEngine::updateRequired() {
	if (std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - lUpdate).count() >= (long long)(MILLIS_PER_SEC / FPS)) {
		//printf("DIFFERENCE: %d \n NEEDED: %d\n", std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - lUpdate).count(), (long long)(MILLIS_PER_SEC / FPS));
		lUpdate = mClock::now();
		return true;
	}
	return false;
}
bool GameEngine::drawRequired() {
	if (std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - lDraw).count() > (long long)(MILLIS_PER_SEC / FPS)) {
		//printf("DIFFERENCE: %d \n NEEDED: %d\n", std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - lDraw).count(), (long long)(MILLIS_PER_SEC / FPS));
		lDraw = mClock::now();
		return true;
	}
	return false;
}

void GameEngine::run() {
	int result1, result2;
	if (FPS < 1000)
		updateThread = SDL_CreateThread(updateGame, "Update", this);
	else
		updateThread = SDL_CreateThread(updateGameUnlimited, "UUpdate", this);

	drawThread = SDL_CreateThread(renderGame, "Render", this);
	while (isRunning) {
		SDL_Event event;
		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_WINDOWEVENT) {
				if (event.window.event == SDL_WINDOWEVENT_CLOSE)
					isRunning = false;
			}
			if (event.type == SDL_KEYDOWN) {
				if (event.key.keysym.sym == SDLK_ESCAPE)
					isRunning = false;
			}
			if (isRunning) handleEvent(event);
		}
	}
	printf("FRAMES: %d FPS: %lld", totalFrames, (totalFrames*MILLIS_PER_SEC) / ((std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - sTime).count())));
	SDL_WaitThread(updateThread, &result1);
	SDL_WaitThread(drawThread, &result2);
	quit();
	std::cin.get();
}
void GameEngine::quit() {
	SDL_DestroyRenderer(ren);
	SDL_DestroyWindow(gwindow);
	Mix_Quit();
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

}

void GameEngine::handleEvent(SDL_Event e) {

}