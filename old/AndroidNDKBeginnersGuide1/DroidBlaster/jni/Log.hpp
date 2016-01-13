#ifndef _PACKT_LOG_HPP_
#define _PACKT_LOG_HPP_

namespace packt {
    class Log {
    public:
        static void error(const char* pMessage, ...);
        static void warn(const char* pMessage, ...);
        static void info(const char* pMessage, ...);
        static void debug(const char* pMessage, ...);
    };
}

#ifndef NDEBUG
    #define packt_Log_debug(...) packt::Log::debug(__VA_ARGS__)
#else
    #define packt_Log_debug(...)
#endif

#endif
