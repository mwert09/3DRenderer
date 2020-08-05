#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vector.h"

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


#endif