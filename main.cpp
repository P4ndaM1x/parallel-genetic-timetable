/**
 * @file main.cpp
 * @brief Main file of the project.
 */

#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "MPIManager.hpp"

int main(int argc, char* argv[])
{
    MPIManager mpi{&argc, &argv};
    CLI::Args::prepare(argc, argv);

    srand(time(NULL) + mpi.getRank());

    const int POPULATION_SAMPLE_SIZE = CLI::Args::populationSize / (mpi.getSize() - 1);

    if (mpi.isMaster()) {
        Timetable timetable;
        const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";
        FileManager::loadClasses(testFilePath, timetable);
        mpi.setMessage(timetable.serialize());
    }
    // Broadcast timetable to worker nodes
    mpi.broadcastMessage();

    GeneticAlgorithm geneticAlgorithm(
        Timetable::deserialize(mpi.getMessage()),
        CLI::Args::populationSize,
        CLI::Args::numberOfGenerations,
        CLI::Args::mutationRate
    );

    for (unsigned step = 1; step <= MPIManager::STEPS; ++step) {
        if (step != 1) {
            GeneticAlgorithm::ChromosomeContainer updatedPopulation
                = GeneticAlgorithm::deserializePopulation(mpi.getMessage());
            geneticAlgorithm.setPopulation(updatedPopulation);
            GeneticAlgorithm::ChromosomeContainer bestPop
                = geneticAlgorithm.getBestPopulationOfSize(10);
            if (mpi.isMaster()) {
                std::cout << "Step " << step << "/" << MPIManager::STEPS << "\n";
                for (unsigned i = 0; i < bestPop.size(); i++) {
                    std::cout << "Chromosome[" << i << "] error: " << bestPop[i].getError() << "\n";
                }
            }
            if (bestPop.at(0).getError() <= 0) {
                break;
            }
            if (step == MPIManager::STEPS) {
                break;
            }
        }

        if (not mpi.isMaster()) {

            geneticAlgorithm.step();

            // Send best population sample to master node
            mpi.setMessage(geneticAlgorithm.serializePopulationOfSize(POPULATION_SAMPLE_SIZE));
            mpi.sendMessageToMaster();
        }

        if (mpi.isMaster()) {
            GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
            for (int worker = 1; worker < mpi.getSize(); ++worker) {

                // Gather best population samples from all nodes
                mpi.recieveMessageFromWorker();

                GeneticAlgorithm::ChromosomeContainer subPopulation
                    = GeneticAlgorithm::deserializePopulation(mpi.getMessage());
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(), subPopulation.begin(), subPopulation.end()
                );
            }
            mpi.setMessage(GeneticAlgorithm::serializePopulation(gatheredChromosomes));
        }

        // Broadcast gathered chromosomes to all nodes
        mpi.broadcastMessage();
    }

    if (mpi.isMaster()) {
        geneticAlgorithm.getPopulation().at(0).printSolution();
        Log::print("All done!\n");
    }

    return 0;
}
