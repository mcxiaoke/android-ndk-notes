#ifndef _LOG_H_
#define _LOG_H_


class Log {
public:
    static void error(const char* message, ...);
    static void warn(const char* message, ...);
    static void info(const char* message, ...);
    static void debug(const char* message, ...);
};

#ifndef NDEBUG
    #define log(...) Log::debug(__VA_ARGS__)
#else
    #define log(...)
#endif


#endif
