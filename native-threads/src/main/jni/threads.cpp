/*
* @Author: mcxiaoke
* @Date:   2016-01-10 21:23:52
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-10 23:11:32
*/

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include "nlog.h"
#include "com_example_hellojni_Native.h"


// cached method id
static jmethodID gOnNativeMessage = NULL;
// JavaVM interface pointer
static JavaVM* gVM = NULL;
// Global reference to object
static jobject gObj = NULL;

// mutex instance
static pthread_mutex_t mutex;

struct NativeWorkerArgs {
    jint id;
    jint interations;
};

static void* nativeWorkerThread(void* args) {
    JNIEnv* env = NULL;
    // attach current thread to jvm
    // and obtain JNIEnv interface pointer
    if (0 == gVM->AttachCurrentThread(&env, NULL)) {
        NativeWorkerArgs* nativeWorkerArgs = (NativeWorkerArgs*) args;
        // run the native worker in thread context
        Java_com_example_hellojni_Native_nativeWorker(env,
                gObj,
                nativeWorkerArgs->id,
                nativeWorkerArgs->interations);
        // free args
        delete nativeWorkerArgs;
        // detach current thread from jvm
        gVM->DetachCurrentThread();
    }
    return (void*)1;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    LOGI("JNI_OnLoad");
    gVM = vm;
    return JNI_VERSION_1_6;
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeInit
(JNIEnv *env, jobject obj) {
    LOGD("nativeInit()");
    if (NULL == gObj) {
        gObj = env->NewGlobalRef(obj);
        if (NULL == gObj) {
            goto exit;
        }
    }
    if (NULL == gOnNativeMessage) {
        jclass clazz = env->GetObjectClass(obj);
        gOnNativeMessage = env->GetMethodID(clazz, "onNativeMessage", "(Ljava/lang/String;)V");
        if (NULL == gOnNativeMessage) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to find method");
        }
    }
    if (0 != pthread_mutex_init(&mutex, NULL)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to initialize mutex");
        goto exit;
    }
exit:
    return;
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeFree
(JNIEnv *env, jobject obj) {
    LOGD("nativeFree()");
    if (NULL != gObj) {
        env->DeleteGlobalRef(gObj);
        gObj = NULL;
    }
    // destroy mutex
    if (0 != pthread_mutex_destroy(&mutex)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to destroy mutex");
    }
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_nativeWorker
(JNIEnv *env, jobject obj, jint id, jint interations)
{
    LOGD("nativeWorker(id:%d, interations:%d)", id, interations);
    // lock mutex
    if (0 != pthread_mutex_lock(&mutex)) {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to lock mutex");
        goto exit;
    }
    for (jint i = 0; i < interations; ++i)
    {
        char message[26];
        sprintf(message, "Worker1 %d: Iteration %d", id, i);
        jstring messageString = env->NewStringUTF(message);
        env->CallVoidMethod(obj, gOnNativeMessage, messageString);
        if (NULL != env->ExceptionOccurred()) {
            break;
        }
        sleep(1);
    }
    // Unlock mutex
    if (0 != pthread_mutex_unlock(&mutex))
    {
        jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
        env->ThrowNew(exceptionClazz, "Unable to unlock mutex");
    }
exit:
    return;
}

JNIEXPORT void JNICALL Java_com_example_hellojni_Native_posixThreads
(JNIEnv *env, jobject obj, jint threads, jint interations) {
    pthread_t* handles = new pthread_t[threads];

    for (jint i = 0; i < threads; ++i)
    {
        NativeWorkerArgs* args = new NativeWorkerArgs();
        args->id = i;
        args->interations = interations;
        // thread create and run
        int result = pthread_create(
                         &handles[i], NULL, nativeWorkerThread, (void*)args);
        if (0 != result) {
            jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
            env->ThrowNew(exceptionClazz, "Unable to create thread");
            goto exit;
        }
    }
    if (1) {
        return;
    }
// wait for threads to terminate
    for (jint i = 0; i < threads; ++i)
    {
        void* result = NULL;
        if (0 != pthread_join(handles[i], &result)) {
            if (0 != result) {
                jclass exceptionClazz = env->FindClass("java/lang/RuntimeException");
                env->ThrowNew(exceptionClazz, "Unable to join thread");
                goto exit;
            }
        } else {
            char message[26];
            sprintf(message, "Worker2 %d: Iteration %d", i, (int)result);
            jstring messageString = env->NewStringUTF(message);
            env->CallVoidMethod(obj, gOnNativeMessage, messageString);
            if (NULL != env->ExceptionOccurred()) {
                goto exit;
            }
        }
    }

exit:
    return;
}
