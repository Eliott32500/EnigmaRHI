#include "logger.h"

void Logger::Log(LogLevel level, LogCategory category, std::string message)
{
    std::string str = "";

    switch (level)
    {
    case LogLevel::Info:
        str += "[INFO] ";
        break;
    case LogLevel::Warning:
        str += YELLOW;
        str += "[WARNING] ";
        break;
    case LogLevel::Error:
        str += RED;
        str += "[ERROR] ";
        break;
    case LogLevel::Fatal:
        str += RED;
        str += "[FATAL] ";
        break;
	}

    switch (category)
    {
    case LogCategory::Core:
        str += "[CORE] ";
        break;
    case LogCategory::RHI:
        str += "[RHI] ";
        break;
    case LogCategory::Vulkan:
        str += "[VULKAN] ";
        break;
    case LogCategory::Shader:
        str += "[SHADER] ";
        break;
    case LogCategory::Memory:
        str += "[MEMORY] ";
        break;
    }

    str += RESET;
	str += message;

    if(level == LogLevel::Fatal)
    {
        throw std::runtime_error(str);
        std::cout << std::endl;
	}

    std::cout << str << std::endl << std::endl;
}