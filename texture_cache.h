#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include "dl_list.h"
#include "texture.h"

typedef struct {
    DlList *textures;
} TextureCache;

TextureCache* texture_cache_new();
void texture_cache_free(TextureCache **cache);

/* registers a texture and returns stored ID. The cache took ownership of texture.
   If cache will be free'd the memory for each cach in it will be free'd too */
int texture_cache_register(TextureCache *cache, Texture *texture);
Texture* texture_cache_get(TextureCache *cache, unsigned int id);

#endif