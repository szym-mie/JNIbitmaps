package com.szymm.jnibitmaps

import android.graphics.Bitmap
import android.os.Bundle
import android.widget.ImageView
import androidx.appcompat.app.AppCompatActivity

class MainActivity : AppCompatActivity() {
    private lateinit var bitmapManager: BitmapManager

    private lateinit var renderThread: Thread

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        val view = findViewById<ImageView>(R.id.imageView)
        val viewBitmap = Bitmap.createBitmap(320, 200, Bitmap.Config.ARGB_8888)

        bitmapManager = BitmapManager(this, 16, Bitmap.Config.ARGB_8888)

        bitmapManager.loadResource(R.drawable.htile0)
        bitmapManager.loadResource(R.drawable.sky0)

        if (!this::renderThread.isInitialized || !renderThread.isAlive) {
            renderThread = Thread {
                try {
                    initViewBitmap(viewBitmap)
                    initTexBitmaps(bitmapManager)
                    while (!renderThread.isInterrupted) {
                        render(viewBitmap)

                        view.setImageBitmap(viewBitmap)
                        view.invalidate()

                        Thread.sleep(33)
                    }
                } catch (e: Exception) {
                    System.err.println(e.message)
                    return@Thread
                }
            }

            renderThread.start()
        }
    }

    override fun onDestroy() {
        super.onDestroy()

        try {
            renderThread.interrupt()
        } catch (_: InterruptedException) {}
    }

    /**
     * A native method that is implemented by the 'jnibitmaps' native library,
     * which is packaged with this application.
     */
    private external fun initTexBitmaps(bitmapManager: BitmapManager)
    private external fun initViewBitmap(bitmap: Bitmap)
    private external fun initData()
    private external fun render(bitmap: Bitmap)

    companion object {
        // Used to load the 'jnibitmaps' library on application startup.
        init {
            System.loadLibrary("jnibitmaps")
        }
    }
}