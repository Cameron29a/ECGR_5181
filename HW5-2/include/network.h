#ifndef NETWORK_H
#define NETWORK_H

#include <queue>
#include <vector>
#include <functional>
#include <cstdint>
// Forward declarations
class Cache;
class Directory;

// Message Types for Network Communication
enum class MessageType {
    ReadMiss,
    WriteMiss,
    DataWriteBack,
    Invalidate,
    DataValueReply,
    Fetch
};

// Structure for a message in the network
struct Message {
    MessageType type;
    uint64_t address;
    uint64_t data;  // Used for DataWriteBack and DataValueReply
    int sourceID;   // ID of the source node
    int destID;     // ID of the destination node

    Message(MessageType type, uint64_t addr, uint64_t data, int srcID, int dstID)
        : type(type), address(addr), data(data), sourceID(srcID), destID(dstID) {}
};

// Forward declaration
class Cache;

// Network Node Class
class NetworkNode {
public:
    int id;
    Cache* cache;
    std::function<void(Message)> sendMessage;

    NetworkNode(int id, Cache* cache) : id(id), cache(cache) {}

    void receiveMessage(const Message& message);
};

// Network Class
class Network {
    std::vector<NetworkNode> nodes;

public:
    void addNode(NetworkNode& node);
    void sendMessage(const Message& message);
    void routeMessage(const Message& message);
};

#endif // NETWORK_H

