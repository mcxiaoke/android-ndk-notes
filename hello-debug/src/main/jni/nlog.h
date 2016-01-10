#pragma once
#ifndef NLOG_H__
#define NLOG_H__
/**
 * Basic logging framework for NDK.
 */

#include <android/log.h>

/**

typedef enum android_LogPriority {
   ANDROID_LOG_UNKNOWN = 0,
   ANDROID_LOG_DEFAULT,
   ANDROID_LOG_VERBOSE,
   ANDROID_LOG_DEBUG,
   ANDROID_LOG_INFO,
   ANDROID_LOG_WARN,
   ANDROID_LOG_ERROR,
   ANDROID_LOG_FATAL,
   ANDROID_LOG_SILENT,
} android_LogPriority;

**/

#define NLOG_LEVEL_VERBOSE 1
#define NLOG_LEVEL_DEBUG 2
#define NLOG_LEVEL_INFO 3
#define NLOG_LEVEL_WARN 4
#define NLOG_LEVEL_ERROR 5
#define NLOG_LEVEL_FATAL 6
#define NLOG_LEVEL_SILENT 7

#ifndef NLOG_TAG
#define NLOG_TAG __FILE__
#endif

#ifndef NLOG_LEVEL
#define NLOG_LEVEL NLOG_LEVEL_VERBOSE
#endif

#define NLOG_NOOP (void) 0

#define LOG_PRINT(level,fmt,...) \
  __android_log_print(level, NLOG_TAG, "(%s:%u) %s: " fmt, \
  __FILE__, __LINE__, __PRETTY_FUNCTION__, ##__VA_ARGS__)

#if NLOG_LEVEL_VERBOSE >= NLOG_LEVEL
# define LOGV(fmt,...) \
  LOG_PRINT(ANDROID_LOG_VERBOSE, fmt, ##__VA_ARGS__)
#else
# define LOGV(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_DEBUG >= NLOG_LEVEL
# define LOGD(fmt,...) \
  LOG_PRINT(ANDROID_LOG_DEBUG, fmt, ##__VA_ARGS__)
#else
# define LOGD(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_INFO >= NLOG_LEVEL
# define LOGI(fmt,...) \
  LOG_PRINT(ANDROID_LOG_INFO, fmt, ##__VA_ARGS__)
#else
# define LOGI(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_WARN >= NLOG_LEVEL
# define LOGW(fmt,...) \
  LOG_PRINT(ANDROID_LOG_WARN, fmt, ##__VA_ARGS__)
#else
# define LOGW(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_ERROR >= NLOG_LEVEL
# define LOGE(fmt,...) \
  LOG_PRINT(ANDROID_LOG_ERROR, fmt, ##__VA_ARGS__)
#else
# define LOGE(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_FATAL >= NLOG_LEVEL
# define LOGF(fmt,...) \
  LOG_PRINT(ANDROID_LOG_FATAL, fmt, ##__VA_ARGS__)
#else
# define LOGF(...) NLOG_NOOP
#endif

#if NLOG_LEVEL_FATAL >= NLOG_LEVEL
# define ASSERT(expression, fmt, ...) \
  if (!(expression)) \
  { \
  __android_log_assert(#expression, NLOG_TAG, \
  fmt, ##__VA_ARGS__); \
  }
#else
# define ASSERT(...) NLOG_NOOP
#endif

#endif /* NLOG_H__ */
