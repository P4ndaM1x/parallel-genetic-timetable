/**
 * @file Log.cpp
 * @brief Implementation file of the Log class.
 */
#include "Log.hpp"

#include "CLIManager.hpp"

#include <utility>

const char* Log::toString(const Severity severity)
{
    switch (severity) {
    case Severity::DEBUG:
        return "DBG";
    case Severity::INFO:
        return "INF";
    case Severity::WARNING:
        return "WRN";
    case Severity::ERROR:
        return "ERR";
    case Severity::FATAL:
        return "FTL";
    default:
        return "???";
    }
}

void Log::print(
    const std::string& message,
    const Severity severity,
    const bool carriageReturn,
    const std::string& functionName
)
{
    if (std::to_underlying(severity) < std::to_underlying(CLI::Args::logLevel)) {
        return;
    }

    if (carriageReturn) {
        std::cout << "\r";
    } else {
        std::cout << std::endl;
    }
    std::cout << toString(severity) << " | " << functionName << " | " << message;
}