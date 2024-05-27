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
	Array * buffer;
} Texture;

Texture * texture_new(unsigned int w, unsigned int h);
Texture * texture_copy(Texture * texture);
void texture_free(Texture * texture);

void filter_middle_arith(Texture * texture, unsigned int pxrange, float factor);
void filter_middle_median_box(Texture * texture, unsigned int pxrange, float factor);
void filter_middle_median_cross(Texture * texture, unsigned int pxrange, float factor);
void filter_gaussian(Texture * texture, unsigned int pxrange, float deviation);

void Noiseo_texture( Noise * src, Texture * target);
void Mandelbroto_texture( Mandelbrot * src, Texture * target, void (*mb_color_func)(Mandelbrot * mb, MandelbrotPoint * mbt, /**ColorRGB */ Color * col));
void Juliao_texture( Julia * src, Texture * target, void (*jul_color_func)(Julia * julia, JuliaPoint * jbt, /**ColorRGB */ Color * col));

void mandelbrot_color_normal_8Bit(Mandelbrot * mb, MandelbrotPoint * mbt, /**ColorRGB */ Color * col);
void mandelbrot_color_line_int_8Bit(Mandelbrot * mb, MandelbrotPoint * mbt, /**ColorRGB */ Color * col);
void mandelbrot_color_line_int_rgb(Mandelbrot * mb, MandelbrotPoint * mbt, /**ColorRGB */ Color * col);

void julia_color_normal_8Bit(Julia * julia, JuliaPoint * jbt, /**ColorRGB */ Color * col);
void julia_color_line_int_8Bit(Julia * julia, JuliaPoint * jbt, /**ColorRGB */ Color * col);
						   
void save_texture_ppm(Texture * texture, const char * filename);

#endif
