#include "mesh.h"


mesh_t mesh = {
    .vertices = NULL,
    .faces = NULL,
    .rotation = {0,0,0},
	.scale = {1, 1, 1},
	.translation = {0, 0, 0}
};

vec3_t CubeVertices[N_CUBE_VERTICES] = {
    {.x = -1, .y = -1, .z = -1 }, // 1
    {.x = -1, .y = 1, .z = -1 }, // 2
    {.x = 1, .y = 1, .z = -1 }, // 3
    {.x = 1, .y = -1, .z = -1 }, // 4
    {.x = 1, .y = 1, .z = 1 }, // 5
    {.x = 1, .y = -1, .z = 1 }, // 6
    {.x = -1, .y = 1, .z = 1 }, // 7
    {.x = -1, .y = -1, .z = 1 }  // 8
};

face_t CubeFaces[N_CUBE_FACES] = {
    // front
    {.a = 1, .b = 2, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 1, .b = 3, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // right
    {.a = 4, .b = 3, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 4, .b = 5, .c = 6, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // back
    {.a = 6, .b = 5, .c = 7, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 6, .b = 7, .c = 8, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // left
    {.a = 8, .b = 7, .c = 2, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 8, .b = 2, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // top
    {.a = 2, .b = 7, .c = 5, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 2, .b = 5, .c = 3, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF },
    // bottom
    {.a = 6, .b = 8, .c = 1, .a_uv = { 0, 0 }, .b_uv = { 0, 1 }, .c_uv = { 1, 1 }, .color = 0xFFFFFFFF },
    {.a = 6, .b = 1, .c = 4, .a_uv = { 0, 0 }, .b_uv = { 1, 1 }, .c_uv = { 1, 0 }, .color = 0xFFFFFFFF }
};

void LoadCubeMeshData(void) {
    /*Load Vertices*/
    for (int i = 0; i < N_CUBE_VERTICES; i++) {
        vec3_t cube_vertex = CubeVertices[i];
        array_push(mesh.vertices, cube_vertex);
    }
    /* Load Faces*/
    for (int i = 0; i < N_CUBE_FACES; i++) {
        face_t cube_face = CubeFaces[i];
        array_push(mesh.faces, cube_face);
    }
}

void LoadMeshData(char* filename) {
    // initialize a buffer to read from file
    char line[1024];
    char cwd[1024];
    tex2_t* tex_coords = NULL;
    
    printf("%s\n", filename);
    char* data_path = NULL;
    char* base_path = SDL_GetBasePath();
    if (base_path) {
        data_path = base_path;
    }
    else {
        data_path = SDL_strdup("./");
    }

    char *fullpath = strcat(data_path, filename);

    // open file
    FILE* file;
    file = fopen(fullpath, "r");

    if (file) {
        // read line fread(&line, sizeof(char), 31, file)
        while (fgets(line, 1024, file)) {
            // check if it is anything we want
            // if it is, then push it to the corresponding array
            if (strncmp(line, "v ", 2) == 0) {
                vec3_t CurrentVertex;
                sscanf(line, "v %f %f %f", &CurrentVertex.x, &CurrentVertex.y, &CurrentVertex.z);
                array_push(mesh.vertices, CurrentVertex);
            }

            if (strncmp(line, "vt ", 3) == 0)
            {
                tex2_t tex_coord;
                sscanf(line, "vt %f %f", &tex_coord.u, &tex_coord.v);
                array_push(tex_coords, tex_coord);
            }

            if (strncmp(line, "f ", 2) == 0) {
                int vertex_indices[3];
                int texture_indices[3];
                int normal_indices[3];
                sscanf(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &vertex_indices[0], &texture_indices[0], &normal_indices[0],
                    &vertex_indices[1], &texture_indices[1], &normal_indices[1],
                    &vertex_indices[2], &texture_indices[2], &normal_indices[2]
                );
                face_t CurrentFace = {
                    .a = vertex_indices[0],
                    .b = vertex_indices[1],
                    .c = vertex_indices[2],
                    .a_uv = tex_coords[texture_indices[0] - 1],
                	.b_uv = tex_coords[texture_indices[1] - 1],
                	.c_uv = tex_coords[texture_indices[2] - 1],
                	.color = 0xFFFFFFFF
                };
                array_push(mesh.faces, CurrentFace);
            }
        }
        array_free(tex_coords);
    }
    else {
        
        printf("Could not open %s app is running here: %s\n", filename, data_path);
        return;
    }
    
    fclose(file);
}