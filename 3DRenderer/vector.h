#ifndef VECTOR_H
#define VECTOR_H
#endif

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

/* Rotate vector around x axis */
vec3_t RotateX(vec3_t point, float angle);
/* Rotate vector around y axis */
vec3_t RotateY(vec3_t point, float angle);
/* Rotate vector around z axis */
vec3_t RotateZ(vec3_t point, float angle);