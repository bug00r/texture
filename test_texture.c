#if 0
/**
	This is a Test programm for the color lib
*/
#endif

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <time.h>
#include "texture.h"

typedef struct {
	const char *name;
	float (*middlefunc)(float x1, float x2, float x3, float x4);
} middle_func4_t;

typedef struct {
	const char *name;
	float (*middlefunc)(float x1, float x2);
} middle_func2_t;

typedef struct {
	const char *name;
	void (*filter)(texture_t * texture, unsigned int pxrange, float factor);
} texture_filter_t;

typedef struct {
	const char *name;
	void (*manipulation)(texture_t * texture);
} texture_manipulation_t;

typedef struct {
	const char *name;
	float complex c;
} julia_c_t;

typedef struct {
	const char *name;
	float complex (*func)(const float complex *cp, const float complex *c);
} julia_polyfunc_t;

char * create_string(const char * msg, ...)
{
	va_list vl;
	va_start(vl, msg);
	int buffsize = vsnprintf(NULL, 0, msg, vl);
	va_end(vl);
	buffsize += 1;
	char * buffer = malloc(buffsize);
	va_start(vl, msg);
	vsnprintf(buffer, buffsize, msg, vl);
	va_end( vl);
	return buffer;
}

void texture_manipulation_brigthness_contrast(texture_t * texture)
{
	array_iterator_t * it = array_iterator_new(texture->buffer);
	cRGB_t * curcolor;
	cRGB_t temp;
	
	float contrast_factor = crgb_contrast_factor_255(seedrndlh(75.f, 80.f));
	float brightness = seedrndlh(-35.f, -25.f);
	while(array_iterator_has_next(it))
	{
		curcolor = (cRGB_t *)array_iterator_next(it);
		crgb_brightness_255_dest(&temp, curcolor, brightness);
		crgb_contrast_255_dest(curcolor, &temp, contrast_factor);
	}
	array_iterator_free(it);
}

