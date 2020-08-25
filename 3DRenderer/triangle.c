#include "triangle.h"

void swap(int* a, int* b)
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
		swap(&y0, &y1);
		swap(&x0, &x1);
	}
	if (y1 > y2) {
		swap(&y1, &y2);
		swap(&x1, &x2);
	}
	if (y0 > y1) {
		swap(&y0, &y1);
		swap(&x0, &x1);
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
