/**
 * @file MPINode.hpp
 * @brief MPINode class definition
 */
#pragma once

#include "mpi.h"
#include <string>

class MPINode {
public:
    static constexpr int MASTER = 0;
    static bool isCalledFromMaster()
    {
        int localRank;
        MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
        return localRank == MASTER;
    }

    MPINode(int* argc, char*** argv)
    {
        MPI_Init(argc, argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
    }
    ~MPINode() { MPI_Finalize(); }

    MPINode(const MPINode&) = delete;
    MPINode& operator=(const MPINode&) = delete;
    MPINode(MPINode&&) = delete;
    MPINode& operator=(MPINode&&) = delete;

    int getRank() const { return rank; }
    int getSize() const { return size; }
    bool isMaster() const { return rank == MASTER; }

    void setMessage(const std::string_view& data)
    {
        message.content = data;
        message.size = message.content.size();
    }
    std::string getMessage() const { return message.content; }

    void broadcastMessage()
    {
        MPI_Bcast(&message.size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        message.content.resize(message.size);
        MPI_Bcast(message.content.data(), message.size, MPI_CHAR, MASTER, MPI_COMM_WORLD);
    }

    void sendMessageToMaster()
    {
        if (isMaster())
            return;

        message.size = message.content.size();
        MPI_Send(&message.size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
        MPI_Send(message.content.data(), message.size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
    }

    void receiveMessageFromWorker(const int workerRank)
    {
        if (not isMaster())
            return;

        MPI_Recv(&message.size, 1, MPI_INT, workerRank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        message.content.resize(message.size);
        MPI_Recv(
            message.content.data(),
            message.size,
            MPI_CHAR,
            workerRank,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
    }

private:
    struct Message {
        std::string content{};
        int size{-1};
    } message;

    int rank{-1};
    int size{-1};
};
