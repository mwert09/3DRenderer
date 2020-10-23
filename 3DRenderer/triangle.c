#include "triangle.h"

void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void float_swap(float* a, float* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void DrawFlatBottom(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color) 
{
	float inv_slope1 = (float)(x1 - x0) / (y1 - y0);
	float inv_slope2 = (float)(x2 - x0) / (y2 - y0);

	float x_start = x0;
	float x_end = x0;

	for (int i = y0; i <= y2; i++) {
		DrawLine(x_start, i, x_end, i, color);
		x_start += inv_slope1;
		x_end += inv_slope2;
	}
}

void DrawFlatTop(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float inv_slope1 = (float)(x2 - x0) / (y2 - y0);
	float inv_slope2 = (float)(x2 - x1) / (y2 - y1);

	float x_start = x2;
	float x_end = x2;

	for (int i = y2; i >= y0; i--) {
		DrawLine(x_start, i, x_end, i, color);
		x_start -= inv_slope1;
		x_end -= inv_slope2;
	}
}

void DrawFilledTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	/*Find top*/
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
	}
	if (y0 == y1) {
		DrawFlatTop(x0, y0, x1, y1, x2, y2, color);
	}
	else if (y1 == y2) {
		DrawFlatBottom(x0, y0, x1, y1, x2, y2, color);
	}
	else {
		int mx = (x2 - x0) * (y1 - y0) / (y2 - y0) + x0;
		int my = y1;

		DrawFlatBottom(x0, y0, x1, y1, mx, my, color);
		DrawFlatTop(x1, y1, mx, my, x2, y2, color);
	}

	
}

vec3_t BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	vec2_t ab = vec2_sub(b, a);
	vec2_t bc = vec2_sub(c, b);
	vec2_t ac = vec2_sub(c, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t bp = vec2_sub(p, b);

	float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);
	float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;
	float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;
	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	return weights;
}

void DrawTexel(int x, int y, uint32_t* texture, vec2_t point_a, vec2_t point_b, vec2_t point_c, float u0, float v0, float u1, float v1, float u2, float v2)
{
	vec2_t point_p = { x,y };
	vec3_t weights = BarycentricWeights(point_a, point_b, point_c, point_p);
	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolated_u = (u0)*alpha + (u1)*beta + (u2)*gamma;
	float interpolated_v = (v0)*alpha + (v1)*beta + (v2)*gamma;

	int tex_x = abs((int)(interpolated_u * texture_width));
	int tex_y = abs((int)(interpolated_v * texture_height));

	DrawPixel(x, y, texture[(texture_width * tex_y) + tex_x]);
}


void DrawTexturedTriangle(int x0, int y0, float u0, float v0, int x1, int y1, float u1, float v1, int x2, int y2, float u2, float v2, uint32_t* texture)
{
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	vec2_t point_a = { x0,y0 };
	vec2_t point_b = { x1,y1 };
	vec2_t point_c = { x2,y2 };

	// TOP TRIANGLE
	float inv_slope1 = 0;
	float inv_slope2 = 0;
	
	if(y1 - y0 != 0)
	{
		inv_slope1 = (float)(x1 - x0) / abs(y1 - y0);
	}
	if(y2 - y0 != 0)
	{
		inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);
	}
	if(y1-y0 != 0)
	{
		for(int y = y0; y <= y1; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end = x0 + (y - y0) * inv_slope2;
			if(x_end < x_start)
			{
				int_swap(&x_end, &x_start);
			}
			for(int x = x_start; x < x_end; x++)
			{
				DrawTexel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
			}
		}
	}

	// BOTTOM TRIANGLE
	inv_slope1 = 0;
	inv_slope2 = 0;

	if (y2 - y1 != 0)
	{
		inv_slope1 = (float)(x2 - x1) / abs(y2 - y1);
	}
	if (y2 - y0 != 0)
	{
		inv_slope2 = (float)(x2 - x0) / abs(y2 - y0);
	}
	if (y2 - y1 != 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			int x_start = x1 + (y - y1) * inv_slope1;
			int x_end = x0 + (y - y0) * inv_slope2;
			if (x_end < x_start)
			{
				int_swap(&x_end, &x_start);
			}
			for (int x = x_start; x < x_end; x++)
			{
				DrawTexel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
			}
		}
	}
}
