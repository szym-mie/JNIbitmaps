//
// Created by szymm on 13.10.2022.
//

#ifndef JNIBITMAPS_BITMAP_MANAGER_H
#define JNIBITMAPS_BITMAP_MANAGER_H

#include <jni.h>

typedef struct
{
    uint32_t width, height;
    uint32_t *pixels;
    jintArray jni_array;
} Bitmap;

#define BITMAP_MANAGER_NEXT_NDX "nextNdx", "I"

#define BITMAP_MANAGER_GET_AS_ARRAY "getAsArray", "(I)[I"
#define BITMAP_MANAGER_GET_WIDTH_OF "getWidthOf", "(I)I"
#define BITMAP_MANAGER_GET_HEIGHT_OF "getHeightOf", "(I)I"

uint32_t JNICALL
bitmap_manager_get_bitmap_count(
        JNIEnv *env,
        jclass bitmap_manager);

Bitmap * JNICALL
bitmap_manager_get_bitmap(
        JNIEnv *env,
        jclass bitmap_manager,
        int32_t ndx);

void JNICALL
bitmap_manager_free_bitmap(
        JNIEnv *env,
        Bitmap *bitmap);

#endif //JNIBITMAPS_BITMAP_MANAGER_H
