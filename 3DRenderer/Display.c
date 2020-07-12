#include "Display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

/*
This function initializes SDL
return false if failed
return true if everything went ok
*/
bool InitializeSDL() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Could not initialize SDL");
	}

	//Display Mode to get currrent monitor resolution
	SDL_DisplayMode display_mode;
	int display_mode_control = SDL_GetCurrentDisplayMode(0, &display_mode);

	if (display_mode_control != 0) {
		printf("Could not get display mode #%s", SDL_GetError());
		return false;
	}
	// Create window
	window = SDL_CreateWindow("Renderer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		display_mode.w,
		display_mode.h,
		SDL_WINDOW_BORDERLESS);

	// Check if window is null
	if (!window) {
		printf("Could not create window");
		return false;
	}

	// Create renderer
	renderer = SDL_CreateRenderer(window, -1, 0);

	if (!renderer) {
		printf("Could not create renderer");
		return false;
	}

	return true;
}