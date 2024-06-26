/**
 * @file main.cpp
 * @brief Main file of the project.
 */

#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "Timetable.hpp"

#ifndef UPCXX
#include "MPINode.hpp"
#else
#include "UPCNode.hpp"
#endif

int main(int argc, char* argv[])
{
#ifdef UPCXX
    UPCNode node{};
    Log::print("Executing with UPC++ as a backend");
#else
    MPINode node{&argc, &argv};
    Log::print("Executing with MPI as a backend");
#endif

    CLI::Args::prepare(argc, argv, node);
    srand(time(NULL) + node.getRank());

    const auto numberOfWorkers = node.getSize() - 1; // Master node is not a worker
    const auto populationSizePerWorker = CLI::Args::populationSize / numberOfWorkers
        + (CLI::Args::populationSize % numberOfWorkers != 0); // ceil

    if (node.isMaster()) {
        Timetable localTimetable;
        FileManager::loadClasses(CLI::Args::dataFilePath, localTimetable);
        node.setMessage(Timetable::serializeClasses(localTimetable.getClasses()));
    }
    // Broadcast timetable to worker nodes
    node.broadcastMessageFromMaster();

    GeneticAlgorithm geneticAlgorithm{
        Timetable::deserializeClasses(node.getMessage()),
        CLI::Args::populationSize,
        CLI::Args::numberOfGenerations,
        CLI::Args::mutationRate
    };

    const auto gatheringSteps = CLI::Args::numberOfGenerations / CLI::Args::gatheringFrequency;
    for (unsigned step = 0; step <= gatheringSteps; ++step) {
        if (step != 0) {
            constexpr unsigned maxChromosomesToPrint = 3u;

            GeneticAlgorithm::ChromosomeContainer updatedPopulation
                = GeneticAlgorithm::deserializePopulation(node.getMessage());
            geneticAlgorithm.setPopulation(updatedPopulation);
            GeneticAlgorithm::ChromosomeContainer bestPopulation
                = geneticAlgorithm.getBestPopulationOfSize(maxChromosomesToPrint);

            if (node.isMaster()) {
                Log::print(
                    "Best chromosomes after gathering step " + std::to_string(step) + '/'
                    + std::to_string(gatheringSteps) + ":"
                );
                for (unsigned i = 0; i < bestPopulation.size(); i++) {
                    Log::print(
                        std::to_string(i + 1u)
                        + ". best chromosome error: " + std::to_string(bestPopulation[i].getError())
                    );
                }
            }
            if (bestPopulation.at(0).getError() <= 0)
                break;
            if (step == gatheringSteps)
                break;
        }

        if (not node.isMaster()) {
            geneticAlgorithm.run();
            node.setMessage(geneticAlgorithm.serializePopulationOfSize(populationSizePerWorker));
#ifndef UPCXX
            // Send best population sample to master node
            node.sendMessageToMaster();
#endif
        }

#ifdef UPCXX
        GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
        for (int workerRank = 1; workerRank < node.getSize(); ++workerRank) {

            if (node.getRank() == workerRank) {
                node.sendMessageToMaster();
            }
            upcxx::barrier();

            if (node.isMaster()) {
                // node.receiveMessageFromWorker(workerRank);
                GeneticAlgorithm::ChromosomeContainer subPopulation
                    = GeneticAlgorithm::deserializePopulation(node.getMessage());
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(), subPopulation.begin(), subPopulation.end()
                );
            }
            upcxx::barrier();
        }
        if (node.isMaster()) {
            node.setMessage(GeneticAlgorithm::serializePopulation(gatheredChromosomes));
        }
#else
        if (node.isMaster()) {
            GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
            for (int workerRank = 1; workerRank < node.getSize(); ++workerRank) {

                // Gather best population samples from all nodes
                node.receiveMessageFromWorker(workerRank);

                GeneticAlgorithm::ChromosomeContainer subPopulation
                    = GeneticAlgorithm::deserializePopulation(node.getMessage());
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(), subPopulation.begin(), subPopulation.end()
                );
            }
            node.setMessage(GeneticAlgorithm::serializePopulation(gatheredChromosomes));
        }
#endif

        // Broadcast gathered chromosomes to all nodes
        node.broadcastMessageFromMaster();
    }

    if (node.isMaster()) {
        geneticAlgorithm.getPopulation().at(0).printSolution();
    }
    Log::print("All done!\n");

    return 0;
}
