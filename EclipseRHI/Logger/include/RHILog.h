#include <logger.h>
#include <format>

#ifdef NDEBUG

#define LOG_INFO(cat, msg)    ((void)0)
#define LOG_WARNING(cat, msg) ((void)0)
#define LOG_ERROR(cat, msg)   ((void)0)
#define LOG_FATAL(cat, msg)   ((void)0)
#define LOG_ERRORF(cat, fmt, ...) ((void)0)
#define LOG_INFOF(cat, fmt, ...) ((void)0)

#else

#define LOG_INFO(cat, msg) \
    Logger::Log(LogLevel::Info, cat, msg)

#define LOG_WARNING(cat, msg) \
    Logger::Log(LogLevel::Warning, cat, msg)

#define LOG_ERROR(cat, msg) \
    Logger::Log(LogLevel::Error, cat, msg)

#define LOG_FATAL(cat, msg) \
    Logger::Log(LogLevel::Fatal, cat, msg)

#define LOG_INFOF(cat, fmt, ...) \
    Logger::Log(LogLevel::Info, cat, std::format(fmt, __VA_ARGS__))

#define LOG_ERRORF(cat, fmt, ...) \
    Logger::Log(LogLevel::Error, cat, std::format(fmt, __VA_ARGS__))

#endif