/*
* @Author: mcxiaoke
* @Date:   2016-01-13 22:46:42
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-14 08:49:09
*/

extern "C" {
#include <avilib.h>
}

#include "common.h"
#include "com_mcxiaoke_ndk_Native.h"
#include <android/bitmap.h>

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

jboolean Java_com_mcxiaoke_ndk_Native_render(
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

#ifdef __cplusplus
}
#endif
