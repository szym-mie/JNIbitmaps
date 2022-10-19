//
// Created by szymm on 12.10.2022.
//

#include <jni.h>
#include <stdlib.h>
#include <android/bitmap.h>
#include "bitmap_manager.h"
#include "render.h"

#define FOV 0.5f

typedef enum { STRAFE_RIGHT = -1, STRAFE_STOP, STRAFE_LEFT } MoveX;
typedef enum { MOVE_BACKWARD = -1, MOVE_STOP, MOVE_FORWARD } MoveY;

typedef struct
{
    float dir;
    Vector3 *pos;
    MoveX move_x;
    MoveY move_y;
} Player;

AndroidBitmapInfo bitmap_info;
Bitmap view_bitmap;
Bitmap **texs;

Vector3 player_pos = { 0.0f, 0.0f, 16.0f };
Player player = { .pos = &player_pos, .dir = 6.0f, .move_y = MOVE_STOP, .move_x = STRAFE_STOP };

jint JNICALL
bitmap_throw_exception(JNIEnv *env, const char *msg);

extern JNIEXPORT void JNICALL
Java_com_szymm_jnibitmaps_MainActivity_initTexBitmaps(
    JNIEnv *env,
    jobject this,
    jobject bitmap_manager)
{
     uint32_t bitmap_count = bitmap_manager_get_bitmap_count(env, bitmap_manager);

    texs = calloc(bitmap_count, sizeof(Bitmap *));
    if (texs == NULL) bitmap_throw_exception(env, "could not allocate texture library");

    for (uint32_t i = 0; i < bitmap_count; i++)
    {
        Bitmap *bitmap = bitmap_manager_get_bitmap(env, bitmap_manager, i);
        if (bitmap == NULL)
        {
            char msg_buf[80];
            sprintf(msg_buf, "could not get texture ndx:%d", i); // replace with safer call
            bitmap_throw_exception(env, msg_buf);
            return;
        }
        *(texs+i) = bitmap;

        for (uint32_t off = 0; off < bitmap->width * bitmap->height; off++)
        {
            uint32_t col = *(bitmap->pixels + off);
            uint32_t rmask = 0xff;
            uint32_t gmask = 0xff<<8;
            uint32_t bmask = 0xff<<16;
            uint32_t amask = 0xff<<24;

            *(bitmap->pixels + off) = (col&amask)|((col&rmask)<<16)|(col&gmask)|((col&bmask)>>16);
        }
    }
}

extern JNIEXPORT void JNICALL
Java_com_szymm_jnibitmaps_MainActivity_initViewBitmap(
    JNIEnv *env,
    jobject this,
    jobject jbitmap)
{
    int get_info_stat = AndroidBitmap_getInfo(env, jbitmap, &bitmap_info);
    if (get_info_stat) bitmap_throw_exception(env, "could not getInfo");

    view_bitmap.width = bitmap_info.width;
    view_bitmap.height = bitmap_info.height;
}

extern JNIEXPORT void JNICALL
Java_com_szymm_jnibitmaps_MainActivity_render(
    JNIEnv *env,
    jobject this,
    jobject bitmap)
{
    if (AndroidBitmap_lockPixels(env, bitmap, &(view_bitmap.pixels)))
        bitmap_throw_exception(env, "could not lock bitmap");

    // use texture ndx:0
    render_floor(&view_bitmap, texs[0], player.pos, player.dir, FOV);

    render_sky(&view_bitmap, texs[1], player.dir, FOV);

    player.dir += 0.01f;
    player.pos->x += 1.0f;
    player.pos->y += 0.5f;

    AndroidBitmap_unlockPixels(env, bitmap);
}

JNIEXPORT void JNICALL
Java_com_szymm_jnibitmaps_MainActivity_initData(JNIEnv *env, jobject this) {
    // currently empty
}

jint JNICALL
bitmap_throw_exception(JNIEnv *env, const char *msg)
{
    const char *class_name = "java/lang/Exception";
    const char *ncdf_class_name = "java/lang/NoClassDefFoundError";

    jclass exception = (*env)->FindClass(env, class_name);
    if (exception == NULL)
    {
        exception = (*env)->FindClass(env, ncdf_class_name);
        return (*env)->ThrowNew(env, exception, "JNI NPE throw failed.");
    }

    return (*env)->ThrowNew(env, exception, msg);
}
