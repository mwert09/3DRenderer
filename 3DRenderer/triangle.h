#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>
#include "Display.h"

/*Type definition for face*/
typedef struct {
	int a;
	int b;
	int c;
}face_t;

/*Type definition for triangle*/
typedef struct {
	vec2_t points[3];
}triangle_t;

/* Function to swap two integer pointer data */
void swap(int* a, int* b);
/* Functions to draw filled triangle */
void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawFlatBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawFlatTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);

#endif