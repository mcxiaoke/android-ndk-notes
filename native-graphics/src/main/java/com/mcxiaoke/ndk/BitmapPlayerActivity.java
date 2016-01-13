package com.mcxiaoke.ndk;

import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
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

        SurfaceView surfaceView = (SurfaceView)
                findViewById(R.id.surface_view);

        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(surfaceHolderCallback);
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
            // Create a new bitmap to hold the frames
            Bitmap bitmap = Bitmap.createBitmap(
                    Native.getWidth(avi),
                    Native.getHeight(avi),
                    Bitmap.Config.RGB_565);

            // Calculate the delay using the frame rate
            long frameDelay = (long) (1000 / Native.getFrameRate(avi));

            // Start rendering while playing
            while (isPlaying.get()) {
                // Render the frame to the bitmap
                Native.render(avi, bitmap);

                // Lock canvas
                Canvas canvas = surfaceHolder.lockCanvas();

                // Draw the bitmap to the canvas
                canvas.drawBitmap(bitmap, 0, 0, null);

                // Post the canvas for displaying
                surfaceHolder.unlockCanvasAndPost(canvas);

                // Wait for the next frame
                try {
                    Thread.sleep(frameDelay);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };


}
