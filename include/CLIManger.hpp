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
        app.set_config("--config");

        CLI11_PARSE(app, argc, argv);
        return 0;
    }

    inline static std::filesystem::path sampleDataDirPath;

private:
    inline static CLI::App app { "Genetic Timetable Scheduling Algorithm" };
};

}
