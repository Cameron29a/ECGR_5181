#include "directory.h"

void Directory::updateEntry(uint64_t address, int cpuID, CacheState newState) {
    size_t index = address % numEntries;  // Simple hash function for indexing
    DirectoryEntry& entry = entries[index];
    // Update the tag and state
    entry.tag = address;
    entry.state = newState;

    // Update the CPU mask based on the new state
    switch(newState) {
        case CacheState::MODIFIED:
        case CacheState::EXCLUSIVE:
            entry.cpuMask.reset();
            entry.cpuMask.set(cpuID);
            break;
        case CacheState::SHARED:
            entry.cpuMask.set(cpuID);
            break;
        case CacheState::INVALID:
            entry.cpuMask.reset(cpuID);
            break;
        default:
            // Handle default or unknown cases if necessary
            break;
}
}

bool Directory::isCachedElsewhere(uint64_t address, int cpuID) {
    size_t index = address % numEntries;
    const DirectoryEntry& entry = entries[index];

    if (entry.tag != address || entry.state == CacheState::INVALID) {
        // The cache line is not present or is invalid in the directory
        return false;
    }

    // Check if any other CPU has this cache line
    return entry.cpuMask.any() && !entry.cpuMask.test(cpuID);
}

uint64_t Directory::readData(uint64_t address, int requestingCpuID, Ram& ram) {
    size_t index = address % numEntries;
    DirectoryEntry& entry = entries[index];

    if (entry.tag != address) {
        // The cache line is not present in the directory, fetch from main memory
        // and update the directory entry accordingly
        uint64_t data = ram.read(address);
        entry.tag = address;
        entry.state = CacheState::SHARED;
        entry.cpuMask.reset();
        entry.cpuMask.set(requestingCpuID);
        return data;
    }

    if (entry.state == CacheState::EXCLUSIVE || entry.state == CacheState::MODIFIED) {
        // In Exclusive or Modified state, the data should be fetched from the owner node
        // For simplicity, we're fetching it from memory here
        uint64_t data = ram.read(address);
        entry.state = CacheState::SHARED;
        entry.cpuMask.set(requestingCpuID);
        return data;
    } else if (entry.state == CacheState::SHARED) {
        // If the block is already in Shared state, simply add the requesting CPU to the sharers
        entry.cpuMask.set(requestingCpuID);
        return ram.read(address);
    }

    // If the state is INVALID or not handled, return an indicative value
    return 0;  // Indicative of an error or invalid state
}

void Directory::sendNetworkMessage(const Message& message) {
    network->sendMessage(message);
}
void Directory::receiveMessage(const Message& message) {
    size_t index = message.address % numEntries;
    DirectoryEntry& entry = entries[index];

    switch (message.type) {
        case MessageType::ReadMiss:
            // Handle a Read Miss request
            if (entry.state == CacheState::INVALID || entry.state == CacheState::SHARED) {
                // Add this CPU to the sharers and send the data back
                entry.cpuMask.set(message.sourceID);
                entry.state = CacheState::SHARED;
                sendNetworkMessage(Message(MessageType::DataValueReply, message.address, ram.read(message.address), -1, message.sourceID));
            } else if (entry.state == CacheState::EXCLUSIVE || entry.state == CacheState::MODIFIED) {
                // Change owner, send data fetch request to current owner
                int currentOwner = entry.cpuMask._Find_first();
                entry.cpuMask.reset();
                entry.cpuMask.set(message.sourceID);
                sendNetworkMessage(Message(MessageType::Fetch, message.address, 0, currentOwner, message.sourceID));
                entry.state = CacheState::SHARED;
            }
            break;

        case MessageType::WriteMiss:
            // Handle a Write Miss request
            if (entry.state != CacheState::INVALID) {
                // Invalidate all other caches
                for (size_t i = 0; i < entry.cpuMask.size(); ++i) {
                    if (entry.cpuMask.test(i) && i != message.sourceID) {
                        sendNetworkMessage(Message(MessageType::Invalidate, message.address, 0, -1, i));
                    }
                }
            }
            entry.cpuMask.reset();
            entry.cpuMask.set(message.sourceID);
            entry.state = CacheState::EXCLUSIVE;
            break;

        case MessageType::DataWriteBack:
            // Handle Data Write Back request
            ram.write(message.address, message.data);
            if (entry.state == CacheState::MODIFIED) {
                entry.state = CacheState::SHARED;
            }
            break;

        case MessageType::Invalidate:
            // Handle Invalidate request
            entry.cpuMask.reset(message.sourceID);
            if (entry.cpuMask.none()) {
                entry.state = CacheState::INVALID;
            }
            break;

        default:
            // Other message types or default action
            break;
    }
}
