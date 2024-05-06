#pragma once

#include "Log.hpp"

#include <ext/CLI11.hpp>
#include <filesystem>
#include <limits>
#include <string>
#include <utility>

namespace CLI {

class Args {
public:
    static int prepare(int argc, char* argv[])
    {
        argv = app.ensure_utf8(argv);

        app.option_defaults()->always_capture_default();

        app.add_option("-l,--logLevel", logLevel, prepareLogLevelDescription())
            ->check(Range{Log::lowestSeverity, Log::highestSeverity})
            ->capture_default_str();
        app.add_option("-d,--dir", sampleDataDirPath, "Path to directory with sample data")
            ->check(detail::ExistingDirectoryValidator{})
            ->required();
        app.add_option("-p,--population", populationSize, "Size of the population")
            ->check(Range{1u, std::numeric_limits<unsigned>::max()});
        app.add_option("-g,--generations", numberOfGenerations, "Number of generations")
            ->check(Range{1u, std::numeric_limits<unsigned>::max()});
        app.add_option("-m,--mutation", mutationRate, "Mutation rate")->check(Range{0.f, 1.f});
        app.set_config("--config");

        customParse(argc, argv);
        return 0;
    }

    inline static std::filesystem::path sampleDataDirPath;
    inline static Severity logLevel{Log::defaultSeverity};
    inline static unsigned populationSize{200};
    inline static unsigned numberOfGenerations{200};
    inline static double mutationRate{0.01};

private:
    static std::string prepareLogLevelDescription()
    {
        const auto begin = std::to_underlying(Log::lowestSeverity);
        const auto end = std::to_underlying(Log::highestSeverity);

        std::stringstream severityDescription;
        severityDescription << "Minimal severity level of log messages:" << std::endl;
        for (unsigned i = begin; i <= end; ++i) {
            severityDescription << i << "->" << Log::toString(static_cast<Severity>(i));
            if (i != end)
                severityDescription << ", ";
        }
        return severityDescription.str();
    }

    static void customParse(int argc, char* argv[])
    {
        try {
            app.parse(argc, argv);
        } catch (const CLI ::ParseError& e) {
            std::exit(app.exit(e));
        }
    }

    inline static CLI::App app{"Genetic Timetable Scheduling Algorithm"};
};

}
