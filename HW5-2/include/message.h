// message.h
#ifndef MESSAGE_H
#define MESSAGE_H

#include <cstdint>

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
#endif // MESSAGE_H

