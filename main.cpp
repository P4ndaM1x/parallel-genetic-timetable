#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"

int main(int argc, char* argv[])
{
    CLI::Args::prepare(argc, argv);
    std::srand(std::time(0));
    const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";
    Timetable timetable;

    Log::print("Loading classes...");
    FileManager::loadClasses(testFilePath, timetable);

    Log::print("Instantiation of the genetic algorithm...");
    GeneticAlgorithm geneticAlgorithm{
        timetable,
        CLI::Args::populationSize,
        CLI::Args::numberOfGenerations,
        CLI::Args::mutationRate
    };

    GeneticAlgorithm::ChromosomeContainer pop = geneticAlgorithm.run();

    Log::print("Printing solution...");
    timetable.print();

    Log::print("All done!\n");
    return 0;
}
