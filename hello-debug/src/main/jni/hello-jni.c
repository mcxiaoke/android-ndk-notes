/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <string.h>
#include <stdio.h>
#include <jni.h>
#include "com_example_hellojni_HelloJni.h"
#include "nlog.h"

static int counter = 0;

void bufTest() {
    char* buffer;
    size_t i;
    buffer = (char*) malloc(1024);
    for (i = 0; i < 1025; i++)
    {
        buffer[i] = 'a';
    }
    free(buffer);
}

jstring getABIInfo(JNIEnv *env);

/* This is a trivial JNI example where we use a native method
 * to return a new VM String. See the corresponding Java source
 * file located at:
 *
 *   apps/samples/hello-jni/project/src/com/example/hellojni/HelloJni.java
 */
JNIEXPORT jstring JNICALL Java_com_example_hellojni_HelloJni_stringFromJNI
(JNIEnv *env, jclass clazz) {
#if defined(__arm__)
#if defined(__ARM_ARCH_7A__)
#if defined(__ARM_NEON__)
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a/NEON (hard-float)"
#else
#define ABI "armeabi-v7a/NEON"
#endif
#else
#if defined(__ARM_PCS_VFP)
#define ABI "armeabi-v7a (hard-float)"
#else
#define ABI "armeabi-v7a"
#endif
#endif
#else
#define ABI "armeabi"
#endif
#elif defined(__i386__)
#define ABI "x86"
#elif defined(__x86_64__)
#define ABI "x86_64"
#elif defined(__mips64)  /* mips64el-* toolchain defines __mips__ too */
#define ABI "mips64"
#elif defined(__mips__)
#define ABI "mips"
#elif defined(__aarch64__)
#define ABI "arm64-v8a"
#else
#define ABI "unknown"
#endif
    printf("log message to stdout\n");
    LOGV("The stringFromJNI is called.");
    ASSERT(0 != env, "JNIEnv cannot be NULL.");
    LOGD("env=%p class=%p", env, clazz);
    LOGW("instance String From JNI");
    LOGI("Returning a new string.");
    LOGE("ERROR: error message text.");
    LOGI("PID = %d, UID = %d", getpid(), getuid());
    return getABIInfo(env);
}

JNIEXPORT jstring JNICALL Java_com_example_hellojni_HelloJni_instanceStringFromJNI
(JNIEnv *env, jobject thiz) {
    return (*env)->NewStringUTF(env, "instance String From JNI");
}

jstring getABIInfo(JNIEnv *env) {
    // env = 0; // make a crash
    // jintArray javaArray = (*env)->NewIntArray(env, -1); // for checkjni
    char str [80];
    sprintf(str, "Hello from JNI ! Compiled with ABI: %s Count: %d", ABI, ++counter);
    return (*env)->NewStringUTF(env, str);
}
