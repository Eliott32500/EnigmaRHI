#pragma once
#include <format>
#include "logger.h"

#ifdef NDEBUG
	#define LOG_INFO(cat, msg)        ((void)0)
	#define LOG_WARNING(cat, msg)     ((void)0)
	#define LOG_INFOF(cat, ...)       ((void)0)
#else
	#define LOG_INFO(cat, msg)        Logger::Log(ELogLevel::INFO, cat, msg)
	#define LOG_WARNING(cat, msg)     Logger::Log(ELogLevel::WARNING, cat, msg)
	#define LOG_INFOF(cat, ...)       Logger::Log(ELogLevel::INFO, cat, std::format(__VA_ARGS__))
#endif

#define LOG_ERROR(cat, msg)           Logger::Log(ELogLevel::ERR, cat, msg)
#define LOG_ERRORF(cat, ...)          Logger::Log(ELogLevel::ERR, cat, std::format(__VA_ARGS__))
#define LOG_FATAL(cat, msg)           Logger::LogFatal(cat, msg)