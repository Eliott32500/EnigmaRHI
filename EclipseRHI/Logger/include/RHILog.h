#include <logger.h>

#ifdef NDEBUG

#define LOG_INFO(cat, msg)    ((void)0)
#define LOG_WARNING(cat, msg) ((void)0)
#define LOG_ERROR(cat, msg)   ((void)0)
#define LOG_FATAL(cat, msg)   ((void)0)

#else

#define LOG_INFO(cat, msg) \
    Logger::Log(LogLevel::Info, cat, msg)

#define LOG_WARNING(cat, msg) \
    Logger::Log(LogLevel::Warning, cat, msg)

#define LOG_ERROR(cat, msg) \
    Logger::Log(LogLevel::Error, cat, msg)

#define LOG_FATAL(cat, msg) \
    Logger::Log(LogLevel::Fatal, cat, msg)

#endif