/*
* @Author: mcxiaoke
* @Date:   2016-01-13 22:46:42
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-14 23:08:44
*/

extern "C" {
#include <avilib.h>
}

#include <android/bitmap.h>
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <malloc.h>
#include "common.h"
#include "com_mcxiaoke_ndk_Native.h"

struct Instance
{
    char* buffer;
    GLuint texture;

    Instance():
        buffer(0),
        texture(0)
    {}
};


#ifdef __cplusplus
extern "C" {
#endif

// http://fossies.org/linux/privat/transcode-1.1.7.tar.gz/

jlong Java_com_mcxiaoke_ndk_Native_open(
    JNIEnv* env,
    jclass clazz,
    jstring fileName)
{
    avi_t* avi = 0;

    // Get the file name as a C string
    const char* cFileName = env->GetStringUTFChars(fileName, 0);
    if (0 == cFileName)
    {
        goto exit;
    }

    // Open the AVI file
    avi = AVI_open_input_file(cFileName, 1);

    // Release the file name
    env->ReleaseStringUTFChars(fileName, cFileName);

    // If AVI file cannot be opened throw an exception
    if (0 == avi)
    {
        ThrowException(env, "java/io/IOException", AVI_strerror());
    }

exit:
    return (jlong) avi;
}

jint Java_com_mcxiaoke_ndk_Native_getWidth(
    JNIEnv* env,
    jclass clazz,
    jlong avi)
{
    return AVI_video_width((avi_t*) avi);
}

jint Java_com_mcxiaoke_ndk_Native_getHeight(
    JNIEnv* env,
    jclass clazz,
    jlong avi)
{
    return AVI_video_height((avi_t*) avi);
}

jdouble Java_com_mcxiaoke_ndk_Native_getFrameRate(
    JNIEnv* env,
    jclass clazz,
    jlong avi)
{
    return AVI_frame_rate((avi_t*) avi);
}

void Java_com_mcxiaoke_ndk_Native_close(
    JNIEnv* env,
    jclass clazz,
    jlong avi)
{
    AVI_close((avi_t*) avi);
}

jboolean Java_com_mcxiaoke_ndk_Native_renderBitmap(
    JNIEnv* env,
    jclass clazz,
    jlong avi,
    jobject bitmap)
{
    jboolean isFrameRead = JNI_FALSE;

    char* frameBuffer = 0;
    long frameSize = 0;
    int keyFrame = 0;

    // Lock bitmap and get the raw bytes
    if (0 > AndroidBitmap_lockPixels(env, bitmap, (void**) &frameBuffer))
    {
        ThrowException(env, "java/io/IOException",
                       "Unable to lock pixels.");
        goto exit;
    }

    // Read AVI frame bytes to bitmap
    frameSize = AVI_read_frame((avi_t*) avi, frameBuffer, &keyFrame);

    // Unlock bitmap
    if (0 > AndroidBitmap_unlockPixels(env, bitmap))
    {
        ThrowException(env, "java/io/IOException",
                       "Unable to unlock pixels.");
        goto exit;
    }

    // Check if frame is successfully read
    if (0 < frameSize)
    {
        isFrameRead = JNI_TRUE;
    }

exit:
    return isFrameRead;
}



// opengl player
/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    init
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_com_mcxiaoke_ndk_Native_init
(JNIEnv *env, jclass clazz, jlong avi)
{
    Instance* instance = 0;

    long frameSize = AVI_frame_size((avi_t*) avi, 0);
    if (0 >= frameSize)
    {
        ThrowException(env, "java/lang/RuntimeException",
                       "Unable to get the frame size.");
        goto exit;
    }

    instance = new Instance();
    if (0 == instance)
    {
        ThrowException(env, "java/lang/RuntimeException",
                       "Unable to allocate instance.");
        goto exit;
    }

    instance->buffer = (char*) malloc(frameSize);
    if (0 == instance->buffer)
    {
        ThrowException(env, "java/lang/RuntimeException",
                       "Unable to allocate buffer.");
        delete instance;
        instance = 0;
    }

exit:
    return (jlong) instance;
}

/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    initSurface
 * Signature: (JJ)V
 */
JNIEXPORT void JNICALL Java_com_mcxiaoke_ndk_Native_initSurface
(JNIEnv *env, jclass clazz, jlong inst, jlong avi)
{
    Instance* instance = (Instance*) inst;

    // Enable textures
    glEnable(GL_TEXTURE_2D);

    // Generate one texture object
    glGenTextures(1, &instance->texture);

    // Bind to generated texture
    glBindTexture(GL_TEXTURE_2D, instance->texture);

    int frameWidth = AVI_video_width((avi_t*) avi);
    int frameHeight = AVI_video_height((avi_t*) avi);

    LOGD("initSurface() w=%d, h=%d", frameWidth, frameHeight);

    // Crop the texture rectangle
    GLint rect[] = {0, frameHeight, frameWidth, -frameHeight};
    glTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_CROP_RECT_OES, rect);

    // Full color
    glColor4f(1.0, 1.0, 1.0, 1.0);

    // Generate an empty texture
    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGB,
                 frameWidth,
                 frameHeight,
                 0,
                 GL_RGB,
                 GL_UNSIGNED_SHORT_5_6_5,
                 0);
}

