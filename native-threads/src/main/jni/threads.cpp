/*
* @Author: mcxiaoke
* @Date:   2016-01-10 21:23:52
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-10 22:08:54
*/

#include <stdio.h>
#include <unistd.h>
#include "nlog.h"
#include "com_example_hellojni_Native.h"

// cached method id
static jmethodID gOnNativeMessage = NULL;

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeInit
(JNIEnv *env, jobject obj) {
    LOGD("nativeInit()");
    if (NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(obj);
        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");
        if (NULL == gOnNativeMessage) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to find method");
        }
    }

}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeFree
(JNIEnv *env, jobject obj) {
    LOGD("nativeFree()");
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeWorker
(JNIEnv *env, jobject obj, jint id, jint interations)
{
    LOGD("nativeWorker(id:%d, interations:%d)", id, interations);
    for (jint i = 0; i < interations; ++i)
    {
        char message[26];
        sprintf(message, "Worker %d: Iteration %d", id, i);
        jstring messageString = env->NewStringUTF(message);
        env->CallVoidMethod(obj, gOnNativeMessage, messageString);
        if (NULL != env->ExceptionOccurred()) {
            break;
        }
        sleep(1);
    }
}
