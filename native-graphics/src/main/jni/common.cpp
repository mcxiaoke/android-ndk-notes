/*
* @Author: mcxiaoke
* @Date:   2016-01-13 22:50:09
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-13 23:55:30
*/

#include "common.h"

#define MAX_LOG_MESSAGE_LENGTH 256

#ifdef __cplusplus
extern "C" {
#endif

// throw errno exception
void ThrowErrnoException(
    JNIEnv* env,
    const char* className,
    int errnum) {
    char buffer[MAX_LOG_MESSAGE_LENGTH];
    if (-1 == strerror_r(errnum, buffer, MAX_LOG_MESSAGE_LENGTH)) {
        strerror_r(errno, buffer, MAX_LOG_MESSAGE_LENGTH);
        ThrowException(env, className, buffer);
    }
}

void ThrowException(
    JNIEnv* env,
    const char* className,
    const char* message)
{
    // Get the exception class
    jclass clazz = env->FindClass(className);

    // If exception class is found
    if (NULL != clazz)
    {
        // Throw exception
        env->ThrowNew(clazz, message);

        // Release local class reference
        env->DeleteLocalRef(clazz);
    }
}

#ifdef __cplusplus
}
#endif
