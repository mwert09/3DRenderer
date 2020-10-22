#include "light.h"

light_t light = { 0, 0, 1 };

uint32_t light_apply_intensity(uint32_t current_color, float intensity)
{
	if(intensity < 0)
	{
		intensity = 0;
	}
	if(intensity > 1)
	{
		intensity = 1;
	}
	uint32_t a = (current_color & 0xFF000000);
	uint32_t r = (current_color & 0x00FF0000) * intensity;
	uint32_t g = (current_color & 0x0000FF00) * intensity;
	uint32_t b = (current_color & 0x000000FF) * intensity;
	uint32_t new_color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);
	return new_color;
}