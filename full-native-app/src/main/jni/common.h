#ifndef _COMMON_H_
#define _COMMON_H_

#pragma once

#include <jni.h>
#include <string.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Throws a new exception using the given exception class
 * and exception message.
 *
 * @param env JNIEnv interface.
 * @param className class name.
 * @param message exception message.
 */
void ThrowException(
    JNIEnv* env,
    const char* className,
    const char* message);

/**
 * Throws a new exception using the given exception class
 * and errno.
 *
 * @param env JNIEnv interface.
 * @param className class name.
 * @param message exception message.
 */
void ThrowErrnoException(
    JNIEnv* env,
    const char* className,
    int errnum);

#ifdef __cplusplus
}
#endif
#endif
