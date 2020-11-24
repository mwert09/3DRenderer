#ifndef TEXTURE_H
#define TEXTURE_H
#include <stdint.h>

#include "upng.h"

typedef struct
{
	float u, v;
}tex2_t;

extern int texture_width;
extern int texture_height;
extern const upng_t* png_texture;
extern uint32_t* mesh_texture;

void LoadPngTextureData(char* filePath);
#endif