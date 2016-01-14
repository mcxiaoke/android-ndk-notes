package com.mcxiaoke.ndk;

import android.os.Bundle;
import android.view.Surface;
import android.view.SurfaceHolder;
import android.view.SurfaceHolder.Callback;
import android.view.SurfaceView;
import butterknife.Bind;
import butterknife.ButterKnife;

import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Author: mcxiaoke
 * Date:  2016/1/14 23:00
 */
public class NativeWindowPlayerActivity extends AbstractPlayerActivity {
    @Bind(R.id.surface_view)
    SurfaceView surfaceView;

    private final AtomicBoolean isPlaying = new AtomicBoolean();
    private SurfaceHolder surfaceHolder;

    @Override
    protected void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_native_window_player);
        ButterKnife.bind(this);
        surfaceHolder = surfaceView.getHolder();
        surfaceHolder.addCallback(callback);

    }

    private final Callback callback = new Callback() {
        @Override
        public void surfaceCreated(final SurfaceHolder surfaceHolder) {
            isPlaying.set(true);
            new Thread(renderer).start();
        }

        @Override
        public void surfaceChanged(final SurfaceHolder surfaceHolder,
                                   final int i, final int i1, final int i2) {

        }

        @Override
        public void surfaceDestroyed(final SurfaceHolder surfaceHolder) {
            isPlaying.set(false);
        }
    };

    private final Runnable renderer = new Runnable() {
        public void run() {
// Get the surface instance
            Surface surface = surfaceHolder.getSurface();
// Initialize the native window
            Native.initNW(avi, surface);
// Calculate the delay using the frame rate
            long frameDelay = (long) (1000 / Native.getFrameRate(avi));
// Start rendering while playing
            while (isPlaying.get()) {
// Render the frame to the surface
                Native.renderNW(avi, surface);
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
