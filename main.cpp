#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "mpi.h"

int main(int argc, char* argv[])
{
    constexpr int MASTER = 0;
    constexpr int STEPS = 10;
    int rank, size;
    int mpi_msg_size;
    std::string mpi_msg;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    CLI::Args::prepare(argc, argv);

    srand(time(NULL) + rank);

    const int POPULATION_SAMPLE_SIZE = CLI::Args::populationSize / (size - 1);

    if (rank == 0) {

        const auto testFilePath = CLI::Args::sampleDataDirPath / "test.csv";

        Timetable timetable;
        FileManager::loadClasses(testFilePath, timetable);

        mpi_msg = timetable.serialize();
        mpi_msg_size = mpi_msg.size();
    }

    // Broadcast timetable to worker nodes
    MPI_Bcast(&mpi_msg_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    mpi_msg.resize(mpi_msg_size);
    MPI_Bcast(&mpi_msg[0], mpi_msg_size, MPI_CHAR, MASTER, MPI_COMM_WORLD);

    GeneticAlgorithm geneticAlgorithm(
        Timetable::deserialize(mpi_msg),
        CLI::Args::populationSize,
        CLI::Args::numberOfGenerations,
        CLI::Args::mutationRate
    );

    for (unsigned step = 0; step <= STEPS; step++) {
        if (step != 0) {
            GeneticAlgorithm::ChromosomeContainer updatedPopulation
                = GeneticAlgorithm::deserializePopulation(mpi_msg);
            geneticAlgorithm.setPopulation(updatedPopulation);
            GeneticAlgorithm::ChromosomeContainer bestPop
                = geneticAlgorithm.getBestPopulationOfSize(10);
            if (rank == MASTER) {
                std::cout << "Step " << step << "/" << STEPS << "\n";
                for (unsigned i = 0; i < bestPop.size(); i++) {
                    std::cout << "Chromosome[" << i << "] error: " << bestPop[i].getError() << "\n";
                }
            }
            if (bestPop.at(0).getError() <= 0) {
                break;
            }
            if (step == STEPS) {
                break;
            }
        }

        if (rank != MASTER) {

            geneticAlgorithm.step();

            // Send best population sample to master node
            mpi_msg = geneticAlgorithm.serializePopulationOfSize(POPULATION_SAMPLE_SIZE);
            mpi_msg_size = mpi_msg.size();
            MPI_Send(&mpi_msg_size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
            MPI_Send(&mpi_msg[0], mpi_msg_size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
        }

        if (rank == MASTER) {
            GeneticAlgorithm::ChromosomeContainer gatheredChromosomes;
            for (int worker = 1; worker < size; ++worker) {

                // Gather best population samples from all nodes
                MPI_Recv(&mpi_msg_size, 1, MPI_INT, worker, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                mpi_msg.resize(mpi_msg_size);
                MPI_Recv(
                    &mpi_msg[0],
                    mpi_msg_size,
                    MPI_CHAR,
                    worker,
                    0,
                    MPI_COMM_WORLD,
                    MPI_STATUS_IGNORE
                );

                GeneticAlgorithm::ChromosomeContainer subPopulation
                    = GeneticAlgorithm::deserializePopulation(mpi_msg);
                gatheredChromosomes.insert(
                    gatheredChromosomes.end(), subPopulation.begin(), subPopulation.end()
                );
            }

            mpi_msg = GeneticAlgorithm::serializePopulation(gatheredChromosomes);
            mpi_msg_size = mpi_msg.size();
        }

        // Broadcast gathered chromosomes to all nodes
        MPI_Bcast(&mpi_msg_size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        mpi_msg.resize(mpi_msg_size);
        MPI_Bcast(&mpi_msg[0], mpi_msg_size, MPI_CHAR, MASTER, MPI_COMM_WORLD);
    }

    if (rank == MASTER) {
        geneticAlgorithm.getPopulation().at(0).printSolution();
    }
    Log::print("All done!\n");

    MPI_Finalize();
    return 0;
}
