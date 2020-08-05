#include <stdio.h>
#include <SDL.h>
#include "Display.h"
#include "vector.h"
#include "mesh.h"
#include "triangle.h"
#include "array.h"

/*#define N_POINTS (9*9*9)
 Cube vectors for testing 
vec3_t cube_points[N_POINTS];
vec2_t projected_points[N_POINTS];*/

// Triangles to render each frame 
triangle_t *TrianglesToRender = NULL;

/* Simple field of view factor */
int fov_factor = 1280;

/* Camera Position temp */
vec3_t camera_pos = { .x = 0, .y = 0, .z = -5 };

/* Cube Rotation This will change later */
//vec3_t cube_rotation = { .x = 0.01, .y = 0.01, .z = 0.01 };

/* previous frame time */
float previous_frame_time = 0;

// is_running to check if SDL initialized failed or not
bool is_running = false;

/* Simple perspective projection */
vec2_t project(vec3_t point) {
	vec2_t projected_point = {
		.x = (point.x * fov_factor) / point.z,
		.y = (point.y * fov_factor) / point.z
	};
	return projected_point;
}

// Runs only once 
void Setup() {
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	/* Initialize cube points array for testing 
	int point_count = 0;
	for (float x = -1; x <= 1; x+=0.25) {
		for (float y = -1; y <= 1; y += 0.25) {
			for (float z = -1; z <= 1; z += 0.25) {
				vec3_t point = { .x = x, .y = y, .z = z };
				cube_points[point_count] = point;
				point_count++;
			}
		}
	}*/
	LoadCubeMeshData();
}

// Render
void Render() {
	
	/*Let's draw a grid*/
	DrawGrid();

	int NumberOfTriangles = array_length(TrianglesToRender);

	/* Testing cube */
	for (int i = 0; i < NumberOfTriangles; i++) {
		triangle_t triangle_to_render = TrianglesToRender[i];

		DrawRect(triangle_to_render.points[0].x, triangle_to_render.points[0].y, 4, 4, 0xFFFFFFFF);
		DrawRect(triangle_to_render.points[1].x, triangle_to_render.points[1].y, 4, 4, 0xFFFFFFFF);
		DrawRect(triangle_to_render.points[2].x, triangle_to_render.points[2].y, 4, 4, 0xFFFFFFFF);

		DrawTriangle(triangle_to_render.points[0].x, triangle_to_render.points[0].y,
			triangle_to_render.points[1].x, triangle_to_render.points[1].y,
			triangle_to_render.points[2].x, triangle_to_render.points[2].y,
			0xFFFFFFFF
		);
	}

	/*Now we have a color buffer
	  We copy our color buffer to our texture and render it
	*/
	array_free(TrianglesToRender);
	RenderColorBuffer();
	ClearColorBuffer(0xFF000000);
	SDL_RenderPresent(renderer);
}

// Process input
void Input() {
	// Event 
	SDL_Event event;
	SDL_PollEvent(&event);
	switch (event.type) {
	case SDL_QUIT:
		is_running = false;
		break;
	case SDL_KEYDOWN:
		if (event.key.keysym.sym == SDLK_ESCAPE) {
			is_running = false;
		}
		break;
	}
}
// Update location rotation scale
void Update() {

	int time_to_wait = TARGET_FRAME_TIME - (SDL_GetTicks() - previous_frame_time);
	if (time_to_wait > 0 && time_to_wait < TARGET_FRAME_TIME) {
		SDL_Delay(time_to_wait);
	}
	TrianglesToRender = NULL;
	previous_frame_time = SDL_GetTicks();

	/*Increase rotation every update for testing */
	mesh.rotation.x += 0.01;
	mesh.rotation.y += 0.01;
	mesh.rotation.z += 0.01;
	

	/* Initialize projected points(vectors) for testing */
	int NumberOfFaces = array_length(mesh.faces);
	for (int i = 0; i < NumberOfFaces; i++) {
		// get the current face
		face_t current_face = mesh.faces[i];
		// store vertices of the current face
		vec3_t face_vertices[3];
		face_vertices[0] = mesh.vertices[current_face.a - 1];
		face_vertices[1] = mesh.vertices[current_face.b - 1];
		face_vertices[2] = mesh.vertices[current_face.c - 1];

		triangle_t projected_triangle;

		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			/*Get the rotated point*/
			transformed_vertex = RotateX(transformed_vertex, mesh.rotation.x);
			transformed_vertex = RotateY(transformed_vertex, mesh.rotation.y);
			transformed_vertex = RotateZ(transformed_vertex, mesh.rotation.z);

			// get away from the camera
			transformed_vertex.z -= camera_pos.z;

			vec2_t projected_vertex = project(transformed_vertex);
			projected_vertex.x += (WINDOW_WIDTH / 2);
			projected_vertex.y += (WINDOW_HEIGHT / 2);
			projected_triangle.points[j] = projected_vertex;
		}
		array_push(TrianglesToRender, projected_triangle);
	}

}

// Free resouces when we are done
void FreeResources() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	free(color_buffer);
	SDL_DestroyTexture(color_buffer_texture);
	array_free(mesh.faces);
	array_free(mesh.vertices);
	SDL_Quit();
}

int main(int argc, char* argv[]) {
	

	is_running = InitializeSDL();

	Setup();

	// simple game loop
	while (is_running) {
		Render();
		Input();
		Update();
	}

	FreeResources();

	return 0;
}