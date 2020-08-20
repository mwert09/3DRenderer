#ifndef VECTOR_H
#define VECTOR_H


#include <stdio.h>
/* Type definition for 2d vector */
typedef struct {
	float x;
	float y;
}vec2_t;
/* Type definition for 3d vector */
typedef struct {
	float x;
	float y;
	float z;
}vec3_t;

/* VECTOR2 FUNCTIONS */
// Vector 2 length magnitude hipotenus
float vec2_length(vec2_t v);

/* Add 2 2d vectors */
vec2_t vec2_add(vec2_t a, vec2_t b);

/* Sub 2 2d vectors */
vec2_t vec2_sub(vec2_t a, vec2_t b);

/* Mul 2d vectors */
vec2_t vec2_mul(vec2_t a, float value);

/* Div 2d vectors */
vec2_t vec2_div(vec2_t a, float value);

/* Dot product */
float vec2_dot(vec2_t a, vec2_t b);

/* Vector normalization */
void vec2_normalize(vec2_t* a);

/* VECTOR3 FUNCTIONS */
// Vector 3 length magnitude hipotenus
float vec3_length(vec3_t v);

/* Add 2 3d vectors */
vec3_t vec3_add(vec3_t a, vec3_t b);

/* Sub 2 3d vectors */
vec3_t vec3_sub(vec3_t a, vec3_t b);

/* Mul 3d vectors */
vec3_t vec3_mul(vec3_t a, float value);

/* Div 3d vectors */
vec3_t vec3_div(vec3_t a, float value);

/* Function to find normal of a triangle face */
vec3_t vec3_cross(vec3_t a, vec3_t b);

/* Dot product */
float vec3_dot(vec3_t a, vec3_t b);

/* Vector normalization */
void vec3_normalize(vec3_t* a);


/* Rotate vector around x axis */
vec3_t RotateX(vec3_t point, float angle);
/* Rotate vector around y axis */
vec3_t RotateY(vec3_t point, float angle);
/* Rotate vector around z axis */
vec3_t RotateZ(vec3_t point, float angle);

#endif