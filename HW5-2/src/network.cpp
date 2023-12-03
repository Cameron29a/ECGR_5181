#include "network.h"
#include <iostream>
#include "cache.h"
#include "directory.h"
void NetworkNode::receiveMessage(const Message& message) {
    // Handle message based on its type
    switch (message.type) {
        case MessageType::ReadMiss:
        case MessageType::WriteMiss:
        case MessageType::Invalidate:
            // These messages are typically directed towards a cache
            if (cache) {
                cache->handleNetworkMessage(message);
            }
            break;
        case MessageType::DataWriteBack:
        case MessageType::DataValueReply:
        case MessageType::Fetch:
            // These messages are typically for the directory
            if (directory) {
                directory->receiveMessage(message);
            }
            break;
        default:
            // Default action or error handling
            std::cerr << "Unhandled message type received." << std::endl;
            break;
    }
}

// Network methods
void Network::addNode(NetworkNode& node) {
    node.sendMessage = [this](Message message) { this->sendMessage(message); };
    nodes.push_back(node);
}

void Network::sendMessage(const Message& message) {
    // Simulate network delay here if necessary
        std::cout << "Sending Message: Type=" << static_cast<int>(message.type) 
              << ", Address=" << message.address << ", SourceID=" << message.sourceID 
              << ", DestID=" << message.destID << std::endl;
    routeMessage(message);
}

void Network::routeMessage(const Message& message) {
    for (auto& node : nodes) {
        if (node.id == message.destID) {
            node.receiveMessage(message);
            break;
        }
    }
}


