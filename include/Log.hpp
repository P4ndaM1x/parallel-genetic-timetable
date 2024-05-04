/**
 * @file Log.hpp
 * @brief This file contains the declaration of the Log class and the Severity enum class.
 * @see Log.cpp
 */
#pragma once

#include <source_location>
#include <string>

/**
 * @brief Represents the severity levels of log messages.
 */
enum class Severity : unsigned short {
    DEBUG = 0, /**< Debug level */
    INFO, /**< Information level */
    WARNING, /**< Warning level */
    ERROR, /**< Error level */
    FATAL, /**< Fatal level */
};

/**
 * @brief Provides logging functionality.
 */
class Log {
public:
    /**
     * @brief Converts the severity level to a string representation.
     * @param severity The severity level to convert.
     * @return The string representation of the severity level.
     */
    static const char* toString(const Severity severity);

    /**
     * @brief Prints a message to the standard output.
     *
     * Print format: "SEVERITY | FUNCTION_NAME | MESSAGE".
     * The message is printed only if the severity level is greater than or equal to the CLI::Args::logLevel.
     * 
     *
     * @param message The log message to print.
     * @param severity The severity level of the log message.
     * @param carriageReturn Whether to add a carriage return instead of a new line before printing the message.
     * @param functionName The simplified prototype of the function where the log message is printed.
     */
    static void print(
        const std::string& message = "",
        const Severity severity = defaultSeverity,
        const bool carriageReturn = false,
        const std::string& functionName
        = getSimplifiedFunctionPrototype(std::source_location::current().function_name())
    );

    /**
     * @brief The lowest possible severity level of log messages.
     */
    static constexpr Severity lowestSeverity = Severity::DEBUG;

    /**
     * @brief The default possible severity level of log messages.
     */
    static constexpr Severity defaultSeverity = Severity::INFO;

    /**
     * @brief The highest possible severity level of log messages.
     */
    static constexpr Severity highestSeverity = Severity::FATAL;

private:
    static constexpr std::string getSimplifiedFunctionPrototype(const std::string& prettyFunction)
    {
        const auto begin = prettyFunction.find(" ") + 1;
        const auto end = prettyFunction.find("(");
        if (prettyFunction[end + 1] == ')')
            return prettyFunction.substr(begin, end - begin) + "()";
        else
            return prettyFunction.substr(begin, end - begin) + "(...)";
    }
};
