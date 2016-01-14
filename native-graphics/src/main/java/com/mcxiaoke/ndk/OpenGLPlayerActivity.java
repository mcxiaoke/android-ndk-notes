package com.mcxiaoke.ndk;

import android.opengl.GLSurfaceView;
import android.opengl.GLSurfaceView.Renderer;
import android.os.Bundle;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;
import java.util.concurrent.atomic.AtomicBoolean;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 23:01
 */
public class OpenGLPlayerActivity extends AbstractPlayerActivity {
    /**
     * Is playing.
     */
    private final AtomicBoolean isPlaying = new AtomicBoolean();

    private long instance;

    private GLSurfaceView glSurfaceView;

    /**
     * On create.
     *
     * @param savedInstanceState saved state.
     */
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_opengl_player);

        glSurfaceView = (GLSurfaceView)
                findViewById(R.id.surface_view);
        glSurfaceView.setRenderer(renderer);
        glSurfaceView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    @Override
    protected void onStart() {
        super.onStart();
        instance = Native.init(avi);
    }

    @Override
    protected void onResume() {
        super.onResume();
        glSurfaceView.onResume();
    }

    @Override
    protected void onPause() {
        super.onPause();
        glSurfaceView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        Native.free(instance);
        instance = 0;
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    /**
     * Request rendering based on the frame rate.
     */
    private final Runnable player = new Runnable() {
        public void run() {
            // Calculate the delay using the frame rate
            long frameDelay = (long) (1000 / Native.getFrameRate(avi));

            // Start rendering while playing
            while (isPlaying.get()) {
                // Request rendering
                glSurfaceView.requestRender();

                // Wait for the next frame
                try {
                    Thread.sleep(frameDelay);
                } catch (InterruptedException e) {
                    break;
                }
            }
        }
    };

    /**
     * OpenGL renderer.
     */
    private final Renderer renderer = new Renderer() {
        public void onDrawFrame(GL10 gl) {
            // Render the next frame
            if (!Native.renderOpenGL(instance, avi)) {
                isPlaying.set(false);
            }
        }

        public void onSurfaceChanged(GL10 gl, int width, int height) {

        }

        public void onSurfaceCreated(GL10 gl, EGLConfig config) {
            // Initialize the OpenGL surface
            Native.initSurface(instance, avi);

            // Start playing since surface is ready
            isPlaying.set(true);

            // Start player
            new Thread(player).start();
        }
    };


}
