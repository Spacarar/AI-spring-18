// ConwaysGame.cpp : Defines the entry point for the console application.
//
#include"stdafx.h"
#include <iostream>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameEngine.h"


int main(int argc, char* argv[]){
	GameEngine *g = new GameEngine();
	//GridPlayer player(100, 15);
	//player.start();
	g->run();
	cout << "\n end of run\n";
	g->quit();
	cin.get();
	delete g;
	return 0;


}

/*
implement the idea of fitness for the grid
develop a mutation step and/or a sex step
sex step, try to combine fit grids to develop new grids
mutation based on fitness perhaps
generate a culling step, removing unfit values. move toward next generation.
geometric average for fitness
*/