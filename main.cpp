#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "mpi.h"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    constexpr int MASTER = 0;
    constexpr int STEPS = 3;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    std::cout << "FFFFFFFFFFFFF" << rank;
    std::cout << "FFFFFFFFFFFFF" << size;
    int msg_size = 0;
    std::string message;
    Timetable timetable;

    // Broadcast timetable
    if (rank == 0) {
        CLI::Args::prepare(argc, argv);

        const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";

        FileManager::loadClasses(testFilePath, timetable);

        message = timetable.serialize();
    }

    if (rank == 0) {
        msg_size = message.size();
    }
    MPI_Bcast(&msg_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    if (rank != 0) {
        message.resize(msg_size);
    }
    MPI_Bcast(&message[0], msg_size, MPI_CHAR, MASTER, MPI_COMM_WORLD);

    const int POPULATION_SAMPLE_SIZE = CLI::Args::populationSize / (size - 1);
    if (rank != 0) {
        timetable = Timetable::deserialize(message);
        GeneticAlgorithm geneticAlgorithm(
            timetable,
            CLI::Args::populationSize,
            CLI::Args::numberOfGenerations,
            CLI::Args::mutationRate
        );

        for (unsigned step = 0; step < STEPS; step++) {
            geneticAlgorithm.step();
            message = geneticAlgorithm.serializePopulationOfSize(POPULATION_SAMPLE_SIZE);
            msg_size = message.size();

            MPI_Send(&msg_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
            MPI_Send(&message[0], msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);

            MPI_Barrier(MPI_COMM_WORLD);
            std::cout << "AAAAAAAAAAAA" << rank;

            message.resize(msg_size);
            GeneticAlgorithm::ChromosomeContainer updatedPopulation
                = GeneticAlgorithm::deserializePopulation(message);
            geneticAlgorithm.setPopulation(updatedPopulation);
        }
    }
    if (rank == MASTER) {
        GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
        for (unsigned step = 0; step < STEPS; step++) {
            for (int i = 1; i < size; ++i) {

                MPI_Recv(&msg_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                message.resize(msg_size);
                MPI_Recv(&message[0], msg_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                GeneticAlgorithm::ChromosomeContainer deserializedChromosomes
                    = GeneticAlgorithm::deserializePopulation(message);
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(),
                    deserializedChromosomes.begin(),
                    deserializedChromosomes.end()
                );
                std::cout << "DDDDDDDDDDDDDDD" << i;
            }
            std::cout << "AAAAAAAAAAAA" << rank;

            GeneticAlgorithm tmpAlgorithm(
                timetable, CLI::Args::populationSize, 0, CLI::Args::mutationRate
            );
            tmpAlgorithm.setPopulation(gatheredChromosomes);
            GeneticAlgorithm::ChromosomeContainer top = tmpAlgorithm.getTopPopulationOfSize(10);
            // std::cout << "Step: " << step << "\n";
            // std::cout << "Chrosome errors" << "\n";
            // for (const auto& chromosome : top) {
            //     std::cout << chromosome.getError() << "\n";
            // }

            std::cout << "AAAAAAAAAAAA" << rank;
            message = GeneticAlgorithm::serializePopulation(gatheredChromosomes);

            msg_size = message.size();
            std::cout << "AAAAAAAAAAAA" << rank;
            MPI_Bcast(&msg_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
            std::cout << "AAAAAAAAAAAA" << rank;
            MPI_Barrier(MPI_COMM_WORLD);
            std::cout << "AAAAAAAAAAAA" << rank;

            MPI_Bcast(&message[0], msg_size, MPI_CHAR, MASTER, MPI_COMM_WORLD);
            gatheredChromosomes.erase(gatheredChromosomes.begin(), gatheredChromosomes.end());
        }
    }

    Log::print("All done!\n");

    MPI_Finalize();
    return 0;
}
