#include "vector.h"
#include <math.h>


/* VECTOR2 FUNCTIONS */
/* VECTOR MAGNITUDE - LENGTH */
float vec2_length(vec2_t v)
{
	return sqrt(v.x * v.x + v.y * v.y);
}

vec2_t vec2_add(vec2_t a, vec2_t b)
{
	vec2_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y
	};
	return result;
}

vec2_t vec2_sub(vec2_t a, vec2_t b)
{
	vec2_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y
	};
	return result;
}

vec2_t vec2_mul(vec2_t a, float value)
{
	vec2_t result = {
		.x = a.x * value,
		.y = a.y * value
	};
	return result;
}


vec2_t vec2_div(vec2_t a, float value)
{
	vec2_t result = {
		.x = a.x / value,
		.y = a.y / value
	};
	return result;
}


float vec2_dot(vec2_t a, vec2_t b)
{
	return (a.x * b.x) + (a.y * b.y);
}

void vec2_normalize(vec2_t* a) 
{
	float len = vec2_length(*a);
	a->x = a->x / len;
	a->y = a->y / len;
}

/* VECTOR3 FUNCTIONS */
float vec3_length(vec3_t v)
{
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3_t vec3_add(vec3_t a, vec3_t b) 
{
	vec3_t result = {
		.x = a.x + b.x,
		.y = a.y + b.y,
		.z = a.z + b.z
	};
	return result;
}

vec3_t vec3_sub(vec3_t a, vec3_t b)
{
	vec3_t result = {
		.x = a.x - b.x,
		.y = a.y - b.y,
		.z = a.z - b.z
	};
	return result;
}

vec3_t vec3_mul(vec3_t a, float value)
{
	vec3_t result = {
		.x = a.x * value,
		.y = a.y * value,
		.z = a.z * value
	};
	return result;
}

vec3_t vec3_div(vec3_t a, float value)
{
	vec3_t result = {
		.x = a.x / value,
		.y = a.y / value,
		.z = a.z / value
	};
	return result;
}

vec3_t vec3_cross(vec3_t a, vec3_t b)
{
	vec3_t result = {
		.x = a.y * b.z - a.z * b.y,
		.y = a.z * b.x - a.x * b.z,
		.z = a.x * b.y - a.y * b.x
	};
	return result;
}

float vec3_dot(vec3_t a, vec3_t b)
{
	return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

void vec3_normalize(vec3_t* a)
{
	float len = vec3_length(*a);
	a->x /= len;
	a->y /= len;
	a->z /= len;
}

vec3_t vec3_from_vec4(vec4_t v)
{
	vec3_t result = {
	v.x,
	v.y,
	v.z
	};
	return result;
}

vec4_t vec4_from_vec3(vec3_t v)
{
	vec4_t result = {
	v.x,
	v.y,
	v.z,
	1
	};
	return result;
}

vec2_t vec2_from_vec4(vec4_t v)
{
	vec2_t result = {
	v.x,
	v.y
	};
	return result;
}

/* Rotate vector around x axis */
vec3_t RotateX(vec3_t point, float angle) {
	vec3_t rotated_point;
	rotated_point.x = point.x;
	rotated_point.y = point.y * cos(angle) - point.z * sin(angle);
	rotated_point.z = point.y * sin(angle) + point.z * cos(angle);
	return rotated_point;
}
/* Rotate vector around y axis */
vec3_t RotateY(vec3_t point, float angle) {
	vec3_t rotated_point;
	rotated_point.x = point.x * cos(angle) - point.z * sin(angle);
	rotated_point.y = point.y;
	rotated_point.z = point.x * sin(angle) + point.z * cos(angle);
	return rotated_point;
}
/* Rotate vector around z axis */
vec3_t RotateZ(vec3_t point, float angle) {
	vec3_t rotated_point;
	rotated_point.x = point.x * cos(angle) - point.y * sin(angle);
	rotated_point.y = point.x * sin(angle) + point.y * cos(angle);
	rotated_point.z = point.z;
	return rotated_point;
}

