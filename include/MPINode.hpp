/**
 * @file MPINode.hpp
 * @brief Contains the declaration of the MPINode class.
 */

#pragma once

#include <string>

/**
 * @brief Represents a node in a parallel MPI program.
 */
class MPINode {
public:
    static constexpr int MASTER = 0; /**< The rank of the master node. */

    /**
     * @brief Checks if the function is called from the master node.
     * @return True if called from the master node, false otherwise.
     */
    static bool isCalledFromMaster();

    /**
     * @brief Constructs an MPINode object, initializing MPI.
     * @param argc Pointer to the number of command-line arguments.
     * @param argv Pointer to the command-line argument values.
     */
    MPINode(int* argc, char*** argv);

    /**
     * @brief Destroys the MPINode object, finalizing MPI.
     */
    ~MPINode();

    MPINode(const MPINode&) = delete;
    MPINode& operator=(const MPINode&) = delete;
    MPINode(MPINode&&) = delete;
    MPINode& operator=(MPINode&&) = delete;

    /**
     * @brief Gets the rank of the node.
     * @return The rank of the node.
     */
    int getRank() const { return rank; }

    /**
     * @brief Gets the total number of nodes in the MPI program.
     * @return The total number of nodes.
     */
    int getSize() const { return size; }

    /**
     * @brief Checks if the node is the master node.
     * @return True if the node is the master node, false otherwise.
     */
    bool isMaster() const { return rank == MASTER; }

    /**
     * @brief Gets a message content on the current node.
     * @return Message content from last communication operation.
     */
    std::string getMessage() const { return message.content; }

    /**
     * @brief Sets the message content to be sent or broadcasted.
     * @param data The message content.
     */
    void setMessage(const std::string_view& data);

    /**
     * @brief Broadcasts the message from the master node to all nodes.
     */
    void broadcastMessageFromMaster();

    /**
     * @brief Sends the message from the current node to the master node.
     */
    void sendMessageToMaster();

    /**
     * @brief Receives a message from a worker node if the current node is the master node.
     * @param workerRank The rank of the worker node.
     */
    void receiveMessageFromWorker(const int workerRank);

private:
    /**
     * @brief Represents a message to be sent or received by the MPI interface.
     */
    struct Message {
        std::string content{}; /**< The content of the message. */
        int size{-1}; /**< The size of the message. */
    } message;

    int rank{-1}; /**< The rank of the node. */
    int size{-1}; /**< The total number of nodes. */
};
