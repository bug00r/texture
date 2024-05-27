#include "texture.h"

const size_t texture_size = sizeof(Texture);

Texture * 
texture_new(const unsigned int w, const unsigned int h){
	Texture * newtexture = malloc(texture_size);
	newtexture->width=w;
	newtexture->height=h;
	newtexture->buffer = crgb_array2D_new(w,h);
	ColorRGB black = {0.f, 0.f, 0.f};
	crgb_array_init(newtexture->buffer, &black);
	
	return newtexture;
}

Texture * texture_copy(Texture * texture){
	Texture * newtexture = malloc(texture_size);
	newtexture->width=texture->width;
	newtexture->height=texture->height;
	newtexture->buffer = array_copy_deep(texture->buffer);
	return newtexture;
}

void texture_free(Texture * texture){
	if(texture != NULL) {
		array_free(texture->buffer);
		free(texture);
	}
}

void texture_clear(Texture * texture, ColorRGB *clearcolor)
{
	ColorRGB * cref;
	ArrayError array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){

		array_res = crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		if(array_res==ARRAY_ERR_OK){
			cref->r = clearcolor->r;
			cref->g = clearcolor->g;
			cref->b = clearcolor->b;
		}
	  }
	}
}

void 
Noiseo_texture( const Noise * src, Texture * target){
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) {
		ColorRGB * curcolor;
		float colval;
		const float * noise_array = (const float*)src->map->entries;
		ArrayIterator * it = array_iterator_new(target->buffer);
		
		while(array_iterator_has_next(it)){
			const float noise_val = noise_array[it->index];
			curcolor = (ColorRGB *)array_iterator_next(it);
			colval = interpolate_lin(noise_val, src->min, 0.f, src->max, 255.f);
			curcolor->r = colval;
			curcolor->g = colval;
			curcolor->b = colval;
		}
		array_iterator_free(it);
	}
}

void 
mandelbrot_color_normal_8Bit(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col){
	/**
		float colfactor = 1.f/curresult.abs;
		float its = ((float)curresult.iterations)/src->cntiterations;
		float re = crealf(curresult.cpoint)/curresult.iterations;
		float imag = cimagf(curresult.cpoint)/curresult.iterations;
		float diff = cimagf(curresult.cpoint)/crealf(curresult.cpoint);
	*/
	if(!mbt->isin){
		const float colfactor = 1.f/mbt->abs;
		col->r = colfactor * mb->minreal;
		col->g = colfactor * 255;
		col->b = 0.f;//colval;
	} else{
		col->r = 0.f;
		col->g = 0.f;
		col->b = 0.f;
	}
}

/**
Mandelbrot *mb = mandelbrot_new(w, h);
	mb->minreal = -2.f;//-1.3f;
	mb->maxreal = 0.5f;//-1.f;
	mb->minimag = -1.f;//-.3f;
	mb->maximag = 1.f;//0.f;
	mb->cntiterations = 20;
*/
void 
mandelbrot_color_line_int_8Bit(const Mandelbrot * mb,const MandelbrotPoint * mbt, ColorRGB * col){
	if(!mbt->isin){	
		col->r = interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f); //real
		col->g = 0.f;//interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 255.f); //imag
		col->b = interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 255.f); //iterations	
	} else{
		col->r = interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 255.f); //real
		col->g = interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 255.f); //imag
		col->b = 0.f;//interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 255.f); //iterations	
	}
}

void 
mandelbrot_color_line_int_rgb(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col){
	if(!mbt->isin){	
		col->r = interpolate_lin(cabsf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f); //real
		col->g = 0.f;//interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 1.f); //imag
		col->b = interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 1.f); //iterations	
	} else{
		col->r = interpolate_lin(crealf(mbt->cpoint), mb->minreal, 0.f, mb->maxreal, 1.f); //real
		col->g = interpolate_lin(cimagf(mbt->cpoint), mb->minimag, 0.f, mb->maximag, 1.f); //imag
		col->b = 0.f;//interpolate_lin((float)mbt->iterations, 0.f, 0.f, mb->cntiterations, 1.f); //iterations	
	}
}

void 
Mandelbroto_texture( const Mandelbrot * src, 
					   Texture * target, 
					   void (*mb_color_func)(const Mandelbrot * mb, const MandelbrotPoint * mbt, ColorRGB * col)){
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) 
	{
		ArrayIterator * it = array_iterator_new(target->buffer);
		MandelbrotPoint * mb_array = (MandelbrotPoint *)src->map->entries;
		while(array_iterator_has_next(it))
		{
			const MandelbrotPoint * mbt = &mb_array[it->index];
			ColorRGB * col = (ColorRGB *)array_iterator_next(it);
			(*mb_color_func)(src, mbt, col);
		}
		array_iterator_free(it);
	}
}

