#include <logger.h>

#define LOG_INFO(cat, msg) \
    Logger::Log(LogLevel::Info, cat, msg)

#define LOG_WARNING(cat, msg) \
    Logger::Log(LogLevel::Warning, cat, msg)

#define LOG_ERROR(cat, msg) \
    Logger::Log(LogLevel::Error, cat, msg)

#define LOG_FATAL(cat, msg) \
    Logger::Log(LogLevel::Fatal, cat, msg)