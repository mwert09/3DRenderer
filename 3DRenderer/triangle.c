#include "triangle.h"

void int_swap(int* a, int* b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}

void float_swap(float* a, float* b)
{
	float temp = *a;
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

void DrawFilledTriangle(int x0, int y0, float z0, float w0, int x1, int y1, float z1, float w1, int x2, int y2, float z2, float w2, uint32_t color)
{
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
	}

	// Create vector points and texture coords after we sort the vertices
	vec4_t point_a = { x0, y0, z0, w0 };
	vec4_t point_b = { x1, y1, z1, w1 };
	vec4_t point_c = { x2, y2, z2, w2 };

	///////////////////////////////////////////////////////
	// Render the upper part of the triangle (flat-bottom)
	///////////////////////////////////////////////////////
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x <= x_end; x++) {
				// Draw our pixel with the color that comes from the texture
				DrawTrianglePixel(x, y, color, point_a, point_b, point_c);
			}
		}
	}

	///////////////////////////////////////////////////////
	// Render the bottom part of the triangle (flat-top)
	///////////////////////////////////////////////////////
	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x <= x_end; x++) {
				// Draw our pixel with the color that comes from the texture
				DrawTrianglePixel(x, y, color, point_a, point_b, point_c);
			}
		}
	}
	
}

