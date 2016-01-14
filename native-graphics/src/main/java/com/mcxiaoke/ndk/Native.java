package com.mcxiaoke.ndk;

import android.graphics.Bitmap;

import java.io.IOException;

/**
 * Author: mcxiaoke
 * Date:  2016/1/13 23:40
 */
public class Native {


    /**
     * Opens the given AVI file and returns a file descriptor.
     *
     * @param fileName file name.
     * @return file descriptor.
     * @throws IOException
     */
    public native static long open(String fileName)
            throws IOException;

    /**
     * Get the video width.
     *
     * @param avi file descriptor.
     * @return video width.
     */
    public native static int getWidth(long fd);

    /**
     * Get the video height.
     *
     * @param avi file descriptor.
     * @return video height.
     */
    public native static int getHeight(long fd);

    /**
     * Gets the frame rate.
     *
     * @param avi file descriptor.
     * @return frame rate.
     */
    public native static double getFrameRate(long fd);

    /**
     * Closes the given AVI file based on given file descriptor.
     *
     * @param avi file descriptor.
     */
    public native static void close(long fd);

    /**
     * Renders the frame from given AVI file descriptor to
     * the given Bitmap.
     *
     * @param avi    file descriptor.
     * @param bitmap bitmap instance.
     * @return true if there are more frames, false otherwise.
     */
    public native static boolean renderBitmap(long fd, Bitmap bitmap);

    /**
     * Initializes the native renderer.
     *
     * @param avi file descriptor.
     * @return native instance.
     */
    public native static long init(long avi);

    /**
     * Initializes the OpenGL surface.
     *
     * @param instance native instance.
     */
    public native static void initSurface(long instance, long avi);

    /**
     * Renders the frame from given AVI file descriptor.
     *
     * @param instance native instance.
     * @param avi      file descriptor.
     * @return true if there are more frames, false otherwise.
     */
    public native static boolean renderOpenGL(long instance, long avi);

    /**
     * Free the native renderer.
     *
     * @param instance native instance.
     */
    public native static void free(long instance);

    static {
        System.loadLibrary("player");
    }
}
