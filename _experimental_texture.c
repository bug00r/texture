#include "texture.h"

texture_t * 
texture_new(unsigned int w, unsigned int h)
{
	texture_t * newtexture = malloc(sizeof(texture_t));
	newtexture->width=w;
	newtexture->height=h;
	//newtexture->buffer = crgb_array2D_new(w,h);
	//cRGB_t black = {0.f, 0.f, 0.f};
	//crgb_array_init(newtexture->buffer, &black);
	newtexture->buffer = color_array2D_new(w,h);
	color_t black = COL_CREATE_RGBA(0,0,0,0);
	color_array_init(newtexture->buffer, &black);
	
	return newtexture;
}

texture_t * texture_copy(texture_t * texture)
{
	texture_t * newtexture = malloc(sizeof(texture_t));
	newtexture->width=texture->width;
	newtexture->height=texture->height;
	newtexture->buffer = array_copy_deep(texture->buffer);
	return newtexture;
}

void texture_free(texture_t * texture)
{
	array_free(texture->buffer);
}

void 
noise_to_texture( noise_t * src, texture_t * target)
{
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) 
	{
		//cRGB_t * curcolor;
		//float colval;
		color_t * curcolor;
		int colval;
		float * noise_array = (float*)src->map->entries;
		array_iterator_t * it = array_iterator_new(target->buffer);
		while(array_iterator_has_next(it))
		{
			//curcolor = (cRGB_t *)array_iterator_next(it);
			curcolor = (color_t *)array_iterator_next(it);
			float noise_val = noise_array[it->index];
			colval = (int)interpolate_lin(noise_val, src->min, 0.f, src->max, 255.f);
			*curcolor = COL_CREATE_RGBA(colval,colval,colval,0);
		}
		array_iterator_free(it);
	}
}

void 
mandelbrot_color_normal_8Bit(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t*/ color_t * col)
{
	/**
		float colfactor = 1.f/curresult.abs;
		float its = ((float)curresult.iterations)/src->cntiterations;
		float re = crealf(curresult.cpoint)/curresult.iterations;
		float imag = cimagf(curresult.cpoint)/curresult.iterations;
		float diff = cimagf(curresult.cpoint)/crealf(curresult.cpoint);
	*/
	if(!mbt->isin)
	{
		float colfactor = 1.f/mbt->abs;
		*col = COL_CREATE_RGBA(((int)(colfactor * mb->minreal)),((int)(colfactor * 255)),0,0);
		//col->r = colfactor * mb->minreal;
		//col->g = colfactor * 255.f;
		//col->b = 0.f;//colval;
	} else
	{
		*col = COL_CREATE_RGBA(0,0,0,0);
		//col->r = 0.f;
		//col->g = 0.f;
		//col->b = 0.f;
	}
}

/**
mandelbrot_t *mb = mandelbrot_new(w, h);
	mb->minreal = -2.f;//-1.3f;
	mb->maxreal = 0.5f;//-1.f;
	mb->minimag = -1.f;//-.3f;
	mb->maximag = 1.f;//0.f;
	mb->cntiterations = 20;
*/
void 
mandelbrot_color_line_int_8Bit(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t*/ color_t * col)
{
	if(!mbt->isin)
	{	
		COL_SET_R(*col,((int)interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f)));
		COL_SET_G(*col,0);
		COL_SET_B(*col,((int)(interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 255.f))));
		//col->r = interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f); //real
		//col->g = 0.f;//interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 255.f); //imag
		//col->b = interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 255.f); //iterations	
	} else
	{
		COL_SET_R(*col,((int)interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f)));
		COL_SET_G(*col,((int)interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 255.f)));
		COL_SET_B(*col,0);
		//col->r = interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f); //real
		//col->g = interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 255.f); //imag
		//col->b = 0.f;//interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 255.f); //iterations	
	}
}

void 
mandelbrot_color_line_int_rgb(mandelbrot_t * mb, mandelbrot_point_t * mbt, /** cRGB_t */ color_t * col)
{
	if(!mbt->isin)
	{	
		COL_SET_R(*col,((int)interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f)));
		COL_SET_G(*col,0);
		COL_SET_B(*col,((int)interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 1.f)));
		//col->r = interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f); //real
		//col->g = 0.f;//interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 1.f); //imag
		//col->b = interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 1.f); //iterations	
	} else
	{
		COL_SET_R(*col,((int)interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f)));
		COL_SET_G(*col,((int)interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 1.f)));
		COL_SET_B(*col,0);
		//col->r = interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f); //real
		//col->g = interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 1.f); //imag
		//col->b = 0.f;//interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 1.f); //iterations	
	}
}

