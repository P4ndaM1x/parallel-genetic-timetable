#pragma once

#include <ext/CLI11.hpp>
#include <filesystem>

namespace CLI {

class Args {
public:
    static int prepare(int argc, char* argv[])
    {
        argv = app.ensure_utf8(argv);

        app.add_option("-d,--dir", sampleDataDirPath, "Path to directory with sample data")->required();
        app.add_option("-v,--verbose", verbose, "Print verbose output");
        app.set_config("--config");

        customParse(argc, argv);
        return 0;
    }

    inline static std::filesystem::path sampleDataDirPath;
    inline static bool verbose { false };

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