void test_noise()
{	 
	texture_manipulation_t tex_manip[] = {
		(texture_manipulation_t){"br_co",texture_manipulation_brigthness_contrast},
		(texture_manipulation_t){NULL,NULL}
	};
	
	texture_filter_t tex_filter[] = {
		(texture_filter_t){"filter_gauss",filter_gaussian},
		#ifdef output
		(texture_filter_t){"filter_middle_arith",filter_middle_arith},
		(texture_filter_t){"filter_middle_median_box",filter_middle_median_box},
		(texture_filter_t){"filter_middle_median_cross",filter_middle_median_cross},
		#endif
		(texture_filter_t){NULL,NULL}	
	};

	middle_func4_t funcs4[] = {
		(middle_func4_t){"middle_arithmetic",middle_arithmetic},
		#ifdef output
		// produces only black images not used anymore(middle_func4_t){"middle_harmonic",middle_harmonic},
		(middle_func4_t){"middle_quantil",middle_quantil},
		(middle_func4_t){"middle_median",middle_median},
		(middle_func4_t){"middle_quad",middle_quad},
		(middle_func4_t){"middle_cubic",middle_cubic},
		// produces only black images not used anymore(middle_func4_t){"middle_geometric",middle_geometric},
		(middle_func4_t){"middle_hoelder",middle_hoelder},
		#endif
		(middle_func4_t){NULL,NULL}	
	};

	middle_func2_t funcs2[] = {
		(middle_func2_t){"middle_arithmetic",middle_arithmetic2},
		#ifdef output
		(middle_func2_t){"middle_harmonic",middle_harmonic2},
		(middle_func2_t){"middle_quad",middle_quad2},
		(middle_func2_t){"middle_cubic",middle_cubic2},
		(middle_func2_t){"middle_geometric",middle_geometric2},
		(middle_func2_t){"middle_hoelder",middle_hoelder2},
		#endif
		(middle_func2_t){NULL,NULL}
	};
	
	int w = 513;
	int h = 513;
	texture_t * texture = texture_new(w,h);
	noise_t * noise = noise_new(w, h);
	
	unsigned int mediancross = 2;
	float arithfactor = 1.f;
	
	float maxreduction = 2.f;
	float reduction = 0.5f;
	
	midpoint_displacement_t md_param;
	
	middle_func4_t * curfunc4;
	for (unsigned int mf4 = 0; ; ++mf4){
		curfunc4 = &funcs4[mf4];

		if(!curfunc4->middlefunc) break;
		middle_func2_t * curfunc2;
		for (unsigned int mf2 = 0; ; ++mf2){
			curfunc2 = &funcs2[mf2];
			
			if(!curfunc2->middlefunc) break;
			noise->min = FLT_MAX;
			noise->max = FLT_MIN;
			md_param.noise = noise;
			md_param.length = w-1;
			md_param.startseed = 1.f;
			md_param.seed = maxreduction;
			md_param.reduction = reduction;
			md_param.middlefunc = curfunc4->middlefunc;
			md_param.middlefunc2 = curfunc2->middlefunc;
			md_param.seedreducefunc = seed_reduction_mul;
			create_midpoint_displacement(&md_param);

			texture_filter_t * curfilter;
			for (unsigned int filter = 0; ; ++filter){
				noise_to_texture( noise, texture);
				curfilter = &tex_filter[filter];

				if (curfilter->filter){
					curfilter->filter(texture, mediancross, arithfactor);
				}
				
				texture_manipulation_t * curmanipulation;
				
				for (unsigned int manipulation = 0; ; ++manipulation){
					texture_t * tex_copy = texture_copy(texture);
					curmanipulation = &tex_manip[manipulation];
					if (curmanipulation->manipulation){
						curmanipulation->manipulation(tex_copy);
					}
					#ifdef output
						char * filename = create_string("build/md_%s_%s_%s_%s.ppm", curfunc4->name, 
																			 curfunc2->name,
																			 curfilter->name,
																			 curmanipulation->name);
						save_texture_ppm(tex_copy, filename);
						free(filename);
					#endif
					texture_free(tex_copy);	
					if (!curmanipulation->manipulation){
						break;
					}
				}	
				if (!curfilter->filter){
					break;
				}
			}
			
		}
	}
	
	diamond_square_t ds_param;

	for (unsigned int mf4 = 0; ; ++mf4){
		curfunc4 = &funcs4[mf4];
		if(!curfunc4->middlefunc) break;
		noise->min = FLT_MAX;
		noise->max = FLT_MIN;
		ds_param.noise = noise;
		ds_param.length = w-1;
		ds_param.startseed = 1.f;
		ds_param.seed = maxreduction;
		ds_param.reduction = reduction;
		ds_param.middlefunc = curfunc4->middlefunc;
		ds_param.seedreducefunc = seed_reduction_mul;
		create_diamond_square(&ds_param);
		texture_filter_t * curfilter;
		for (unsigned int filter = 0; ; ++filter){
			noise_to_texture( noise, texture);
			curfilter = &tex_filter[filter];
			if (curfilter->filter){
				curfilter->filter(texture, mediancross, arithfactor);
			}
			
			texture_manipulation_t * curmanipulation;
			
			for (unsigned int manipulation = 0; ; ++manipulation){
				texture_t * tex_copy = texture_copy(texture);
				curmanipulation = &tex_manip[manipulation];
				if (curmanipulation->manipulation){
					curmanipulation->manipulation(tex_copy);
				}
				#ifdef output
					char * filename = create_string("build/ds_%s_%s_%s.ppm", curfunc4->name,
																		 curfilter->name,
																		 curmanipulation->name);
					save_texture_ppm(tex_copy, filename);
					free(filename);
				#endif
				texture_free(tex_copy);	
				if (!curmanipulation->manipulation){
					break;
				}
			}
			
			if (!curfilter->filter){
				break;
			}
		}
	}
	
	noise_free(noise);
	texture_free(texture);	
}

