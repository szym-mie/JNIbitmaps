//
// Created by szymm on 13.10.2022.
//

#include "bitmap_manager.h"

#include <jni.h>
#include <stdlib.h>

uint32_t JNICALL
bitmap_manager_get_bitmap_count(
    JNIEnv *env,
    jclass bitmap_manager)
{
    jclass bitmap_manager_class = (*env)->FindClass(env, "com/szymm/jnibitmaps/BitmapManager");
    jfieldID nextNdx_id = (*env)->GetFieldID(env, bitmap_manager_class, BITMAP_MANAGER_NEXT_NDX);
    return (*env)->GetIntField(env, bitmap_manager, nextNdx_id);
}

Bitmap * JNICALL
bitmap_manager_get_bitmap(
    JNIEnv *env,
    jclass bitmap_manager,
    int32_t ndx)
{
    jclass bitmap_manager_class = (*env)->FindClass(env, "com/szymm/jnibitmaps/BitmapManager");
    jmethodID getWidthOf_id = (*env)->GetMethodID(env, bitmap_manager_class, BITMAP_MANAGER_GET_WIDTH_OF);
    jmethodID getHeightOf_id = (*env)->GetMethodID(env, bitmap_manager_class, BITMAP_MANAGER_GET_HEIGHT_OF);
    jmethodID getAsArray_id = (*env)->GetMethodID(env, bitmap_manager_class, BITMAP_MANAGER_GET_AS_ARRAY);

    jint width = (*env)->CallIntMethod(env, bitmap_manager, getWidthOf_id, (jint)ndx);
    jint height = (*env)->CallIntMethod(env, bitmap_manager, getHeightOf_id, (jint)ndx);
    jarray array = (*env)->CallObjectMethod(env, bitmap_manager, getAsArray_id, (jint)ndx);

    if (array == NULL) return NULL;

    Bitmap *bitmap = malloc(sizeof(Bitmap));
    if (bitmap == NULL) return NULL;

    jboolean isCopy;
    bitmap->pixels = (uint32_t *)(*env)->GetIntArrayElements(env, array, &isCopy);
    bitmap->jni_array = array;
    bitmap->width = (int32_t)width;
    bitmap->height = (int32_t)height;

    return bitmap;
}

void JNICALL
bitmap_manager_free_bitmap(
    JNIEnv *env,
    Bitmap *bitmap)
{
    if (bitmap == NULL) return;

    (*env)->ReleaseIntArrayElements(env, bitmap->jni_array, (jint *)bitmap->pixels, 0);

    free(bitmap);
}