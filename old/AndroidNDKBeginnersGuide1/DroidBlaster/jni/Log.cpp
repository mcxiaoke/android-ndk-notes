#include "Log.hpp"

#include <stdarg.h>
#include <android/log.h>

namespace packt {
    void Log::info(const char* pMessage, ...) {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_INFO, "PACKT", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_INFO, "PACKT", "\n");
        va_end(lVarArgs);
    }

    void Log::error(const char* pMessage, ...) {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_ERROR, "PACKT", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_ERROR, "PACKT", "\n");
        va_end(lVarArgs);
    }

    void Log::warn(const char* pMessage, ...) {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_WARN, "PACKT", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_WARN, "PACKT", "\n");
        va_end(lVarArgs);
    }

    void Log::debug(const char* pMessage, ...) {
        va_list lVarArgs;
        va_start(lVarArgs, pMessage);
        __android_log_vprint(ANDROID_LOG_DEBUG, "PACKT", pMessage,
            lVarArgs);
        __android_log_print(ANDROID_LOG_DEBUG, "PACKT", "\n");
        va_end(lVarArgs);
    }
}