void 
julia_color_normal_8Bit(const Julia * src, const JuliaPoint * jbt, ColorRGB * col){
	const float colfactor = ((float)jbt->iterations)/(src->cntiterations*0.2f);
	#if 0
		//colval = ( noise_val > 1.f ? 1.f : (noise_val < -1.f ? -1.f : noise_val) );
		//colval = interpolate_lin(colval, -1.f, 0.f, 1.f, 255.f);
	#endif
	if (jbt->isin){
		col->r = colfactor * 255;
		col->g = 0.f;//colval;
		col->b = 0.f;//colval;
	} else{
		col->r = 0.f;
		col->g = 0.f;
		col->b = 0.f;
	}
}

void 
julia_color_line_int_8Bit(const Julia * julia, const JuliaPoint * jbt, ColorRGB * col){
	if(jbt->isin){	
		float colfactor_real = interpolate_lin(crealf(jbt->spoint), julia->minreal, 0.f, julia->maxreal, 255.f) / 255.f;
		float colfactor_imag = interpolate_lin(cimagf(jbt->spoint), julia->minimag, 0.f, julia->maximag, 255.f) / 255.f;
		col->g = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * colfactor_real; //iterations
		col->r = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * ((colfactor_real + colfactor_imag)/8.f); //iterations
		col->b = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.18f, 255.f) * colfactor_imag; //iterations
		
	} else{
		col->r = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.3f, 255.f); //iterations
		col->g = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.2f, 255.f); //iterations
		col->b = interpolate_lin((float)jbt->iterations, 0.f, 0.f, julia->cntiterations*0.1f, 255.f); //iterations	
	}
}

void 
Juliao_texture( const Julia * src, 
				  Texture * target,  
				  void (*jul_color_func)(const Julia * julia, const JuliaPoint * jbt, ColorRGB * col)){
	if ( src->map->config->size == target->buffer->config->size &&
		 src->map->config->cnt == target->buffer->config->cnt) {
		ArrayIterator * it = array_iterator_new(target->buffer);
		JuliaPoint * j_array = (JuliaPoint *)src->map->entries;
		while(array_iterator_has_next(it)){
			const JuliaPoint * jbt = &j_array[it->index];
			 ColorRGB * col = (ColorRGB *)array_iterator_next(it);
			(*jul_color_func)(src, jbt, col);
		}
		array_iterator_free(it);
	}
}

void filter_invert(Texture * texture) {

	ColorRGB * cref;
	ArrayError array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){

		array_res = crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		if(array_res==ARRAY_ERR_OK){
			cref->r = 255.0 - cref->r;
			cref->g = 255.0 - cref->g;
			cref->b = 255.0 - cref->b;
		}
	  }
	}
}

void 
filter_middle_median_box(Texture * texture, const unsigned int pxrange, const float factor){
	unsigned int maxlength = pxrange*2+1;
	maxlength *= maxlength;
	float * values_r = malloc(maxlength * sizeof(float));
	float * values_g = malloc(maxlength * sizeof(float));
	float * values_b = malloc(maxlength * sizeof(float));
	
	ColorRGB * cref;
	ArrayError array_res;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		unsigned int cntvalue = 0;
		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry ){
			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx ){
				array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK){
					cntvalue++;
					values_r[cntvalue] = cref->r;
					values_g[cntvalue] = cref->g;
					values_b[cntvalue] = cref->b;
				}
			}
		}
		
		qsort(values_r, cntvalue, sizeof(float), compare_floats);
		qsort(values_g, cntvalue, sizeof(float), compare_floats);
		qsort(values_b, cntvalue, sizeof(float), compare_floats);
		
		float newval_r = values_r[cntvalue >> 1];
		float newval_g = values_g[cntvalue >> 1];
		float newval_b = values_b[cntvalue >> 1];
		
		newval_r *= factor;
		newval_g *= factor;
		newval_b *= factor;
		
		array_res = crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval_r;
		cref->g = newval_g;
		cref->b = newval_b;
	  }
	}
	free(values_r);
	free(values_g);
	free(values_b);
}

