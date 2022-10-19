//
// Created by szymm on 14.10.2022.
//

#ifndef JNIBITMAPS_RENDER_H
#define JNIBITMAPS_RENDER_H

#include "bitmap_manager.h"

typedef struct
{
    float x, y, z;
} Vector3;

uint32_t
render_texel64(Bitmap *tex, uint32_t x, uint32_t y);

uint32_t
render_texel(Bitmap *tex, uint32_t x, uint32_t y);

void
render_floor(Bitmap *view_bitmap, Bitmap *tex, Vector3 *pos, float angle, float fov);

void
render_sky(Bitmap *view_bitmap, Bitmap *tex, float angle, float fov);

#endif //JNIBITMAPS_RENDER_H
