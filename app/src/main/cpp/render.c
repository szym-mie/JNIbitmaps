//
// Created by szymm on 14.10.2022.
//

#include "render.h"
#include "bitmap_manager.h"
#include <math.h>

#define PIXEL_FETCH_DIV 128

#define ANGLE_MAX 6.28318f

#define RENDER_TEXTURE_PTR(TEX, OFF) ((uint32_t *)TEX->pixels + OFF)
#define RENDER_TEXTURE_ROW_PTR(TEX, V) RENDER_TEXTURE_PTR(TEX, TEX->width * V)
#define RENDER_TEXEL_PTR(TEX, U, V) RENDER_TEXTURE_PTR(TEX, TEX->width * V + U)

uint32_t
render_texel64(Bitmap* tex, uint32_t x, uint32_t y)
{
    uint32_t u = x & 63, v = y & 63;
    return *RENDER_TEXEL_PTR(tex, u, v);
}

uint32_t
render_texel(Bitmap* tex, uint32_t x, uint32_t y)
{
    uint32_t u = x % tex->width, v = y % tex->height;
    return *RENDER_TEXEL_PTR(tex, u, v);
}

void
render_floor(Bitmap *view_bitmap, Bitmap *tex, Vector3 *pos, float angle, float fov)
{
    uint32_t vwidth = view_bitmap->width, vheight = view_bitmap->height;
    float view_aspect = (float)vwidth / (float)vheight;
    float dvfov = fov / view_aspect / ((float)vheight / 2);
    float vangle = 0;
    float xdp = cosf(-angle);
    float ydp = sinf(-angle);
    uint32_t hheight = vheight / 2;

    for (uint32_t row = 0; row < hheight; row++)
    {
        vangle += dvfov;
        uint32_t y = row + hheight;
        float fangle = (float)(row + 1) / (float)hheight * fov;
        float dist = pos->z / sinf(fangle);
        float hdist = pos->z / tanf(fangle);
        uint32_t *prow = RENDER_TEXTURE_ROW_PTR(view_bitmap, y);

        uint32_t x = vwidth / 2;

        float uo = ydp * hdist;
        float vo = xdp * hdist;

        float u = uo;
        float v = vo;

        for (int col = 0; col < vwidth / 2; col++)
        {
            u += (dist/PIXEL_FETCH_DIV * -xdp);
            v += (dist/PIXEL_FETCH_DIV * ydp);
            uint32_t fc = render_texel64(tex, (int32_t) (u + pos->x), (int32_t) (v + pos->y));
            *(prow+x++) = fc;
        }

        x = vwidth / 2 - 1;

        u = uo;
        v = vo;

        for (int col = 0; col < vwidth / 2; col++)
        {
            u += (dist/PIXEL_FETCH_DIV * xdp);
            v += (dist/PIXEL_FETCH_DIV * -ydp);
            uint32_t fc = render_texel64(tex, (int32_t) (u + pos->x), (int32_t) (v + pos->y));
            *(prow+x--) = fc;
        }
    }
}

void
render_sky(Bitmap *view_bitmap, Bitmap *tex, float angle, float fov) {
    uint32_t vwidth = view_bitmap->width, vheight = view_bitmap->height;
    uint32_t hheight = vheight / 2;
    float dhfov = fov / ((float)vwidth / 2);

    for (uint32_t row = 0; row < hheight; row++)
    {
        uint32_t *prow = RENDER_TEXTURE_ROW_PTR(view_bitmap, row);

        uint32_t tcol = (uint32_t)(angle / ANGLE_MAX * 1280);
        float hangle = dhfov / 2;
        uint32_t x = vwidth / 2;

        for (int col = 0; col < vwidth / 2; col++)
        {
            float hpscale = cosf(hangle);
            float hpoff = 1 - hpscale;
            uint32_t fc = render_texel(tex, tcol, (uint32_t)((float)row * hpscale + (hpoff * 100)));
            *(prow+x++) = fc;
            tcol++;
            hangle += dhfov;
        }

        tcol = (uint32_t)(angle / ANGLE_MAX * 1280) - 1;
        hangle = -dhfov / 2;
        x = vwidth / 2 - 1;

        for (int col = 0; col < vwidth / 2; col++)
        {
            float hpscale = cosf(hangle);
            float hpoff = 1 - hpscale;
            uint32_t fc = render_texel(tex, tcol, (uint32_t)((float)row * hpscale + (hpoff * 100)));
            *(prow+x--) = fc;
            tcol--;
            hangle -= dhfov;
        }
    }
}