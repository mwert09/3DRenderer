#include "vector.h"
#include <math.h>

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

