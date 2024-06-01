/**
 * @file UPCNode.hpp
 * @brief Defines the UPCNode class.
 */
#pragma once

#include "Node.hpp"

#include <upcxx/upcxx.hpp>

/**
 * @brief Represents a node in a parallel program utilizing UPC++.
 */
class UPCNode : public Node {
public:
    /**
     * @brief Constructs an Node object, initializing UPC++.
     */
    UPCNode()
    {
        upcxx::init();
        rank = upcxx::rank_me();
        size = upcxx::rank_n();
    }

    /**
     * @brief Destroys the Node object, finalizing UPC++.
     */
    ~UPCNode() { upcxx::finalize(); }

    /**
     * @brief Broadcasts the message from the master node to all nodes using UPC++ communication.
     */
    void broadcastMessageFromMaster()
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

    void sendMessageToMaster() { }

    void receiveMessageFromWorker(const int workerRank) { }
};
