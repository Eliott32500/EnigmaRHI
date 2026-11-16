#include "logger.h"
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <format>

namespace
{
    std::string CurrentTime()
    {
        using namespace std::chrono;

        const auto now = system_clock::now();
        const auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
        const std::time_t t = system_clock::to_time_t(now);

        std::tm tm{};
        localtime_s(&tm, &t);

        return std::format("{:02}:{:02}:{:02}.{:03}", tm.tm_hour, tm.tm_min, tm.tm_sec, ms.count());
    }

    std::string FileName(const char* path)
    {
        std::string s = path;
        const size_t pos = s.find_last_of("/\\");
        return pos == std::string::npos ? s : s.substr(pos + 1);
    }
}

void Logger::Log(ELogLevel level, ELogCategory category, std::string message, std::source_location loc)
{
    std::string str = "[" + CurrentTime() + "] ";

    switch (level)
    {
    case ELogLevel::INFO:       str += "[INFO] ";                   break;
    case ELogLevel::WARNING:    str += YELLOW; str += "[WARNING] "; break;
    case ELogLevel::ERR:        str += RED;    str += "[ERROR] ";   break;
    case ELogLevel::FATAL:      str += RED;    str += "[FATAL] ";   break;
    }

    switch (category)
    {
    case ELogCategory::CORE:   str += "[CORE] ";   break;
    case ELogCategory::RHI:    str += "[RHI] ";    break;
    case ELogCategory::VULKAN: str += "[VULKAN] "; break;
    case ELogCategory::DX12:   str += "[DX12] ";   break;
    case ELogCategory::SHADER: str += "[SHADER] "; break;
    case ELogCategory::MEMORY: str += "[MEMORY] "; break;
    }

    str += RESET;
    str += std::format("{}:{} | ", FileName(loc.file_name()), loc.line());
    str += message;

    std::cout << str << std::endl << std::endl;
}

void Logger::LogFatal(ELogCategory category, std::string message, std::source_location loc)
{
    Log(ELogLevel::FATAL, category, std::move(message), loc);
    std::abort();
}
