/*
	Mert Kilic
 */

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
vec3_t camera_pos = {0, 0, 0 };

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
	render_method = WIREFRAME;
	culling_mode = CULL_BACKFACE;
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);
	
	LoadCubeMeshData();
	//LoadMeshData("./assets/f22.obj");
}

// Render
void Render() {
	
	/*Let's draw a grid*/
	DrawGrid();

	int NumberOfTriangles = array_length(TrianglesToRender);

	/* Testing cube */
	for (int i = 0; i < NumberOfTriangles; i++) {
		triangle_t triangle_to_render = TrianglesToRender[i];

		if (render_method == RENDER_FILL_TRIANGLE || render_method == RENDER_FILL_TRIANGLE_WIREFRAME) {

			DrawFilledTriangle(triangle_to_render.points[0].x, triangle_to_render.points[0].y,
				triangle_to_render.points[1].x, triangle_to_render.points[1].y,
				triangle_to_render.points[2].x, triangle_to_render.points[2].y,
				triangle_to_render.color
			);
		}

		if (render_method == RENDER_FILL_TRIANGLE_WIREFRAME || render_method == WIREFRAME || render_method == WIREFRAME_VERTEX) {
			DrawTriangle(triangle_to_render.points[0].x, triangle_to_render.points[0].y,
				triangle_to_render.points[1].x, triangle_to_render.points[1].y,
				triangle_to_render.points[2].x, triangle_to_render.points[2].y,
				0xFFFFFFFF
			);
		}

		if (render_method == WIREFRAME_VERTEX) {
			DrawRect(triangle_to_render.points[0].x, triangle_to_render.points[0].y, 4, 4, 0xFFFFFFFF);
			DrawRect(triangle_to_render.points[1].x, triangle_to_render.points[1].y, 4, 4, 0xFFFFFFFF);
			DrawRect(triangle_to_render.points[2].x, triangle_to_render.points[2].y, 4, 4, 0xFFFFFFFF);
		}
		
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
		if (event.key.keysym.sym == SDLK_1) {
			render_method = WIREFRAME_VERTEX;
		}
		if (event.key.keysym.sym == SDLK_2) {
			render_method = WIREFRAME;
		}
		if (event.key.keysym.sym == SDLK_3) {
			render_method = RENDER_FILL_TRIANGLE;
		}
		if (event.key.keysym.sym == SDLK_4) {
			render_method = RENDER_FILL_TRIANGLE_WIREFRAME;
		}
		if (event.key.keysym.sym == SDLK_c) {
			culling_mode = CULL_BACKFACE;
		}
		if (event.key.keysym.sym == SDLK_d) {
			culling_mode = CULL_NONE;
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

		vec3_t transformed_vertices[3];
		for (int j = 0; j < 3; j++) {
			vec3_t transformed_vertex = face_vertices[j];

			/*Get the rotated point*/
			transformed_vertex = RotateX(transformed_vertex, mesh.rotation.x);
			transformed_vertex = RotateY(transformed_vertex, mesh.rotation.y);
			transformed_vertex = RotateZ(transformed_vertex, mesh.rotation.z);

			// get away from the camera
			transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}
		
		if(culling_mode == CULL_BACKFACE){
			// Get All 3 vertices of the current triangle face
			vec3_t vector_a = transformed_vertices[0]; // a
			vec3_t vector_b = transformed_vertices[1]; // b
			vec3_t vector_c = transformed_vertices[2]; // c

			// Find length
			vec3_t vector_ab = vec3_sub(vector_b, vector_a);
			vec3_t vector_ac = vec3_sub(vector_c, vector_a);
			vec3_normalize(&vector_ab);
			vec3_normalize(&vector_ac);

			// Calculate normal
			vec3_t normal = vec3_cross(vector_ab, vector_ac);
			vec3_normalize(&normal);

			// Calculate camera ray
			vec3_t camera_ray = vec3_sub(camera_pos, vector_a);

			// Calculate dot 
			float DotNormalCamera = vec3_dot(normal, camera_ray);

			// If it is less than zero don't add this triangle to triangles_to_render array
			if (DotNormalCamera < 0) {
				continue;
			}
		}

		
		vec2_t projected_points[3];
		for(int j = 0; j < 3; j++){
			projected_points[j] = project(transformed_vertices[j]);
			projected_points[j].x += WINDOW_WIDTH / 2;
			projected_points[j].y += (WINDOW_HEIGHT / 2);
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;

		triangle_t projected_triangle = {
			.points = {
				{projected_points[0].x, projected_points[0].y},
				{projected_points[1].x, projected_points[1].y},
				{projected_points[2].x, projected_points[2].y},
			},
			.color = current_face.color,
			.avg_depth = avg_depth
		};
		
		array_push(TrianglesToRender, projected_triangle);
	}

	// SORT BY AVG_DEPTH
	int length = array_length(TrianglesToRender) - 1;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length - i; j++) {
			if (TrianglesToRender[j].avg_depth < TrianglesToRender[j + 1].avg_depth) {
				triangle_t temp;
				temp = TrianglesToRender[j];
				TrianglesToRender[j] = TrianglesToRender[j + 1];
				TrianglesToRender[j + 1] = temp;
			}
		}
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