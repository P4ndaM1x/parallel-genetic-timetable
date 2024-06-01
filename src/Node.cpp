/**
 * @file Node.cpp
 * @brief Implementation file of the Node class.
 */
#include "Node.hpp"

#ifndef UPCXX
#include "mpi.h"

bool Node::isCalledFromMaster()
{
    int localRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &localRank);
    return localRank == MASTER;
}

#else
#include <upcxx/upcxx.hpp>

bool Node::isCalledFromMaster() { return upcxx::rank_me() == MASTER; }

#endif
