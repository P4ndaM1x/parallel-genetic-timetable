/**
 * @file Node.hpp
 * @brief Contains the declaration of the Node class.
 */
#pragma once

#include <string>

/**
 * @brief Represents an abstract node in a parallel program.
 */
class Node {
public:
    static constexpr int MASTER = 0; /**< The rank of the master node. */

    /**
     * @brief Checks if the function is called from the master node.
     * @return True if called from the master node, false otherwise.
     */
    static bool isCalledFromMaster();

    Node() = default;
    virtual ~Node() = default;

    Node(const Node&) = delete;
    Node& operator=(const Node&) = delete;
    Node(Node&&) = delete;
    Node& operator=(Node&&) = delete;

    /**
     * @brief Gets the rank of the node.
     * @return The rank of the node.
     */
    int getRank() const { return rank; }

    /**
     * @brief Gets the total number of nodes in the parallel program.
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
    void setMessage(const std::string_view& data)
    {
        message.content = data;
        message.size = message.content.size();
    }

    /**
     * @brief Broadcasts the message from the master node to all nodes.
     */
    virtual void broadcastMessageFromMaster() = 0;

    // /**
    //  * @brief Sends the message from the current node to the master node.
    //  */
    // virtual void sendMessageToMaster() = 0;

    // /**
    //  * @brief Receives a message from a worker node if the current node is the master node.
    //  * @param workerRank The rank of the worker node.
    //  */
    // virtual void receiveMessageFromWorker(const int workerRank) = 0;

protected:
    /**
     * @brief Represents a message to be sent or received by the parallel library interface.
     */
    struct Message {
        std::string content{}; /**< The content of the message. */
        int size{-1}; /**< The size of the message. */
    } message;

    int rank{-1}; /**< The rank of the node. */
    int size{-1}; /**< The total number of nodes. */
};
