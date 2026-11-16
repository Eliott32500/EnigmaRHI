#pragma once
#include <string>
#include <iostream>
#include <source_location>

#define RESET    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"

enum class ELogLevel
{
    INFO,
    WARNING,
    ERR,
    FATAL
};

enum class ELogCategory
{
    CORE,
    RHI,
    VULKAN,
    DX12,
    SHADER,
    MEMORY
};

namespace Logger
{
    void Log(ELogLevel level, ELogCategory category, std::string message, std::source_location loc = std::source_location::current());
    [[noreturn]] void LogFatal(ELogCategory category, std::string message,std::source_location loc = std::source_location::current());
}