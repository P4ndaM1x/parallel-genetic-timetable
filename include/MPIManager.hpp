/**
 * @file MPIManager.hpp
 * @brief MPIManager class definition
 */
#pragma once

#include "mpi.h"
#include <string>

class MPIManager {
public:
    static constexpr int MASTER = 0;
    static constexpr int STEPS = 10;

    MPIManager(int* argc, char*** argv)
    {
        MPI_Init(argc, argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
    }
    ~MPIManager() { MPI_Finalize(); }

    MPIManager(const MPIManager&) = delete;
    MPIManager& operator=(const MPIManager&) = delete;
    MPIManager(MPIManager&&) = delete;
    MPIManager& operator=(MPIManager&&) = delete;

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

    void receiveMessageFromWorker()
    {
        if (not isMaster())
            return;

        MPI_Recv(&message.size, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        message.content.resize(message.size);
        MPI_Recv(
            message.content.data(),
            message.size,
            MPI_CHAR,
            MPI_ANY_SOURCE,
            0,
            MPI_COMM_WORLD,
            MPI_STATUS_IGNORE
        );
    }

    static void
    send(const void* buf, int count, MPI_Datatype datatype, int dest, int tag, MPI_Comm comm)
    {
        MPI_Send(buf, count, datatype, dest, tag, comm);
    }

    static void recv(
        void* buf,
        int count,
        MPI_Datatype datatype,
        int source,
        int tag,
        MPI_Comm comm,
        MPI_Status* status
    )
    {
        MPI_Recv(buf, count, datatype, source, tag, comm, status);
    }

    static void bcast(void* buf, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
    {
        MPI_Bcast(buf, count, datatype, root, comm);
    }

private:
    struct Message {
        std::string content{};
        int size{-1};
    } message;

    int rank{-1};
    int size{-1};
};
