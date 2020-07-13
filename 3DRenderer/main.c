#include <stdio.h>
#include <SDL.h>
#include "Display.h"
#include "vector.h"

#define N_POINTS (9*9*9)
/* Cube vectors for testing */
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];

/* Simple field of view factor */
int fov_factor = 1280;

/* Camera Position temp */
vec3_t camera_pos = { .x = 0, .y = 0, .z = -5 };

// is_running to check if SDL initialized failed or not
bool is_running = false;

/* Simple perspective projection */
vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (point.x * fov_factor) / point.z,
		.y = (point.y * fov_factor) / point.z
	};
	return projected_point;
}

// Runs only once 
void Setup() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	/* Initialize cube points array for testing */
	int point_count = 0;
	for (float x = -1; x <= 1; x+=0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t point = { .x = x, .y = y, .z = z };
				cube_points[point_count] = point;
				point_count++;
			}
		}
	}

}

// Render
void Render() {
	/*Let's draw a grid*/
	DrawGrid();

	/* Testing cube */
	for (int i = 0; i < N_POINTS; i++) {
		vec2_t current_point = projected_points[i];
		DrawRect(current_point.x + (WINDOW_WIDTH / 2),
			current_point.y + (WINDOW_HEIGHT / 2),
			4,
			4,
			0xFFFFFFFF);
	}

	/*Now we have a color buffer
	  We copy our color buffer to our texture and render it
	*/
	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);
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

	/* Initialize projected points(vectors) for testing */
	for (int i = 0; i < N_POINTS; i++) {
		vec3_t point = cube_points[i];
		point.z -= camera_pos.z;
		vec2_t projected_point = project(point);
		projected_points[i] = projected_point;
	}

}

// Free resouces when we are done
void FreeResources() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	free(color_buffer);
	SDL_DestroyTexture(color_buffer_texture);
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