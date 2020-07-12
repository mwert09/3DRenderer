#ifndef DISPLAY_H
#define DISPLAY_H
#endif
#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

extern int WINDOW_WIDTH;
extern int WINDOW_HEIGHT;

extern SDL_Window* window;
extern SDL_Renderer* renderer;

extern uint32_t* color_buffer;
extern SDL_Texture* color_buffer_texture;

//Initialize SDL
bool InitializeSDL(void);
//Clear color buffer
void ClearColorBuffer(uint32_t color);
//Render color buffer
void RenderColorBuffer(void);
//Draw a grid on the screen
void DrawGrid(void);
//Draw a pixel on the screen
void DrawPixel(int x, int y, uint32_t color);
