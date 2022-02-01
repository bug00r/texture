#if 0
/**
	This is a simple texture class. 
*/
#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <float.h>
#include "utilsmath.h"
#include "array.h"
#include "color.h"
#include "crgb_array.h"
#include "noise.h"
#include "fractals.h"

#ifndef M_PI
	#define M_PI 3.14159265358979323846264338327
#endif

typedef struct {
	unsigned int width;
	unsigned int height;
	array_t * buffer;
} texture_t;

texture_t * texture_new(unsigned int w, unsigned int h);
texture_t * texture_copy(texture_t * texture);
void texture_free(texture_t * texture);

void filter_middle_arith(texture_t * texture, unsigned int pxrange, float factor);
void filter_middle_median_box(texture_t * texture, unsigned int pxrange, float factor);
void filter_middle_median_cross(texture_t * texture, unsigned int pxrange, float factor);
void filter_gaussian(texture_t * texture, unsigned int pxrange, float deviation);

void noise_to_texture( noise_t * src, texture_t * target);
void mandelbrot_to_texture( mandelbrot_t * src, texture_t * target, void (*mb_color_func)(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t */ color_t * col));
void julia_to_texture( julia_t * src, texture_t * target, void (*jul_color_func)(julia_t * julia, julia_point_t * jbt, /**cRGB_t */ color_t * col));

void mandelbrot_color_normal_8Bit(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t */ color_t * col);
void mandelbrot_color_line_int_8Bit(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t */ color_t * col);
void mandelbrot_color_line_int_rgb(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t */ color_t * col);

void julia_color_normal_8Bit(julia_t * julia, julia_point_t * jbt, /**cRGB_t */ color_t * col);
void julia_color_line_int_8Bit(julia_t * julia, julia_point_t * jbt, /**cRGB_t */ color_t * col);
						   
void save_texture_ppm(texture_t * texture, const char * filename);

#endif
