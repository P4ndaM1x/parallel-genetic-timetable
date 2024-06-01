/**
 * @file CLIManager.hpp
 * @brief The file containing CLI::Args class.
 */
#pragma once

#include "Log.hpp"
#include "Node.hpp"

#include <ext/CLI11.hpp>
#include <filesystem>
#include <limits>
#include <string>
#include <utility>

/**
 * @brief CLI11 library extension.
 */
namespace CLI {

/**
 * @brief The command line arguments manager.
 */
class Args {
public:
    /**
     * @brief Prepares the command line arguments.
     * @param argc The number of command line arguments.
     * @param argv The array of command line arguments.
     * @param mpi The instance containing information if the process is master.
     * @return The exit code.
     */
    static int prepare(int argc, char* argv[], const Node& node)
    {
        argv = app.ensure_utf8(argv);

        app.option_defaults()->always_capture_default();

        app.add_option("--file", dataFilePath, "Path to CSV file with class data")
            ->check(detail::ExistingFileValidator{})
            ->required();
        app.add_option("--logLevel", logLevel, prepareLogLevelDescription())
            ->check(Range{Log::lowestSeverity, Log::highestSeverity});
        app.add_option(
            "--workerLogs", showWorkerLogs, "Show logs from all nodes, including workers"
        );
        app.add_option(
               "-p,--population",
               populationSize,
               "Size of the population at the start of each generation"
        )
            ->check(Range{1u, std::numeric_limits<unsigned>::max()});
        app.add_option("-g,--generations", numberOfGenerations, "Number of all generations")
            ->check(Range{1u, std::numeric_limits<unsigned>::max()});
        app.add_option(
               "-f,--frequency",
               gatheringFrequency,
               "Number of generations between gathering best chromosomes"
        )
            ->check(Range{1u, std::numeric_limits<unsigned>::max()});
        app.add_option("-m,--mutation", mutationRate, "Mutation rate")->check(Range{0.f, 1.f});
        app.set_config("--config");

        customParse(argc, argv, node);
        return 0;
    }

    /**
     * @brief The path to the CSV file with class data.
     */
    inline static std::filesystem::path dataFilePath;

    /**
     * @brief The severity level of log messages.
     */
    inline static Severity logLevel{Log::defaultSeverity};

    /**
     * @brief The flag indicating if worker logs should be shown.
     */
    inline static bool showWorkerLogs{false};

    /**
     * @brief The size of the population at the start of each generation.
     */
    inline static unsigned populationSize{100};

    /**
     * @brief The total number of generations.
     */
    inline static unsigned numberOfGenerations{100};

    /**
     * @brief The number of generations between gathering best chromosomes.
     */
    inline static unsigned gatheringFrequency{10};

    /**
     * @brief The mutation rate.
     */
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

    static void customParse(int argc, char* argv[], const Node& node)
    {
        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& e) {
            const auto printHelp = node.isMaster();
            node.~Node();
            if (printHelp)
                std::exit(app.exit(e));
            std::exit(-1);
        }
    }

    inline static CLI::App app{"Genetic Timetable Scheduling Algorithm"};
};

}
