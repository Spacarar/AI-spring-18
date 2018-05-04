#pragma once
#include "stdafx.h"
#include<iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <chrono>
#include "GridPlayer.h"
typedef std::chrono::steady_clock mClock; 
typedef enum GameState { MAINMENU, SEARCHING, FREEPLAY, EXIT } GameState;


//using namespace std;
const int PSIZE = 7;
const int SCR_WIDTH = PSIZE*GRIDSIZE;
const int SCR_HEIGHT = PSIZE*GRIDSIZE;
const int MILLIS_PER_SEC = 1000;
const int MAX_CYCLE = 1500;
const int PIECES_PICKED = 20;


class GameEngine {
	//debugging tools
	int FPS;
	int totalFrames;
	mClock::time_point sTime;

	//Game Handlers
	bool isRunning;
	GameState gameState;

	//Threads
	SDL_Thread *updateThread, *drawThread;
	SDL_Thread *searchThread;//set gridplayer to search in background for things

	//SDL STUFF
	SDL_Window *gwindow;
	SDL_Renderer *ren;
	mClock::time_point lUpdate, lDraw;

	//MY STUFF
	Grid grid;
	GridPlayer gPlayer;
	void init();
	void initSDL();
	void update();

	//THREAD HANDLERS
	static int updateGame(void* self);
	//runs unlimited updates/sec (doesnt call updateRequired)
	static int updateGameUnlimited(void* self);
	static int renderGame(void* self);
	static int searchPatterns(void* self);

	//FPS Control
	bool updateRequired();
	bool drawRequired();

	void handleEvent(SDL_Event e);
	void draw();


public:
	GameEngine();

	void run();
	void quit();

};

