/**
 * @file MPINode.hpp
 * @brief Defines the MPINode class.
 */
#pragma once

#include "Node.hpp"

#include "mpi.h"

/**
 * @brief Represents a node in a parallel program utilizing MPI.
 */
class MPINode : public Node {
public:
    /**
     * @brief Constructs an Node object, initializing MPI.
     * @param argc Pointer to the number of command-line arguments.
     * @param argv Pointer to the command-line argument values.
     */
    MPINode(int* argc, char*** argv)
    {
        MPI_Init(argc, argv);
        MPI_Comm_rank(MPI_COMM_WORLD, &rank);
        MPI_Comm_size(MPI_COMM_WORLD, &size);
    }

    /**
     * @brief Destroys the Node object, finalizing MPI.
     */
    ~MPINode() { MPI_Finalize(); }

    /**
     * @brief Broadcasts the message from the master node to all nodes using MPI communication.
     */
    void broadcastMessageFromMaster()
    {
        MPI_Bcast(&message.size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        message.content.resize(message.size);
        MPI_Bcast(message.content.data(), message.size, MPI_CHAR, MASTER, MPI_COMM_WORLD);
    }

    /**
     * @brief Sends the message from the current node to the master node using MPI communication.
     */
    void sendMessageToMaster()
    {
        if (isMaster())
            return;

        message.size = message.content.size();
        MPI_Send(&message.size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
        MPI_Send(message.content.data(), message.size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
    }

    /**
     * @brief Receives a message from a worker node (if the current node is the master node)
     * using MPI communication.
     * @param workerRank The rank of the worker node.
     */
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
};
