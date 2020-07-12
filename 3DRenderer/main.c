#include <stdio.h>
#include <SDL.h>
#include "Display.h"

// is_running to check if SDL initialized failed or not
bool is_running = false;

// Runs only once 
void Setup() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
}

// Render
void Render() {
	/*Let's draw a grid*/

	DrawGrid();

	/*Now we have a color buffer
	  We copy our color buffer to our texture and render it
	*/
	ClearColorBuffer(0xFF000000);
	RenderColorBuffer();
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
	

	is_running = InitializeSDL();

	Setup();

	// simple game loop
	while (is_running) {
		Render();
		Input();
		Update();
	}

	FreeResources();

	return 0;
}