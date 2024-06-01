/**
 * @file MPINode.cpp
 * @brief Implementation file of the MPINode class.
 */
#include "MPINode.hpp"

#define UPCXX
#ifndef UPCXX
#include "mpi.h"

bool MPINode::isCalledFromMaster()
{
    int localRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
    return localRank == MASTER;
}

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
#else
#include <upcxx/upcxx.hpp>

bool MPINode::isCalledFromMaster() { return false; }

MPINode::MPINode(int* argc, char*** argv)
{
    upcxx::init();
    rank = upcxx::rank_me();
    size = upcxx::rank_n();
}

MPINode::~MPINode() { upcxx::finalize(); }

void MPINode::setMessage(const std::string_view& data)
{
    message.content = data;
    message.size = message.content.size();
}

void MPINode::broadcastMessageFromMaster()
{
    upcxx::global_ptr<int> messageSizePtr = nullptr;
    upcxx::global_ptr<char> messageContentPtr = nullptr;
    if (isMaster()) {
        messageSizePtr = upcxx::new_<int>(message.size);
        messageContentPtr = upcxx::new_array<char>(message.size);
        std::strcpy(messageContentPtr.local(), message.content.c_str());
    }

    messageSizePtr = upcxx::broadcast(messageSizePtr, MASTER).wait();
    messageContentPtr = upcxx::broadcast(messageContentPtr, MASTER).wait();
    message.size = upcxx::rget(messageSizePtr).wait();
    message.content.resize(message.size);
    upcxx::rget(messageContentPtr, message.content.data(), message.size).wait();

    if (isMaster()) {
        upcxx::delete_array(messageContentPtr);
        upcxx::delete_(messageSizePtr);
    }
}

void MPINode::sendMessageToMaster() { }

void MPINode::receiveMessageFromWorker(const int workerRank) { }
#endif