#include "texture_cache.h"

static void __tex_cache_texture_free_wrapper(void **data, void *eachdata)
{
	(void)(eachdata);

	Texture *texture = (Texture *)*data;
	texture_free(texture);
}

TextureCache* texture_cache_new()
{
    TextureCache* newcache = malloc(sizeof(TextureCache));
    newcache->textures = dl_list_new();
    return newcache;
}

void texture_cache_free(TextureCache **cache)
{
    if ( cache != NULL && *cache != NULL )
    {
        TextureCache *to_delete = *cache;
        if ( to_delete->textures != NULL )
		{
			dl_list_each_data(to_delete->textures, NULL, __tex_cache_texture_free_wrapper);
		}

        dl_list_free(&to_delete->textures);
        free(to_delete);
    } 
}

int texture_cache_register(TextureCache *cache, Texture *texture)
{
    dl_list_append(cache->textures, texture);
    return cache->textures->cnt - 1;
}

Texture* texture_cache_get(TextureCache *cache, unsigned int id)
{
    if ( cache != NULL)
    {
        return dl_list_get(cache->textures, id);
    }

    return NULL;   
}
