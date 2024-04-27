#pragma once

#include "Log.hpp"

#include <ext/CLI11.hpp>
#include <filesystem>

namespace CLI {

class Args {
public:
    static int prepare(int argc, char* argv[])
    {
        argv = app.ensure_utf8(argv);

        app.add_option("-d,--dir", sampleDataDirPath, "Path to directory with sample data")->required();
        app.add_option("-l,--logLevel", logLevel, "Minimal severity level of log messages");
        app.set_config("--config");

        customParse(argc, argv);
        return 0;
    }

    inline static std::filesystem::path sampleDataDirPath;
    inline static Severity logLevel { Log::defaultSeverity };

private:
    static void customParse(int argc, char* argv[])
    {
        try {
            app.parse(argc, argv);
        } catch (const CLI ::ParseError& e) {
            std::exit(app.exit(e));
        }
    }

    inline static CLI::App app { "Genetic Timetable Scheduling Algorithm" };
};

}
