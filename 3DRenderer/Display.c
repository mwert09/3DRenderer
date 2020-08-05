#include "Display.h"

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

uint32_t* color_buffer = NULL;
SDL_Texture* color_buffer_texture = NULL;

int WINDOW_WIDTH = 0;
int WINDOW_HEIGHT = 0;

/*
This function initializes SDL
return false if failed
return true if everything went ok
*/
bool InitializeSDL(void) {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("Could not initialize SDL");
	}

	//Display Mode to get currrent monitor resolution
	SDL_DisplayMode display_mode;
	int display_mode_control = SDL_GetCurrentDisplayMode(0, &display_mode);
	WINDOW_WIDTH = display_mode.w;
	WINDOW_HEIGHT = display_mode.h;

	if (display_mode_control != 0) {
		printf("Could not get display mode #%s", SDL_GetError());
		return false;
	}
	// Create window
	window = SDL_CreateWindow("Renderer",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
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

/*Clears the color buffer*/
void ClearColorBuffer(uint32_t color) {
	for (int y = 0; y < WINDOW_HEIGHT; y++) {
		for (int x = 0; x < WINDOW_WIDTH; x++) {
			color_buffer[(WINDOW_WIDTH * y) + x] = color;
		}
	}
}
/*This function is for copying our color buffer to texture and render it*/
void RenderColorBuffer(void) {
	SDL_UpdateTexture(color_buffer_texture, NULL, color_buffer, (int)(sizeof(uint32_t) * WINDOW_WIDTH));
	SDL_RenderCopy(renderer, color_buffer_texture, NULL, NULL);
}
/*Draw a grid on the screen*/
void DrawGrid(void) {
	for (int y = 0; y < WINDOW_HEIGHT; y += 10) {
		for (int x = 0; x < WINDOW_WIDTH; x += 10) {
			DrawPixel(x, y, 0xFF333333);
			//color_buffer[(WINDOW_WIDTH * y) + x] = 0xFF333333;
		}
	}
}

/* Draw a pixel on the screen */
void DrawPixel(int x, int y, uint32_t color) {
	if (x < WINDOW_WIDTH && x >= 0 && y < WINDOW_HEIGHT && y >= 0) {
		color_buffer[(WINDOW_WIDTH * y) + x] = color;
	}
}
/* Draw a rect on the screen */
void DrawRect(int x1, int y1, int x2, int y2, uint32_t color) {
	for (int y = y1; y <= y1 + y2; y ++) {
		for (int x = x1; x <= x1 + x2; x ++) {
			DrawPixel(x, y, color);
			//color_buffer[(WINDOW_WIDTH * y) + x] = 0xFF333333;
		}
	}
}

/* Draw a triangle on the screen */
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) {
	DrawLine(x0, y0, x1, y1, 0xFFFFFFFF);
	DrawLine(x1, y1, x2, y2, 0xFFFFFFFF);
	DrawLine(x0, y0, x2, y2, 0xFFFFFFFF);
}

/* Draw a line on the screen */
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color){
	float step;
	float dx = x1 - x0;
	float dy = y1 - y0;

	step = (abs(dx) > abs(dy) ? abs(dx) : abs(dy));

	float x_inc = dx / step;
	float y_inc = dy / step;

	float x = x0;
	float y = y0;
	int i = 0;
	while (i <= step) {
		DrawPixel(x, y, 0xFFFFFFFF);
		x += x_inc;
		y += y_inc;
		i++;
	}
	

}