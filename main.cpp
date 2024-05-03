#include "CLIManager.hpp"
#include "FileManager.hpp"
#include "GeneticAlgorithm.hpp"
#include "mpi.h"

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);
    CLI::Args::prepare(argc, argv);

    constexpr int MASTER = 0;
    constexpr int STEPS = 3;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int msg_size = 0;
    std::string message;
    Timetable timetable;

    if (rank == 0) {

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

    const int LOCAL_POP_BEST_SIZE = CLI::Args::populationSize / (size - 1);
    if (rank != 0) {
        timetable = Timetable::deserialize(message);
        GeneticAlgorithm geneticAlgorithm(
            timetable,
            CLI::Args::populationSize,
            CLI::Args::numberOfGenerations,
            CLI::Args::mutationRate
        );

        geneticAlgorithm.run();
        GeneticAlgorithm::ChromosomeContainer localPop = geneticAlgorithm.getPopulation();
        GeneticAlgorithm::ChromosomeContainer localPopBest(
            localPop.begin(), localPop.begin() + LOCAL_POP_BEST_SIZE
        );

        std::stringstream ss;
        for (const auto& chromosome : localPopBest) {
            ss << chromosome.serialize() << "\n";
        }
        message = ss.str();
        msg_size = message.size();

        MPI_Send(&msg_size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Send(&message[0], msg_size, MPI_CHAR, 0, 0, MPI_COMM_WORLD);
    }
    if (rank == 0) {
        for (int i = 1; i < size; ++i) {

            MPI_Recv(&msg_size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            char received_msg[msg_size + 1];
            MPI_Recv(received_msg, msg_size, MPI_CHAR, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            received_msg[msg_size] = '\n';
            std::string received_msg_string(received_msg);

            std::istringstream iss(received_msg_string);
            std::string line;

            while (std::getline(iss, line)) {
                if (line.empty()) {
                    std::cout << "A";
                }
            }
        }
    }

    Log::print("All done!\n");

    MPI_Finalize();
    return 0;
}
