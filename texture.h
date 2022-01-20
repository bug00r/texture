#if 0
/**
	This is a simple texture class. 
*/
#endif

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdlib.h>
#include <stddef.h>
#include <float.h>
#include "utils_math.h"
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

extern const size_t texture_size;

texture_t * texture_new(const unsigned int w, const unsigned int h);
texture_t * texture_copy(texture_t * texture);
void texture_free(texture_t * texture);

void filter_invert(texture_t * texture, const unsigned int ignore, const float ignore_too);
void filter_middle_arith(texture_t * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_box(texture_t * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_cross(texture_t * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_diag_cross(texture_t * texture, const unsigned int pxrange, const float factor);
void filter_middle_cubic_box(texture_t * texture, const unsigned int pxrange, const float factor);
void filter_gaussian(texture_t * texture, const unsigned int pxrange, const float deviation);


void noise_to_texture( const noise_t * src, texture_t * target);
void mandelbrot_to_texture( const mandelbrot_t * src, texture_t * target, 
							void (*mb_color_func)(const mandelbrot_t * mb, const mandelbrot_point_t * mbt, cRGB_t * col));
void julia_to_texture( const julia_t * src, texture_t * target, 
							void (*jul_color_func)(const julia_t * julia, const julia_point_t * jbt, cRGB_t * col));

void mandelbrot_color_normal_8Bit(const mandelbrot_t * mb, const mandelbrot_point_t * mbt, cRGB_t * col);
void mandelbrot_color_line_int_8Bit(const mandelbrot_t * mb, const mandelbrot_point_t * mbt, cRGB_t * col);
void mandelbrot_color_line_int_rgb(const mandelbrot_t * mb, const mandelbrot_point_t * mbt, cRGB_t * col);

void julia_color_normal_8Bit(const julia_t * julia, const julia_point_t * jbt, cRGB_t * col);
void julia_color_line_int_8Bit(const julia_t * julia, const julia_point_t * jbt, cRGB_t * col);
						   
void save_texture_ppm(const texture_t * texture, const char * filename);
void save_texture_normalized_ppm(const texture_t * texture, const char * filename);
#endif
