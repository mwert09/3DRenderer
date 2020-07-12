#include <stdio.h>
#include <SDL.h>
#include "Display.h"

// is_running to check if SDL initialized failed or not
bool is_running = false;

// Runs only once 
void Setup() {

}

// Render
void Render() {
	/* This will change soon */
	SDL_RenderClear(renderer);
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 1);
	SDL_RenderPresent(renderer);
}

// Process input
void Input() {
	// Event 
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
// Update location rotation scale
void Update() {

}

// Free resouces when we are done
void FreeResources() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
}

int main(int argc, char* argv[]) {
	Setup();

	is_running = InitializeSDL();

	// simple game loop
	while (is_running) {
		Render();
		Input();
		Update();
	}

	FreeResources();

	return 0;
}