package com.szymm.jnibitmaps

import android.content.Context
import android.graphics.Bitmap
import android.graphics.BitmapFactory

class BitmapManager(
    private val ctx: Context,
    bitmapCount: Int,
    private val bitmapConfig: Bitmap.Config
) {
    private val bitmapArray: Array<Bitmap> = Array(
        bitmapCount
    ) { Bitmap.createBitmap(64, 64, bitmapConfig) }

    private var nextNdx: Int = 0

    operator fun get(ndx: Int): Bitmap = bitmapArray[ndx]
    operator fun set(ndx: Int, bitmap: Bitmap) { bitmapArray[ndx] = bitmap }

    fun getWidthOf(ndx: Int): Int = this[ndx].width
    fun getHeightOf(ndx: Int): Int = this[ndx].height

    fun getAsArray(ndx: Int): IntArray {
        val bitmap = this[ndx]
        val width = bitmap.width
        val height = bitmap.height
        val array = IntArray(width * height)

        bitmap.getPixels(array, 0, width, 0, 0, width, height)

        return array
    }

    fun loadResource(id: Int) {
        val options = BitmapFactory.Options().apply {
            inPreferredConfig = bitmapConfig
            inDensity = 1
            inTargetDensity = 1
            inScaled = false
        }
        this[nextNdx++] = BitmapFactory.decodeResource(ctx.resources, id, options)
    }
}