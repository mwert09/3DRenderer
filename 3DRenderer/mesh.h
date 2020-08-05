#ifndef MESH_H
#define MESH_H

#include "vector.h"
#include "triangle.h"
#include "array.h"

#define N_CUBE_VERTICES 8
#define N_CUBE_FACES (6 * 2) // 6 cube faces, 2 triangles per face

/* Type definition for mesh */
typedef struct {
	vec3_t* vertices;
	face_t* faces;
	vec3_t rotation;
}mesh_t;

extern mesh_t mesh;
extern vec3_t CubeVertices[N_CUBE_VERTICES];
extern face_t CubeFaces[N_CUBE_FACES];

// Temp function to load hardcoded data
void LoadCubeMeshData(void);







#endif