// Core Libraries
#include <crtdbg.h>
#include <iostream>
#include <Windows.h>

#include "Game.h"

const int FPS = 60;
const int DELAY_TIME = 1000.0f / FPS;

int main(int argc, char *args[]) {

	Uint32 frameStart, frameTime;
	UINT32 frames = 0;

	// show console
	AllocConsole();
	freopen("CON", "w", stdout);

	TheGame::Instance()->init("Dog - Far From Home", 100, 75, 1600, 900, false);

	while (TheGame::Instance()->isRunning()) {

		frameStart = SDL_GetTicks();

		TheGame::Instance()->handleEvents();
		TheGame::Instance()->update();
		TheGame::Instance()->render();

		frameTime = SDL_GetTicks() - frameStart;
		if (frameTime < DELAY_TIME) {
			SDL_Delay(int(DELAY_TIME - frameTime));
		}

		// Delta time
		auto deltaTime = float(SDL_GetTicks() - frameStart) / 1000.0f;
		
		frames++;
		TheGame::Instance()->setFrames(frames);
		TheGame::Instance()->SetDeltaTime(deltaTime);
	}

	TheGame::Instance()->clean();
	return 0;
}

