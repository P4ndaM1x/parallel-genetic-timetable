/**
 * @file MPINode.cpp
 * @brief Implementation file of the MPINode class.
 */
#include "MPINode.hpp"

MPINode::MPINode(int* argc, char*** argv)
{
    MPI_Init(argc, argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
}

MPINode::~MPINode() { MPI_Finalize(); }

void MPINode::setMessage(const std::string_view& data)
{
    message.content = data;
    message.size = message.content.size();
}

void MPINode::broadcastMessageFromMaster()
{
    MPI_Bcast(&message.size, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    message.content.resize(message.size);
    MPI_Bcast(message.content.data(), message.size, MPI_CHAR, MASTER, MPI_COMM_WORLD);
}

void MPINode::sendMessageToMaster()
{
    if (isMaster())
        return;

    message.size = message.content.size();
    MPI_Send(&message.size, 1, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
    MPI_Send(message.content.data(), message.size, MPI_CHAR, MASTER, 0, MPI_COMM_WORLD);
}

void MPINode::receiveMessageFromWorker(const int workerRank)
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