void DrawTrianglePixel(int x, int y, uint32_t color, vec4_t point_a, vec4_t point_b, vec4_t point_c)
{
	vec2_t p = { x, y };
	vec2_t a = vec2_from_vec4(point_a);
	vec2_t b = vec2_from_vec4(point_b);
	vec2_t c = vec2_from_vec4(point_c);

	vec3_t weights = BarycentricWeights(a, b, c, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	const float EPSILON = 0.00001;
	if (alpha < -EPSILON || beta < -EPSILON || gamma < -EPSILON) {
		return;
	}

	float interpolated_reciprocal_w;

	// Also interpolate the value of 1/w for the current pixel
	interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

	interpolated_reciprocal_w = 1 - interpolated_reciprocal_w;

	if (interpolated_reciprocal_w < z_buffer[(WINDOW_WIDTH * y) + x])
	{
		DrawPixel(x, y, color);
		z_buffer[(WINDOW_WIDTH * y) + x] = interpolated_reciprocal_w;
	}
}


vec3_t BarycentricWeights(vec2_t a, vec2_t b, vec2_t c, vec2_t p)
{
	// Find the vectors between the vertices ABC and point p
	vec2_t ab = vec2_sub(b, a);
	vec2_t bc = vec2_sub(c, b);
	vec2_t ac = vec2_sub(c, a);
	vec2_t ap = vec2_sub(p, a);
	vec2_t bp = vec2_sub(p, b);

	// Calcualte the area of the full triangle ABC using cross product (area of parallelogram)
	float area_triangle_abc = (ab.x * ac.y - ab.y * ac.x);

	// Weight alpha is the area of subtriangle BCP divided by the area of the full triangle ABC
	float alpha = (bc.x * bp.y - bp.x * bc.y) / area_triangle_abc;

	// Weight beta is the area of subtriangle ACP divided by the area of the full triangle ABC
	float beta = (ap.x * ac.y - ac.x * ap.y) / area_triangle_abc;

	// Weight gamma is easily found since barycentric cooordinates always add up to 1
	float gamma = 1 - alpha - beta;

	vec3_t weights = { alpha, beta, gamma };
	return weights;
}

void DrawTexel(int x, int y, uint32_t* texture, vec4_t point_a, vec4_t point_b, vec4_t point_c, float u0, float v0, float u1, float v1, float u2, float v2)
{
	vec2_t p = { x, y };
	vec2_t a = vec2_from_vec4(point_a);
	vec2_t b = vec2_from_vec4(point_b);
	vec2_t c = vec2_from_vec4(point_c);

	vec3_t weights = BarycentricWeights(a, b, c, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	const float EPSILON = 0.00001;
	if (alpha < -EPSILON || beta < -EPSILON || gamma < -EPSILON) {
		return;
	}

	// Variables to store the interpolated values of U, V, and also 1/w for the current pixel
	float interpolated_u;
	float interpolated_v;
	float interpolated_reciprocal_w;

	// Perform the interpolation of all U/w and V/w values using barycentric weights and a factor of 1/w
	interpolated_u = (u0 / point_a.w) * alpha + (u1 / point_b.w) * beta + (u2 / point_c.w) * gamma;
	interpolated_v = (v0 / point_a.w) * alpha + (v1 / point_b.w) * beta + (v2 / point_c.w) * gamma;

	// Also interpolate the value of 1/w for the current pixel
	interpolated_reciprocal_w = (1 / point_a.w) * alpha + (1 / point_b.w) * beta + (1 / point_c.w) * gamma;

	// Now we can divide back both interpolated values by 1/w
	interpolated_u /= interpolated_reciprocal_w;
	interpolated_v /= interpolated_reciprocal_w;

	// Map the UV coordinate to the full texture width and height
	int tex_x = abs((int)(interpolated_u * texture_width)) % texture_width;
	int tex_y = abs((int)(interpolated_v * texture_height)) % texture_height;

	interpolated_reciprocal_w = 1 - interpolated_reciprocal_w;
	
	if(interpolated_reciprocal_w < z_buffer[(WINDOW_WIDTH * y) + x])
	{
		DrawPixel(x, y, texture[(texture_width * tex_y) + tex_x]);
		z_buffer[(WINDOW_WIDTH * y) + x] = interpolated_reciprocal_w;
	}
}

void DrawTexturedTriangle(int x0, int y0, float z0, float w0, float u0, float v0,
	int x1, int y1, float z1, float w1, float u1, float v1,
	int x2, int y2, float z2, float w2, float u2, float v2, uint32_t* texture)
{
	// We need to sort the vertices by y-coordinate ascending (y0 < y1 < y2)
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}
	if (y1 > y2) {
		int_swap(&y1, &y2);
		int_swap(&x1, &x2);
		float_swap(&z1, &z2);
		float_swap(&w1, &w2);
		float_swap(&u1, &u2);
		float_swap(&v1, &v2);
	}
	if (y0 > y1) {
		int_swap(&y0, &y1);
		int_swap(&x0, &x1);
		float_swap(&z0, &z1);
		float_swap(&w0, &w1);
		float_swap(&u0, &u1);
		float_swap(&v0, &v1);
	}

	// Flip the v component ( V grows downwards)
	v0 = 1 - v0;
	v1 = 1 - v1;
	v2 = 1 - v2;

	// Create vector points and texture coords after we sort the vertices
	vec4_t point_a = { x0, y0, z0, w0 };
	vec4_t point_b = { x1, y1, z1, w1 };
	vec4_t point_c = { x2, y2, z2, w2 };
	tex2_t a_uv = { u0, v0 };
	tex2_t b_uv = { u1, v1 };
	tex2_t c_uv = { u2, v2 };

	///////////////////////////////////////////////////////
	// Render the upper part of the triangle (flat-bottom)
	///////////////////////////////////////////////////////
	float inv_slope_1 = 0;
	float inv_slope_2 = 0;

	if (y1 - y0 != 0) inv_slope_1 = (float)(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0) {
		for (int y = y0; y <= y1; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x <= x_end; x++) {
				// Draw our pixel with the color that comes from the texture
				DrawTexel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
			}
		}
	}

	///////////////////////////////////////////////////////
	// Render the bottom part of the triangle (flat-top)
	///////////////////////////////////////////////////////
	inv_slope_1 = 0;
	inv_slope_2 = 0;

	if (y2 - y1 != 0) inv_slope_1 = (float)(x2 - x1) / abs(y2 - y1);
	if (y2 - y0 != 0) inv_slope_2 = (float)(x2 - x0) / abs(y2 - y0);

	if (y2 - y1 != 0) {
		for (int y = y1; y <= y2; y++) {
			int x_start = x1 + (y - y1) * inv_slope_1;
			int x_end = x0 + (y - y0) * inv_slope_2;

			if (x_end < x_start) {
				int_swap(&x_start, &x_end); // swap if x_start is to the right of x_end
			}

			for (int x = x_start; x <= x_end; x++) {
				// Draw our pixel with the color that comes from the texture
				DrawTexel(x, y, texture, point_a, point_b, point_c, u0, v0, u1, v1, u2, v2);
			}
		}
	}
}