void test_fractals()
{

	julia_c_t julia_constants[] = {
		(julia_c_t){"000",0.7f + 0.f*I},
		#ifdef output
		(julia_c_t){"001",-0.8f + 0.2f*I},
		(julia_c_t){"002",-1.f + 0.f*I},
		(julia_c_t){"003",0.f + 1.f*I},
		(julia_c_t){"004",-.2f + 1.f*I},
		(julia_c_t){"005",0.f + 1.0f*I},
		(julia_c_t){"006",-.2f + .8f*I},
		(julia_c_t){"007",0.f + .8f*I},
		(julia_c_t){"008",.39f + .6f*I},
		(julia_c_t){"009",-0.75f+0.11f*I},
		(julia_c_t){"010",-0.1f+0.651f*I},
		(julia_c_t){"011",-0.74543f+0.11301f*I},
		#endif
		(julia_c_t){NULL,0.f + 0.f*I}	
	};
	
	julia_polyfunc_t julia_funcs[] = {
		(julia_polyfunc_t){"julia_pfunc_quad_plus_c",julia_pfunc_quad_plus_c},
		#ifdef output
		(julia_polyfunc_t){"julia_pfunc_third_plus_c",julia_pfunc_third_plus_c},  
		(julia_polyfunc_t){"julia_pfunc_fourth_plus_c",julia_pfunc_fourth_plus_c},
		(julia_polyfunc_t){"julia_pfunc_ten_plus_c",julia_pfunc_ten_plus_c},  
		(julia_polyfunc_t){"julia_pfunc_quad_plus_c_2",julia_pfunc_quad_plus_c_2},
		(julia_polyfunc_t){"julia_pfunc_quad_plus_c_1_2",julia_pfunc_quad_plus_c_1_2},
		#endif
		(julia_polyfunc_t){NULL,NULL}	
	};

	int w = 512;
	int h = 512;
	
	texture_t * texture_fractals = texture_new(w,h);
	
	julia_t *julia = julia_new(w, h);
	julia->minreal = -2.1f;//-1.3f;
	julia->maxreal =  2.1f;//-1.f;
	julia->minimag = -2.1f;//-.3f;
	julia->maximag =  2.1f;//0.f;
	julia->cntiterations = 400;
	
	julia_c_t * curconstant;
	for (unsigned int constant = 0; ; ++constant){
		curconstant = &julia_constants[constant];
		if(!curconstant->name) break;
		
		julia_polyfunc_t * curfunc;
		for (unsigned int func = 0; ; ++func){
			curfunc = &julia_funcs[func];
			if(!curfunc->name) break;
			julia->polyfunc = curfunc->func;
			julia->c = curconstant->c;
			create_julia(julia);
			//julia_to_texture(julia, texture_fractals, julia_color_normal_8Bit);
			julia_to_texture(julia, texture_fractals, julia_color_line_int_8Bit);
			#ifdef output
				char * filename = create_string("build/julia_%s_%s.ppm", curconstant->name,
																		 curfunc->name);
				save_texture_ppm(texture_fractals, filename);
				free(filename);
			#endif
		}
		
	}
	
	julia_free(julia);
		
	mandelbrot_t *mb = mandelbrot_new(w, h);
	mb->minreal = -2.f;//-1.3f;
	mb->maxreal = 0.5f;//-1.f;
	mb->minimag = -1.f;//-.3f;
	mb->maximag = 1.f;//0.f;
	mb->cntiterations = 20;
	create_mandelbrot(mb);
	mandelbrot_to_texture(mb, texture_fractals, mandelbrot_color_normal_8Bit);
	#ifdef output
		save_texture_ppm(texture_fractals, "build/mandelbrot_to_texture.ppm");
	#endif
	mandelbrot_to_texture(mb, texture_fractals, mandelbrot_color_line_int_8Bit);
	#ifdef output
		save_texture_ppm(texture_fractals, "build/mandelbrot_to_texture_line_int_8bit.ppm");
	#endif
	mandelbrot_free(mb);
	texture_free(texture_fractals);
	
}

int 
main() 
{
	#ifdef debug
		printf("Start testing texture lib:\n");
	#endif
	srand(time(NULL));
	test_noise();
	test_fractals();
	
	#ifdef debug
		printf("End testing texture lib:\n");
	#endif
	return 0;
}