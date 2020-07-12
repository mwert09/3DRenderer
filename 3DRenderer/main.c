#include <stdio.h>
#include <SDL.h>
#include "Display.h"

bool is_running = false;


void Setup() {

}

void Render() {
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
	SDL_RenderPresent(renderer);
}

void Input() {
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
		}
		break;
	}
}

void Update() {

}

void FreeResources() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

int main(int argc, char* argv[]) {
	Setup();

	is_running = InitializeSDL();

	while (is_running) {
		Render();
		Input();
		Update();
	}

	FreeResources();

	return 0;
}