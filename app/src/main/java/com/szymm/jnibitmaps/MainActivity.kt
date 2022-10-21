package com.szymm.jnibitmaps

import android.annotation.SuppressLint
import android.graphics.Bitmap
import android.os.Bundle
import android.os.Handler
import android.widget.ImageView
import android.widget.Toast
import androidx.appcompat.app.AppCompatActivity
import androidx.core.os.HandlerCompat

class MainActivity : AppCompatActivity() {
    private lateinit var view: ImageView
    private lateinit var viewBitmap: Bitmap

    private lateinit var bitmapManager: BitmapManager

    private lateinit var renderThread: Thread

    @SuppressLint("ClickableViewAccessibility")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        view = findViewById(R.id.imageView)
        viewBitmap = Bitmap.createBitmap(320, 200, Bitmap.Config.ARGB_8888)

        bitmapManager = BitmapManager(this, 16, Bitmap.Config.ARGB_8888)

        bitmapManager.loadResource(R.drawable.htile0)
        bitmapManager.loadResource(R.drawable.sky0)

        if (!this::renderThread.isInitialized || renderThread.isInterrupted) {
            renderThread = Thread {
                initViewBitmap(viewBitmap)
                initTexBitmaps(bitmapManager)
                while (!renderThread.isInterrupted) {
                    render(viewBitmap)

                    view.setImageBitmap(viewBitmap)
                    view.invalidate()

                    Thread.sleep(33)
                }
            }
            renderThread.setUncaughtExceptionHandler { thread, eh ->
                val threadMessage = "Exception on ${thread.name}:\n ${eh.message}"
                System.err.println(threadMessage)
                Toast.makeText(this, threadMessage, Toast.LENGTH_LONG).show()
            }
        }
    }

    override fun onResume() {
        super.onResume()

        if (!renderThread.isAlive) {
            Thread.sleep(5000L)
            renderThread.start()
        }
    }

    override fun onPause() {
        super.onPause()

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