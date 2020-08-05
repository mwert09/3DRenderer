#ifndef DISPLAY_H
#define DISPLAY_H

#include <stdio.h>
#include <stdbool.h>
#include <SDL.h>

#define TARGET_FRAME 60
#define TARGET_FRAME_TIME (1000/TARGET_FRAME)

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
//Draw a rect on the screen
void DrawRect(int x1, int y1, int x2, int y2, uint32_t color);
//Draw a triangle on the screen
void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
//Draw a line on the screen
void DrawLine(int x0, int y0, int x1, int y1, uint32_t color);

#endif