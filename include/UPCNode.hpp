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
            std::strncpy(messageContentPtr.local(), message.content.c_str(), message.size);
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

    /**
     * @brief Invokes one-sided communication to remotely put a message from the current node to the
     * master node using UPC++.
     */
    void sendMessageToMaster()
    {
        if (isMaster())
            return;

        message.size = message.content.size();
        upcxx::global_ptr<int> messageSizePtr = upcxx::new_<int>(message.size);
        upcxx::global_ptr<char> messageContentPtr = upcxx::new_array<char>(message.size);
        std::strncpy(messageContentPtr.local(), message.content.c_str(), message.size);

        upcxx::rpc(
            MASTER,
            [this](
                upcxx::global_ptr<int> messageSizePtr, upcxx::global_ptr<char> messageContentPtr
            ) {
                const auto messageSize = upcxx::rget(messageSizePtr).wait();
                std::string messageContent;
                messageContent.resize(messageSize);
                upcxx::rget(messageContentPtr, messageContent.data(), messageSize).wait();
                message.size = messageContent.size();
                message.content = messageContent;
            },
            messageSizePtr,
            messageContentPtr
        )
            .wait();

        upcxx::delete_array(messageContentPtr);
        upcxx::delete_(messageSizePtr);
    }

    /**
     * @brief Does nothing as a message is already put in the sendMessageToMaster method.
     */
    void receiveMessageFromWorker(const int workerRank) { }
};
