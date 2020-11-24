#include <stdio.h>
#include "texture.h"

int texture_width = 64;
int texture_height = 64;

uint32_t* mesh_texture = NULL;

const upng_t* png_texture = NULL;

void LoadPngTextureData(char* filePath)
{
	png_texture = upng_new_from_file(filePath);
	if(png_texture != NULL)
	{
		upng_decode(png_texture);
		if(upng_get_error(png_texture) == UPNG_EOK)
		{
			mesh_texture = (uint32_t*)upng_get_buffer(png_texture);
			texture_width = upng_get_width(png_texture);
			texture_height = upng_get_height(png_texture);
		}
	}
}

