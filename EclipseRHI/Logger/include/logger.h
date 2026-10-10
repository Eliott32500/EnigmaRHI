#pragma once
#include <string>
#include <iostream>

#define RESET    "\033[0m"
#define RED      "\033[31m"
#define GREEN    "\033[32m"
#define YELLOW   "\033[33m"

enum class LogLevel
{
    Info,
    Warning,
    Error,
    Fatal
};

enum class LogCategory
{
    Core,
    RHI,
    Vulkan,
    DX12,
    Shader,
    Memory
};

class Logger
{
public:
    static void Log(
        LogLevel level, 
        LogCategory category, 
        std::string message
    );
};