/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    render
 * Signature: (JJ)Z
 */
JNIEXPORT jboolean JNICALL Java_com_mcxiaoke_ndk_Native_renderOpenGL
(JNIEnv *env, jclass clazz, jlong inst, jlong avi)
{
    Instance* instance = (Instance*) inst;

    jboolean isFrameRead = JNI_FALSE;
    int keyFrame = 0;

    // Read AVI frame bytes to bitmap
    long frameSize = AVI_read_frame((avi_t*) avi,
                                    instance->buffer,
                                    &keyFrame);

    // Check if frame read
    if (0 >= frameSize)
    {
        goto exit;
    }

    // Frame read
    isFrameRead = JNI_TRUE;

    // Update the texture with the new frame
    glTexSubImage2D(GL_TEXTURE_2D,
                    0,
                    0,
                    0,
                    AVI_video_width((avi_t*) avi),
                    AVI_video_height((avi_t*) avi),
                    GL_RGB,
                    GL_UNSIGNED_SHORT_5_6_5,
                    instance->buffer);

    // Draw texture
    glDrawTexiOES(0, 0, 0,
                  AVI_video_width((avi_t*) avi),
                  AVI_video_height((avi_t*) avi));
    /**
    to fix error: glDrawTexiOES was not declared in this scope
    add this to Android.mk: LOCAL_CFLAGS += -DGL_GLEXT_PROTOTYPES
    **/
exit:
    return isFrameRead;
}

/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    free
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_com_mcxiaoke_ndk_Native_free
(JNIEnv *evn, jclass clazz, jlong inst)
{
    Instance* instance = (Instance*) inst;

    if (0 != instance)
    {
        free(instance->buffer);
        delete instance;
    }
}

// native window render
/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    initNW
 * Signature: (JLandroid/view/Surface;)V
 */
JNIEXPORT void JNICALL Java_com_mcxiaoke_ndk_Native_initNW
(JNIEnv *env, jclass clazz, jlong avi, jobject surface)
{
    // Get the native window from the surface
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(
                                      env, surface);
    if (0 == nativeWindow)
    {
        ThrowException(env, "java/lang/RuntimeException",
                       "Unable to get native window from surface.");
        goto exit;
    }

    // Set the buffers geometry to AVI movie frame dimensions
    // If these are different than the window's physical size
    // then the buffer will be scaled to match that size.
    if (0 > ANativeWindow_setBuffersGeometry(nativeWindow,
            AVI_video_width((avi_t*) avi),
            AVI_video_height((avi_t*) avi),
            WINDOW_FORMAT_RGB_565))
    {
        ThrowException(env, "java/lang/RuntimeException",
                       "Unable to set buffers geometry.");
    }

    // Release the native window
    ANativeWindow_release(nativeWindow);
    nativeWindow = 0;

exit:
    return;
}

/*
 * Class:     com_mcxiaoke_ndk_Native
 * Method:    renderNW
 * Signature: (JLandroid/view/Surface;)Z
 */
JNIEXPORT jboolean JNICALL Java_com_mcxiaoke_ndk_Native_renderNW
(JNIEnv *env, jclass clazz, jlong avi, jobject surface)
{
    jboolean isFrameRead = JNI_FALSE;

    long frameSize = 0;
    int keyFrame = 0;

    // Get the native window from the surface
    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(
                                      env, surface);
    if (0 == nativeWindow)
    {
        ThrowException(env, "java/io/RuntimeException",
                       "Unable to get native window from surface.");
        goto exit;
    }

    // Lock the native window and get access to raw buffer
    ANativeWindow_Buffer windowBuffer;
    if (0 > ANativeWindow_lock(nativeWindow, &windowBuffer, 0))
    {
        ThrowException(env, "java/io/RuntimeException",
                       "Unable to lock native window.");
        goto release;
    }

    // Read AVI frame bytes to raw buffer
    frameSize = AVI_read_frame((avi_t*) avi,
                               (char*) windowBuffer.bits,
                               &keyFrame);

    // Check if frame is successfully read
    if (0 < frameSize)
    {
        isFrameRead = JNI_TRUE;
    }

    // Unlock and post the buffer for displaying
    if (0 > ANativeWindow_unlockAndPost(nativeWindow))
    {
        ThrowException(env, "java/io/RuntimeException",
                       "Unable to unlock and post to native window.");
        goto release;
    }

release:
    // Release the native window
    ANativeWindow_release(nativeWindow);
    nativeWindow = 0;

exit:
    return isFrameRead;
}



#ifdef __cplusplus
}
#endif
