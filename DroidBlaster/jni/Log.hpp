/*
 * Log.hpp
 *
 *  Created on: 2012-7-10
 *      Author: mcxiaoke
 */

#ifndef LOG_HPP_
#define LOG_HPP_

namespace demo {
class Log {
public:
    static void error(const char* message, ...);
    static void warn(const char* message, ...);
    static void info(const char* message, ...);
    static void debug(const char* message, ...);
};
}

#ifndef NDEBUG
#define demo_Log_debug(...) demo::Log::debug(__VA_ARGS__)
#else
#define demo_Log_debug(...)
#endif

#endif /* LOG_HPP_ */
