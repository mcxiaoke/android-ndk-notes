/*
 * Log.cpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#include "Log.hpp"
#include <stdarg.h>
#include <android/log.h>

namespace demo {
void Log::info(const char *message, ...) {
    va_list varArgs;
    va_start(varArgs, message);
    __android_log_vprint(ANDROID_LOG_INFO, "DEMO", message, varArgs);
    __android_log_print(ANDROID_LOG_INFO, "DEMO", "\n");
    va_end(varArgs);
}

void Log::debug(const char *message, ...) {
    va_list varArgs;
    va_start(varArgs, message);
    __android_log_vprint(ANDROID_LOG_DEBUG, "DEMO", message, varArgs);
    __android_log_print(ANDROID_LOG_DEBUG, "DEMO", "\n");
    va_end(varArgs);
}

void Log::warn(const char *message, ...) {
    va_list varArgs;
    va_start(varArgs, message);
    __android_log_vprint(ANDROID_LOG_WARN, "DEMO", message, varArgs);
    __android_log_print(ANDROID_LOG_WARN, "DEMO", "\n");
    va_end(varArgs);
}

void Log::error(const char *message, ...) {
    va_list varArgs;
    va_start(varArgs, message);
    __android_log_vprint(ANDROID_LOG_ERROR, "DEMO", message, varArgs);
    __android_log_print(ANDROID_LOG_ERROR, "DEMO", "\n");
    va_end(varArgs);
}
}

