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
	Array * buffer;
} Texture;

extern const size_t texture_size;

Texture * texture_new(const unsigned int w, const unsigned int h);
Texture * texture_copy(Texture * texture);
void texture_free(Texture * texture);
void texture_clear(Texture * texture, ColorRGB *clearcolor);

void filter_invert(Texture * texture);
void filter_middle_arith(Texture * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_box(Texture * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_cross(Texture * texture, const unsigned int pxrange, const float factor);
void filter_middle_median_diag_cross(Texture * texture, const unsigned int pxrange, const float factor);
void filter_middle_cubic_box(Texture * texture, const unsigned int pxrange, const float factor);
void filter_gaussian(Texture * texture, const unsigned int pxrange, const float deviation);


void Noiseo_texture( const Noise * src, Texture * target);
void Mandelbroto_texture( const Mandelbrot * src, Texture * target, 
							void (*mb_color_func)(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col));
void Juliao_texture( const Julia * src, Texture * target, 
							void (*jul_color_func)(const Julia * julia, const JuliaPoint * jbt, ColorRGB * col));

void mandelbrot_color_normal_8Bit(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col);
void mandelbrot_color_line_int_8Bit(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col);
void mandelbrot_color_line_int_rgb(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col);

void julia_color_normal_8Bit(const Julia * julia, const JuliaPoint * jbt, ColorRGB * col);
void julia_color_line_int_8Bit(const Julia * julia, const JuliaPoint * jbt, ColorRGB * col);
						   
void save_texture_ppm(const Texture * texture, const char * filename);
void save_texture_normalized_ppm(const Texture * texture, const char * filename);
#endif
