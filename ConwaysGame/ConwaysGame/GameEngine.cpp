#include"stdafx.h"
#include "GameEngine.h"
GameEngine::GameEngine() {
	totalFrames = 0;
	FPS = 5;
	grid = Grid(SCR_WIDTH, SCR_HEIGHT, PSIZE);
	gPlayer = GridPlayer(MAX_CYCLE, PIECES_PICKED);
	sTime = mClock::now();
	isRunning = true;
	//gPlayer.start();
	//printf("Time(seconds): %lld \n", ((std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - sTime).count())) / 1000);
	//grid.turnOnPixel(gPlayer.nextFound());
	sTime = lUpdate = lDraw = mClock::now();
	initSDL();
}

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
	if (std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - lUpdate).count() >= (long long)(MILLIS_PER_SEC / (FPS + 1))) {
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

//THREAD HANDLERS
int GameEngine::updateGame(void* self) {
	GameEngine *g = (GameEngine*)self;
	while (g->isRunning) {
		if (g->updateRequired()) {
			g->update();
		}
	}
	return 0;
}
//runs unlimited updates/sec (doesnt call updateRequired)
int GameEngine::updateGameUnlimited(void* self) {
	GameEngine *g = (GameEngine*)self;
	while (g->isRunning) {
		g->update();
	}
	return 0;
}

int GameEngine::renderGame(void* self) {
	GameEngine *g = (GameEngine*)self;
	while (g->isRunning) {
		//if (g->drawRequired()) {
		g->draw();
		//printf("drew\n");
		//}
	}
	return 0;
}
int GameEngine::searchPatterns(void* self) {
	GameEngine *g = (GameEngine*)self;
	g->gPlayer.start();
	return 0;
}

void GameEngine::run() {
	int result1, result2,result3;
	if (FPS < 1000)
		updateThread = SDL_CreateThread(updateGame, "Update", this);
	else
		updateThread = SDL_CreateThread(updateGameUnlimited, "UUpdate", this);

	drawThread = SDL_CreateThread(renderGame, "Render", this);
	searchThread = SDL_CreateThread(searchPatterns, "Search", this);
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
	gPlayer.halt();
	printf("FRAMES: %d FPS: %lld", totalFrames, (totalFrames*MILLIS_PER_SEC) / ((std::chrono::duration_cast<std::chrono::milliseconds>(mClock::now() - sTime).count())));
	SDL_WaitThread(updateThread, &result1);
	SDL_WaitThread(drawThread, &result2);
	SDL_WaitThread(searchThread, &result3);
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
	static int mouseX;
	static int mouseY;
	if (e.type == SDL_MOUSEBUTTONDOWN) {
		SDL_GetMouseState(&mouseX, &mouseY);
		if (e.button.button == SDL_BUTTON_LEFT) {
			grid.clear();
			grid.turnOnPixel(gPlayer.previousFound());
		}
		if (e.button.button == SDL_BUTTON_RIGHT) {
			grid.clear();
			grid.turnOnPixel(gPlayer.nextFound());
			//grid.drawGlider(mouseX, mouseY);
		}
	}
	else if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_0:
			FPS = 0;
			break;
		case SDLK_1:
			FPS = 2;
			break;
		case SDLK_2:
			FPS = 5;
			break;
		case SDLK_3:
			FPS = 10;
			break;
		case SDLK_4:
			FPS = 20;
			break;
		case SDLK_5:
			FPS = 40;
			break;
		case SDLK_6:
			FPS = 80;
			break;
		case SDLK_7:
			FPS = 160;
			break;
		case SDLK_8:
			FPS = 320;
			break;
		case SDLK_9:
			FPS = 640;
			break;
		}
	}
}