//void 
//filter_middle_median_box(Texture * texture, const unsigned int pxrange, const float factor){
//	unsigned int maxlength = pxrange*2+1;
//	maxlength *= maxlength;
//	float * values = malloc(maxlength * sizeof(float));
//	
//	ColorRGB * cref;
//	ArrayError array_res;
//	
//	for (unsigned int h = 0; h < texture->height; ++h){
//	  for (unsigned int w = 0; w < texture->width; ++w){
//		unsigned int cntvalue = 0;
//		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry ){
//			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx ){
//				array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
//				if(array_res==ARRAY_ERR_OK){
//					values[cntvalue++] = cref->r;
//				}
//			}
//		}
//		
//		qsort(values, cntvalue, sizeof(float), compare_floats);
//		
//		float newval = values[cntvalue >> 1];
//		
//		newval *= factor;
//		
//		array_res = crgb_array2D_get_ref(texture->buffer, w, h, &cref);
//		cref->r = newval;
//		cref->g = newval;
//		cref->b = newval;
//	  }
//	}
//	free(values);
//}

void filter_middle_cubic_box(Texture * texture, const unsigned int pxrange, const float factor) {
	
	ColorRGB * cref;
	ArrayError array_res;
	ColorRGB newval;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		unsigned int cntvalue = 0;
		newval.r = 0.f;
		newval.g = 0.f;
		newval.b = 0.f;
		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry ){
			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx ){
				array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK){
					newval.r += (cref->r*cref->r*cref->r);
					newval.g += (cref->g*cref->g*cref->g);
					newval.b += (cref->b*cref->b*cref->b);
					++cntvalue;
				}
			}
		}
		
		newval.r = cbrtf(newval.r * (1.f/cntvalue));
		newval.g = cbrtf(newval.g * (1.f/cntvalue));
		newval.b = cbrtf(newval.b * (1.f/cntvalue));
		
		crgb_mul(&newval, factor);
		
		array_res = crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval.r;
		cref->g = newval.g;
		cref->b = newval.b;
	  }
	}
}

void 
filter_middle_median_cross(Texture * texture, const unsigned int pxrange, const float factor) {
	const unsigned int cntValues = (4*pxrange)+1;
	float * values = malloc(cntValues*sizeof(float));
	const unsigned int maxidx = texture->width-1;
	
	ColorRGB * cref;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		unsigned int curValues = 0;
		int x = w;
		int y = h;
		
		crgb_array2D_get_ref(texture->buffer, x, y, &cref);
		values[curValues] = cref->r;
		
		for(int delta = -pxrange; delta <= (int)pxrange ; ++delta){
			if ( delta == 0 ) {
				continue;
			}
			++curValues;
			x = fmaxf(fminf(w+delta, maxidx), 0);
			crgb_array2D_get_ref(texture->buffer, x, y, &cref);
			values[curValues] = cref->r;
		}
		x = w;
		for(int delta = -pxrange; delta <= (int)pxrange ; ++delta){
			if ( delta == 0 ) {
				continue;
			}
			++curValues;
			y = fmaxf(fminf(h+delta, maxidx), 0);
			crgb_array2D_get_ref(texture->buffer, x, y, &cref);
			values[curValues] = cref->r;
		}
				
		qsort(values, cntValues, sizeof(float), compare_floats);
	
		float newval = values[cntValues >> 1];
		newval *= factor;
		crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval;
		cref->g = newval;
		cref->b = newval;
	  }
	}
	free(values);
}

void filter_middle_median_diag_cross(Texture * texture, const unsigned int pxrange, const float factor) {
	const unsigned int cntValues = (4*pxrange)+1;
	float * values = malloc(cntValues*sizeof(float));
	const unsigned int maxidx = texture->width-1;
	
	ColorRGB * cref;
	
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		unsigned int curValues = 0;
		int x = w;
		int y = h;
		
		crgb_array2D_get_ref(texture->buffer, x, y, &cref);
		values[curValues] = cref->r;
		
		for(int delta = -pxrange; delta <= (int)pxrange ; ++delta){
			if ( delta == 0 ) {
				continue;
			}
			x = fmaxf(fminf(w+delta, maxidx), 0);
			crgb_array2D_get_ref(texture->buffer, x, x, &cref);
			values[++curValues] = cref->r;
			crgb_array2D_get_ref(texture->buffer, x, -x, &cref);
			values[++curValues] = cref->r;
		}
				
		qsort(values, cntValues, sizeof(float), compare_floats);
	
		float newval = values[cntValues >> 1];
		newval *= factor;
		crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval;
		cref->g = newval;
		cref->b = newval;
	  }
	}
	free(values);
}

