/**
 * @file main.cpp
 * @brief Main file of the project.
 */

#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "MPINode.hpp"

int main(int argc, char* argv[])
{
    MPINode node{&argc, &argv};
    CLI::Args::prepare(argc, argv, node);

    srand(time(NULL) + node.getRank());

    const auto numberOfWorkers = node.getSize() - 1; // Master node is not a worker
    const auto populationSizePerWorker = CLI::Args::populationSize / numberOfWorkers
        + (CLI::Args::populationSize % numberOfWorkers != 0); // ceil

    if (node.isMaster()) {
        Timetable timetable;
        const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";
        FileManager::loadClasses(testFilePath, timetable);
        node.setMessage(timetable.serialize());
    }
    // Broadcast timetable to worker nodes
    node.broadcastMessage();

    GeneticAlgorithm geneticAlgorithm(
        Timetable::deserialize(node.getMessage()),
        CLI::Args::populationSize,
        CLI::Args::numberOfGenerations,
        CLI::Args::mutationRate
    );

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
            geneticAlgorithm.step();
            // Send best population sample to master node
            node.setMessage(geneticAlgorithm.serializePopulationOfSize(populationSizePerWorker));
            node.sendMessageToMaster();
        }

        if (node.isMaster()) {
            GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
            for (int worker = 1; worker < node.getSize(); ++worker) {

                // Gather best population samples from all nodes
                node.receiveMessageFromWorker();

                GeneticAlgorithm::ChromosomeContainer subPopulation
                    = GeneticAlgorithm::deserializePopulation(node.getMessage());
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(), subPopulation.begin(), subPopulation.end()
                );
            }
            node.setMessage(GeneticAlgorithm::serializePopulation(gatheredChromosomes));
        }

        // Broadcast gathered chromosomes to all nodes
        node.broadcastMessage();
    }

    if (node.isMaster()) {
        geneticAlgorithm.getPopulation().at(0).printSolution();
        Log::print("All done!\n");
    }

    return 0;
}
