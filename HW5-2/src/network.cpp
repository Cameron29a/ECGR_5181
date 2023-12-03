#include "network.h"

// NetworkNode methods
void NetworkNode::receiveMessage(const Message& message) {
    // Handle message based on its type
    switch (message.type) {
        case MessageType::ReadMiss:
            // Handle ReadMiss
            break;
        // Other message types...
        default:
            // Default action
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