void 
filter_middle_arith(Texture * texture, const unsigned int pxrange, const float factor){
	ColorRGB * cref;
	ArrayError array_res;
	ColorRGB newval;
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		newval.r = 0.f;
		newval.g = 0.f;
		newval.b = 0.f;
		unsigned int cntvalue = 0;
		for ( int pry = -pxrange; pry <= (int)pxrange; ++pry ){
			for ( int prx = -pxrange; prx <= (int)pxrange; ++prx ){
				array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK){
					crgb_crgb_add(&newval, cref);
					cntvalue++;
				}
			}
		}
		
		crgb_mul(&newval, (1.f/cntvalue));
		crgb_mul(&newval, factor);

		crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval.r;
		cref->g = newval.g;
		cref->b = newval.b;
	  }
	}
}

void filter_gaussian(Texture * texture, const unsigned int pxrange, const float deviation)
{

	//https://homepages.inf.ed.ac.uk/rbf/HIPR2/gsmooth.htm
	const int kernelside = 1+(2*pxrange);
	float * gausskernel = malloc(kernelside*kernelside * sizeof(float));
	
	float useddeviavtion = 2*deviation*deviation;
	float base=1.f/(M_PI*useddeviavtion);
	for ( int pry = -pxrange, ky = 0; pry <= (int)pxrange; ++pry, ++ky ){
		int y_2 = pry*pry;
		for ( int prx = -pxrange, kx = 0; prx <= (int)pxrange; ++prx, ++kx ){
			int x_2 = prx*prx;
			gausskernel[ky * kernelside + kx] = base * expf(-((y_2+x_2)/useddeviavtion));
		}
	}

	ColorRGB * cref;
	ArrayError array_res;
	ColorRGB newval;
	for (unsigned int h = 0; h < texture->height; ++h){
	  for (unsigned int w = 0; w < texture->width; ++w){
		newval.r = 0.f;
		newval.g = 0.f;
		newval.b = 0.f;
		for ( int pry = -pxrange, ky = 0; pry <= (int)pxrange; ++pry, ++ky ){
			for ( int prx = -pxrange, kx = 0; prx <= (int)pxrange; ++prx, ++kx ){
				array_res = crgb_array2D_get_ref(texture->buffer, (w+prx), (h+pry), &cref);
				if(array_res==ARRAY_ERR_OK){
					float factor = gausskernel[ky * kernelside + kx];
					
					newval.r += (cref->r * factor);
					newval.g += (cref->g * factor);
					newval.b += (cref->b * factor);
				}
			}
		}
		
		crgb_array2D_get_ref(texture->buffer, w, h, &cref);
		cref->r = newval.r;
		cref->g = newval.g;
		cref->b = newval.b;
	  }
	}
	free(gausskernel);
}

void 
save_texture_ppm(const Texture * texture, const char * filename){
	ArrayIterator * it = array_iterator_new(texture->buffer);
	ColorRGB * curcolor;
	FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", texture->width, texture->height);
	while(array_iterator_has_next(it)){
		curcolor = (ColorRGB *)array_iterator_next(it);
		static unsigned char color[3];
		color[0] = (unsigned char)curcolor->r ;  
		color[1] = (unsigned char)curcolor->g ;  
		color[2] = (unsigned char)curcolor->b ;  
		(void) fwrite(color, 1, 3, fp);
	}
	(void) fclose(fp);
	array_iterator_free(it);
}

void 
save_texture_normalized_ppm(const Texture * texture, const char * filename){
	ArrayIterator * it = array_iterator_new(texture->buffer);
	ColorRGB curcolor;
	FILE *fp = fopen(filename, "wb"); /* b - binary mode */
    (void) fprintf(fp, "P6\n%d %d\n255\n", texture->width, texture->height);
	for( unsigned int curH = 0; curH < texture->height; curH++ )
	{
		for( unsigned int curW = 0; curW < texture->width; curW++ )
		{
			crgb_array2D_get(texture->buffer, curW, curH, &curcolor);
			static unsigned char color[3];
			color[0] = (unsigned char)( curcolor.r * 255.f);  
			color[1] = (unsigned char)( curcolor.g * 255.f);  
			color[2] = (unsigned char)( curcolor.b * 255.f);  
			(void) fwrite(color, 1, 3, fp);
		}
	}

	(void) fclose(fp);
	array_iterator_free(it);
}

