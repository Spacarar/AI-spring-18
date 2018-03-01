// ConwaysGame.cpp : Defines the entry point for the console application.
//
#include"stdafx.h"
#include <stdio.h>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameEngine.h"

int main(int argc, char* argv[]){
	GameEngine *g = new GameEngine();
	g->run();
	return 0;


}