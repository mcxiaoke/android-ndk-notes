package com.mcxiaoke.ndk;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.os.SystemClock;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 23:01
 */
public class BitmapPlayerActivity extends AbstractPlayerActivity {
    /**
     * Is playing.
     */
    private final AtomicBoolean isPlaying = new AtomicBoolean();

    private SurfaceView surfaceView;

    /**
     * Surface holder.
     */
    private SurfaceHolder surfaceHolder;

    /**
     * On create.
     *
     * @param savedInstanceState saved state.
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_bitmap_player);

        surfaceView = (SurfaceView)
                findViewById(R.id.surface_view);

        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(surfaceHolderCallback);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    /**
     * Surface holder callback listens for surface events.
     */
    private final Callback surfaceHolderCallback = new Callback() {
        public void surfaceChanged(SurfaceHolder holder, int format,
                                   int width, int height) {
        }

        public void surfaceCreated(SurfaceHolder holder) {
            // Start playing since surface is ready
            isPlaying.set(true);

            // Start renderer on a separate thread
            new Thread(renderer).start();
        }

        public void surfaceDestroyed(SurfaceHolder holder) {
            // Stop playing since surface is destroyed
            isPlaying.set(false);
        }
    };

    /**
     * Renderer runnable renders the video frames from the
     * AVI file to the surface through a bitmap.
     */
    private final Runnable renderer = new Runnable() {
        public void run() {
            int w = Native.getWidth(avi);
            int h = Native.getHeight(avi);
            // Create a new bitmap to hold the frames
            Bitmap bitmap = Bitmap.createBitmap(w, h, Bitmap.Config.RGB_565);

            // Calculate the delay using the frame rate
            long frameDelay = (long) (1000 / Native.getFrameRate(avi));

            int cw;
            int ch;
            int left;
            int top;
            cw = surfaceView.getWidth();
            ch = surfaceView.getHeight();
            left = (cw - w) >> 1;
            top = (ch - h) >> 1;
            // Start rendering while playing
            while (isPlaying.get()) {
                long start = SystemClock.elapsedRealtime();
                // Render the frame to the bitmap
                Native.render(avi, bitmap);
                // Lock canvas
                Canvas canvas = surfaceHolder.lockCanvas();
                // Draw the bitmap to the canvas
                start = SystemClock.elapsedRealtime();
                canvas.drawBitmap(bitmap, left, top, null);
                // Post the canvas for displaying
                start = SystemClock.elapsedRealtime();
                surfaceHolder.unlockCanvasAndPost(canvas);
                long ms = SystemClock.elapsedRealtime() - start;
                Log.v("DEBUG", "renderer time: " + ms + "ms" + " -> " + frameDelay);
                // Wait for the next frame
                try {
                    Thread.sleep(Math.max(0, frameDelay - ms));
                } catch (Exception e) {
                    e.printStackTrace();
                    break;
                }
            }
        }
    };


}
