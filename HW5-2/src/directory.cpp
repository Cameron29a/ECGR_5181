#include "directory.h"

void Directory::updateEntry(uint64_t address, int cpuID, DirectoryState newState) {
    size_t index = address % numEntries;
    DirectoryEntry& entry = entries[index];

    entry.tag = address;
    entry.state = newState;
}

bool Directory::isCachedElsewhere(uint64_t address, int cpuID) {
    size_t index = address % numEntries;
    const DirectoryEntry& entry = entries[index];

    return (entry.tag == address && entry.state != DirectoryState::UNCACHED);
}

uint64_t Directory::readData(uint64_t address, int requestingCpuID, Ram& ram) {
    size_t index = address % numEntries;
    DirectoryEntry& entry = entries[index];

    if (entry.tag != address) {
        uint64_t data = ram.read(address);
        entry.tag = address;
        entry.state = DirectoryState::SHARED;
        return data;
    }

    if (entry.state == DirectoryState::EXCLUSIVE) {
        // Fetch data from the owner node instead of memory
        // Update the state accordingly
        entry.state = DirectoryState::SHARED;
        // Return the fetched data (this part is dependent on your fetch implementation)
    } else if (entry.state == DirectoryState::SHARED) {
        return ram.read(address);
    }

    return 0; // Indicative of an error or invalid state
}

void Directory::sendNetworkMessage(const Message& message) {
    network->sendMessage(message);
    Directory::printDirectoryEntryState(message.address);
}
void Directory::receiveMessage(const Message& message) {
    size_t index = message.address % numEntries;
    DirectoryEntry& entry = entries[index];
    std::cout << "Directory received message type: " << message.messageTypeToString() << " for address: " << message.address << " from CPU" << message.sourceID << std::endl;


    switch (message.type) {
        case MessageType::ReadMiss:
            if (entry.state == DirectoryState::UNCACHED) {
                entry.state = DirectoryState::SHARED;
                 entry.sharers.clear();
        entry.sharers.insert(message.sourceID);
                sendNetworkMessage(Message(MessageType::DataValueReply, message.address, ram.read(message.address), -1, message.sourceID));
            } else if (entry.state == DirectoryState::SHARED) {
                    entry.sharers.insert(message.sourceID);
                sendNetworkMessage(Message(MessageType::DataValueReply, message.address, ram.read(message.address), -1, message.sourceID));
            } else if (entry.state == DirectoryState::EXCLUSIVE) {
                sendNetworkMessage(Message(MessageType::Fetch, message.address, 0, -1, message.sourceID));
            }
            break;

        case MessageType::WriteMiss:
            if (entry.state == DirectoryState::SHARED) {
                // Send invalidate messages to all sharers except the requesting node
                for (int sharerID : entry.sharers) {
                    if (sharerID != message.sourceID) {
                        sendNetworkMessage(Message(MessageType::Invalidate, message.address, 0, sharerID, -1));
                    }
                }
            }
            
            // For both UNCACHED and SHARED states
            entry.state = DirectoryState::EXCLUSIVE;
            entry.sharers.clear();
            entry.sharers.insert(message.sourceID);
            // Send the data value reply to the requesting node
            sendNetworkMessage(Message(MessageType::DataValueReply, message.address, ram.read(message.address), -1, message.sourceID));
            break;

        case MessageType::DataWriteBack:
            ram.write(message.address, message.data);
            entry.state = DirectoryState::UNCACHED;
            break;

        default:
            // Default action or unrecognized message type
            break;
    }
}


