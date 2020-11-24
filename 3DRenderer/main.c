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
#include "matrix.h"
#include "light.h"
#include "texture.h"

 /*#define N_POINTS (9*9*9)
  Cube vectors for testing
 vec3_t cube_points[N_POINTS];
 vec2_t projected_points[N_POINTS];*/

 // Triangles to render each frame 
triangle_t* TrianglesToRender = NULL;

mat4_t perspective_matrix;


/* Camera Position temp */
vec3_t camera_pos = { 0, 0, 0 };

/* Cube Rotation This will change later */
//vec3_t cube_rotation = { .x = 0.01, .y = 0.01, .z = 0.01 };

/* previous frame time */
float previous_frame_time = 0;

// is_running to check if SDL initialized failed or not
bool is_running = false;

// Runs only once 
void Setup() {
	render_method = RENDER_TEXTURED_TRIANGLE_WIREFRAME;
	culling_mode = CULL_BACKFACE;
	color_buffer = (uint32_t*)malloc(sizeof(uint32_t) * WINDOW_WIDTH * WINDOW_HEIGHT);
	color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, WINDOW_WIDTH, WINDOW_HEIGHT);

	float fov = M_PI / 3.0;
	float aspect_ratio = (float)WINDOW_HEIGHT / (float)WINDOW_WIDTH;
	float znear = 0.1;
	float zfar = 100.0;
	perspective_matrix = mat4_make_perspective(fov, aspect_ratio, znear, zfar);

	LoadCubeMeshData();
	//LoadMeshData("./assets/cube.obj");
	LoadPngTextureData("./assets/cube.png");

	
	
}

// Render
void Render() {
	SDL_RenderClear(renderer);
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

		if (render_method == RENDER_TEXTURED_TRIANGLE || render_method == RENDER_TEXTURED_TRIANGLE_WIREFRAME) {

			DrawTexturedTriangle(triangle_to_render.points[0].x, triangle_to_render.points[0].y, triangle_to_render.points[0].z, triangle_to_render.points[0].w, triangle_to_render.texcoords[0].u, triangle_to_render.texcoords[0].v, // vertex A
				triangle_to_render.points[1].x, triangle_to_render.points[1].y, triangle_to_render.points[1].z, triangle_to_render.points[1].w, triangle_to_render.texcoords[1].u, triangle_to_render.texcoords[1].v, // vertex B
				triangle_to_render.points[2].x, triangle_to_render.points[2].y, triangle_to_render.points[2].z, triangle_to_render.points[2].w, triangle_to_render.texcoords[2].u, triangle_to_render.texcoords[2].v, // vertex C
				mesh_texture);
		}

		if (render_method == RENDER_FILL_TRIANGLE_WIREFRAME || render_method == WIREFRAME || render_method == WIREFRAME_VERTEX || render_method == RENDER_TEXTURED_TRIANGLE_WIREFRAME) {
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
		if (event.key.keysym.sym == SDLK_5) {
			render_method = RENDER_TEXTURED_TRIANGLE;
		}
		if (event.key.keysym.sym == SDLK_6) {
			render_method = RENDER_TEXTURED_TRIANGLE_WIREFRAME;
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
	previous_frame_time = SDL_GetTicks();
	
	TrianglesToRender = NULL;
	

	mesh.rotation.x -= 0.000;
	mesh.rotation.y += 0.009;
	mesh.rotation.z += 0.000;
	mesh.translation.z = 5;
	
	mat4_t scale_matrix = mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
	mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
	mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
	mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
	mat4_t translation_matrix = mat4_make_translation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

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

		vec4_t transformed_vertices[3];
		for (int j = 0; j < 3; j++) {
			vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

			// Create a World Matrix combining scale, rotation, and translation matrices
			mat4_t world_matrix = mat4_identity();

			// Order matters: First scale, then rotate, then translate. [T]*[R]*[S]*v
			world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
			world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
			world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

			transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);

			// get away from the camera
			//transformed_vertex.z += 5;
			transformed_vertices[j] = transformed_vertex;
		}


		// Get All 3 vertices of the current triangle face
		vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]); // a
		vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]); // b
		vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]); // c


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

		if (culling_mode == CULL_BACKFACE) {
			// If it is less than zero don't add this triangle to triangles_to_render array
			if (DotNormalCamera < 0) {
				continue;
			}
		}


		vec4_t projected_points[3];
		for (int j = 0; j < 3; j++) {
			projected_points[j] = mat4_mul_vec4_project(perspective_matrix, transformed_vertices[j]);
			projected_points[j].y *= -1;
			projected_points[j].x *= (WINDOW_WIDTH / 2.0);
			projected_points[j].y *= (WINDOW_HEIGHT / 2.0);

			projected_points[j].x += (WINDOW_WIDTH / 2.0);
			projected_points[j].y += (WINDOW_HEIGHT / 2.0);
		}

		float avg_depth = (transformed_vertices[0].z + transformed_vertices[1].z + transformed_vertices[2].z) / 3;
		float light_intensity_factor = -vec3_dot(normal, light.direction);
		uint32_t triangle_color = light_apply_intensity(current_face.color, light_intensity_factor);

		triangle_t projected_triangle = {
			.points = {
				{ projected_points[0].x, projected_points[0].y, projected_points[0].z, projected_points[0].w },
				{ projected_points[1].x, projected_points[1].y, projected_points[1].z, projected_points[1].w },
				{ projected_points[2].x, projected_points[2].y, projected_points[2].z, projected_points[2].w },
			},
			.texcoords = {
				{ current_face.a_uv.u, current_face.a_uv.v },
				{ current_face.b_uv.u, current_face.b_uv.v },
				{ current_face.c_uv.u, current_face.c_uv.v }
			},
			.color = triangle_color,
			.avg_depth = avg_depth
		};

		array_push(TrianglesToRender, projected_triangle);
	}

	// Sort the triangles to render by their avg_depth
	int num_triangles = array_length(TrianglesToRender);
	for (int i = 0; i < num_triangles; i++) {
		for (int j = i; j < num_triangles; j++) {
			if (TrianglesToRender[i].avg_depth < TrianglesToRender[j].avg_depth) {
				// Swap the triangles positions in the array
				triangle_t temp = TrianglesToRender[i];
				TrianglesToRender[i] = TrianglesToRender[j];
				TrianglesToRender[j] = temp;
			}
		}
	}
}

// Free resouces when we are done
void FreeResources() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	free(color_buffer);
	upng_free(png_texture);
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