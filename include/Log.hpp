#pragma once

#include <source_location>
#include <string>

enum class Severity : unsigned short {
    DEBUG = 0,
    INFO,
    WARNING,
    ERROR,
    FATAL,
};

class Log {
public:
    static const char* toString(const Severity severity);
    static void print(
        const std::string& message = "",
        const Severity severity = defaultSeverity,
        const bool carriageReturn = false,
        const std::string& functionName
        = getSimplifiedFunctionPrototype(std::source_location::current().function_name())
    );

    static constexpr Severity lowestSeverity = Severity::DEBUG;
    static constexpr Severity defaultSeverity = Severity::INFO;
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
