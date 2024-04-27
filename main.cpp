#include "CLIManger.hpp"
#include "FileManager.hpp"
#include "Chromosome.hpp"
#include "GeneticAlgorithm.hpp"

int main(int argc, char* argv[])
{
    CLI::Args::prepare(argc, argv);
    srand(time(NULL));
    const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";
    Timetable timetable;
    FileManager::loadClasses(testFilePath, timetable);

    GeneticAlgorithm geneticAlgorithm(timetable, 1, 1, 0.01);

    std::vector<Chromosome> pop = geneticAlgorithm.getPopulation();
    pop[0].printSolution();

    return 0;
}
