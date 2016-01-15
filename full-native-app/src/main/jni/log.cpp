/*
* @Author: mcxiaoke
* @Date:   2016-01-15 21:19:13
* @Last Modified by:   mcxiaoke
* @Last Modified time: 2016-01-15 21:21:55
*/

#include <stdarg.h>
#include <android/log.h>
#include "log.h"

void Log::error(const char* message, ...) {
    va_list args;
    va_start(args, message);
    __android_log_vprint(ANDROID_LOG_ERROR, "NativeApp", message, args);
    __android_log_print(ANDROID_LOG_ERROR, "NativeApp", "\n");
    va_end(args);
}

void Log::warn(const char* message, ...) {
    va_list args;
    va_start(args, message);
    __android_log_vprint(ANDROID_LOG_WARN, "NativeApp", message, args);
    __android_log_print(ANDROID_LOG_WARN, "NativeApp", "\n");
    va_end(args);
}


void Log::info(const char* message, ...) {
    va_list args;
    va_start(args, message);
    __android_log_vprint(ANDROID_LOG_INFO, "NativeApp", message, args);
    __android_log_print(ANDROID_LOG_INFO, "NativeApp", "\n");
    va_end(args);
}


void Log::debug(const char* message, ...) {
    va_list args;
    va_start(args, message);
    __android_log_vprint(ANDROID_LOG_DEBUG, "NativeApp", message, args);
    __android_log_print(ANDROID_LOG_DEBUG, "NativeApp", "\n");
    va_end(args);
}