void 
mandelbrot_to_texture( mandelbrot_t * src, texture_t * target, void (*mb_color_func)(mandelbrot_t * mb, mandelbrot_point_t * mbt, /**cRGB_t*/ color_t * col))
{
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) 
	{
		array_iterator_t * it = array_iterator_new(target->buffer);
		mandelbrot_point_t * mb_array = (mandelbrot_point_t *)src->map->entries;
		while(array_iterator_has_next(it))
		{
			(*mb_color_func)(src, &mb_array[it->index],(/**cRGB_t*/color_t *)array_iterator_next(it));
		}
		array_iterator_free(it);
	}
}

void 
julia_color_normal_8Bit(julia_t * src, julia_point_t * jbt, /**cRGB_t*/ color_t * col)
{
	float colfactor = ((float)jbt->iterations)/(src->cntiterations*0.2f);
	#if 0
		//colval = ( noise_val > 1.f ? 1.f : (noise_val < -1.f ? -1.f : noise_val) );
		//colval = interpolate_lin(colval, -1.f, 0.f, 1.f, 255.f);
	#endif
	if (jbt->isin)
	{
		COL_SET_R(*col,(int)(colfactor * 255));
		COL_SET_G(*col,0);
		COL_SET_B(*col,0);
		//col->r = colfactor * 255;
		//col->g = 0.f;//colval;
		//col->b = 0.f;//colval;
	} else
	{
		COL_SET_R(*col,0);
		COL_SET_G(*col,0);
		COL_SET_B(*col,0);
		//col->r = 0.f;
		//col->g = 0.f;
		//col->b = 0.f;
	}
}

void 
julia_color_line_int_8Bit(julia_t * julia, julia_point_t * jbt, /**cRGB_t*/ color_t * col)
{
	if(jbt->isin)
	{	
		float colfactor_real = interpolate_lin(crealf(jbt->spoint), julia->minreal, 0.f, julia->maxreal, 255.f) / 255.f;
		float colfactor_imag = interpolate_lin(cimagf(jbt->spoint), julia->minimag, 0.f, julia->maximag, 255.f) / 255.f;
		COL_SET_R(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.12f, 255.f) * colfactor_real))); //iterations
		COL_SET_G(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.12f, 255.f) * ((colfactor_real + colfactor_imag)/8.f)))); //iterations
		COL_SET_B(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.12f, 255.f) * colfactor_imag))); //iterations
		//col->g = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * colfactor_real; //iterations
		//col->r = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * ((colfactor_real + colfactor_imag)/8.f); //iterations
		//col->b = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * colfactor_imag; //iterations
		
	} else
	{
		COL_SET_R(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.4f, 255.f)))); //iterations
		COL_SET_G(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.3f, 255.f)))); //iterations
		COL_SET_B(*col,((int)(interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.2f, 255.f)))); //iterations	
		//col->r = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.3f, 255.f); //iterations
		//col->g = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.2f, 255.f); //iterations
		//col->b = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.1f, 255.f); //iterations	
	}
}

void 
julia_to_texture( julia_t * src, texture_t * target,  void (*jul_color_func)(julia_t * julia, julia_point_t * jbt, /**cRGB_t*/ color_t * col))
{
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) 
	{
		array_iterator_t * it = array_iterator_new(target->buffer);
		julia_point_t * j_array = (julia_point_t *)src->map->entries;
		while(array_iterator_has_next(it))
		{
			(*jul_color_func)(src, &j_array[it->index], (/**cRGB_t*/color_t *)array_iterator_next(it));
		}
		array_iterator_free(it);
	}
}

void 
filter_middle_median_box(texture_t * texture, unsigned int pxrange, float factor)
{
	unsigned int maxlength = pxrange*2+1;
	maxlength *= maxlength;
	//float * values = malloc(maxlength * sizeof(float));
	int * values = malloc(maxlength * sizeof(int));
	
	/**cRGB_t*/color_t * cref;
	array_error_t array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h)
	{
	  for (unsigned int w = 0; w < texture->width; ++w)
	  {
		unsigned int cntvalue = 0;
		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry )
		{
			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx )
			{
				//array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				array_res = color_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK)
				{
					values[cntvalue++] = COL_GET_R(*cref);
				}
			}
		}
		
		qsort(values, cntvalue, sizeof(int), compare_floats);
		
		int newval = values[cntvalue >> 1];
		
		newval *= factor;
		
		array_res = color_array2D_get_ref(texture->buffer, w, h, &cref);
		COL_SET_R(*cref,newval);
		COL_SET_G(*cref,newval);
		COL_SET_B(*cref,newval);
	  }
	}
	free(values);
}

