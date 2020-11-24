#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"
#include <stdint.h>
#include "Display.h"
#include "texture.h"
#include "Display.h"

/*Type definition for face*/
typedef struct {
	int a;
	int b;
	int c;
	tex2_t a_uv;
	tex2_t b_uv;
	tex2_t c_uv;
	uint32_t color;
}face_t;

/*Type definition for triangle*/
typedef struct {
	vec4_t points[3];
	tex2_t texcoords[3];
	uint32_t color;
}triangle_t;

/* Function to swap two integer pointer data */
void swap(int* a, int* b);
/* Functions to draw filled triangle */
void DrawFilledTriangle(int x0, int y0, float z0, float w0, int x1, int y1, float z1, float w1, int x2, int y2, float z2, float w2, uint32_t color);
void DrawTexturedTriangle(int x0, int y0, float z0, float w0, float u0, float v0,
						  int x1, int y1, float z1, float w1, float u1, float v1,
					      int x2, int y2, float z2, float w2, float u2, float v2, uint32_t* texture);
void DrawFlatBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawFlatTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color);
void DrawTexel(int x, int y, uint32_t* texture, vec2_t point_a, vec2_t point_b, vec2_t point_c, float u0, float v0, float u1, float v1, float u2, float v2);
void DrawTrianglePixel(int x, int y, uint32_t color, vec4_t point_a, vec4_t point_b, vec4_t point_c);
vec3_t BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p);

#endif