void 
filter_middle_median_cross(texture_t * texture, unsigned int pxrange, float factor)
{
	unsigned int cntValues = (4*pxrange)+1;
	//float * values = malloc(cntValues*sizeof(float));
	int * values = malloc(cntValues*sizeof(float));
	unsigned int maxidx = texture->width-1;
	
	/**cRGB_t*/ color_t * cref;
	
	for (unsigned int h = 0; h < texture->height; ++h)
	{
	  for (unsigned int w = 0; w < texture->width; ++w)
	  {
		unsigned int curValues = 0;
		int x = w;
		int y = h;
		
		color_array2D_get_ref(texture->buffer, x, y, &cref);
		values[curValues] = COL_GET_R(*cref);
		
		for(int delta = -pxrange; delta <= (int)pxrange ; ++delta)
		{
			if ( delta == 0 ) 
			{
				continue;
			}
			++curValues;
			x = fmaxf(fminf(w+delta, maxidx), 0);
			color_array2D_get_ref(texture->buffer, x, y, &cref);
			values[curValues] = COL_GET_R(*cref);
		}
		x = w;
		for(int delta = -pxrange; delta <= (int)pxrange ; ++delta)
		{
			if ( delta == 0 ) 
			{
				continue;
			}
			++curValues;
			y = fmaxf(fminf(h+delta, maxidx), 0);
			color_array2D_get_ref(texture->buffer, x, y, &cref);
			values[curValues] = COL_GET_R(*cref);
		}
				
		qsort(values, cntValues, sizeof(int), compare_floats);
	
		int newval = values[cntValues >> 1];
		newval *= factor;
		color_array2D_get_ref(texture->buffer, w, h, &cref);
		COL_SET_R(*cref,newval);
		COL_SET_G(*cref,newval);
		COL_SET_B(*cref,newval);
	  }
	}
	free(values);
}

void 
filter_middle_arith(texture_t * texture, unsigned int pxrange, float factor)
{
	/**cRGB_t*/color_t * cref;
	array_error_t array_res;
	for (unsigned int h = 0; h < texture->height; ++h)
	{
	  for (unsigned int w = 0; w < texture->width; ++w)
	  {
		int newval = 0.f;
		unsigned int cntvalue = 0;
		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry )
		{
			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx )
			{
				array_res = color_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK)
				{
					newval += COL_GET_R(*cref);
					cntvalue++;
				}
			}
		}
		
		newval /= cntvalue;
		newval *= factor;
		color_array2D_get_ref(texture->buffer, w, h, &cref);
		COL_SET_R(*cref,newval);
		COL_SET_G(*cref,newval);
		COL_SET_B(*cref,newval);
	  }
	}
}

void filter_gaussian(texture_t * texture, unsigned int pxrange, float deviation)
{

	//https://homepages.inf.ed.ac.uk/rbf/HIPR2/gsmooth.htm
	int kernelside = 1+(2*pxrange);
	float * gausskernel = malloc(kernelside*kernelside * sizeof(float));
	
	float useddeviavtion = 2*deviation*deviation;
	float base=1.f/(M_PI*useddeviavtion);
	for ( int pry = -pxrange, ky = 0; pry <= (int)pxrange; ++pry, ++ky )
	{
		int y_2 = pry*pry;
		for ( int prx = -pxrange, kx = 0; prx <= (int)pxrange; ++prx, ++kx )
		{
			int x_2 = prx*prx;
			gausskernel[ky * kernelside + kx] = base * expf(-((y_2+x_2)/useddeviavtion));
		}
	}

	color_t * cref;
	array_error_t array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h)
	{
	  for (unsigned int w = 0; w < texture->width; ++w)
	  {
		int newval = 0;
		for ( int pry = -pxrange, ky = 0; pry <= (int)pxrange; ++pry, ++ky )
		{
			for ( int prx = -pxrange, kx = 0; prx <= (int)pxrange; ++prx, ++kx )
			{
				array_res = color_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK)
				{
					float factor = gausskernel[ky * kernelside + kx];
					
					newval += (COL_GET_R(*cref) * factor);
				}
			}
		}
		
		color_array2D_get_ref(texture->buffer, w, h, &cref);
		COL_SET_R(*cref,newval);
		COL_SET_G(*cref,newval);
		COL_SET_B(*cref,newval);
	  }
	}
}

void 
save_texture_ppm(texture_t * texture, const char * filename)
{
	array_iterator_t * it = array_iterator_new(texture->buffer);
	color_t * curcolor;
	FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", texture->width, texture->height);
	while(array_iterator_has_next(it))
	{
		curcolor = (color_t *)array_iterator_next(it);
		static unsigned char color[3];
		color[0] = (unsigned char)COL_GET_R(*curcolor);  
		color[1] = (unsigned char)COL_GET_G(*curcolor);  
		color[2] = (unsigned char)COL_GET_B(*curcolor);  
		(void) fwrite(color, 1, 3, fp);
	}
	(void) fclose(fp);
	array_iterator_free(it);
}
