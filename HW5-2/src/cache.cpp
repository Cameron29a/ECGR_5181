#include "cache.h"
#include "directory.h" 

CacheState Cache::getCurrentState(uint64_t address)
{
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        return it->second.state;
    } else {
        return CacheState::INVALID;
    }
}

void Cache::setCurrentState(uint64_t address, CacheState newState) {
    auto it = cacheData.find(address);

    if (it != cacheData.end()) {
        it->second.state = newState;
    }
        printCacheLineState(address); // Display the state of the cache line

}

uint64_t Cache::readFromCache(uint64_t address) {
    auto it = cacheData.find(address);
    // Check if cache line is in the cache
    if (it != cacheData.end() && it->second.state != CacheState::INVALID) {
        return it->second.data; // Cache hit
    } else {
        uint64_t data = 0;
        // Cache miss, consult the directory
        if (directory.isCachedElsewhere(address, this->id)) {
            // Logic to handle case when cache line is cached elsewhere
	data = directory.readData(address, this->id, ram);
            cacheData[address] = {CacheState::SHARED, address, data}; // Update cache
        } else {
            // Cache line is not in other caches, fetch from main memory (or simulate fetching)
            data = ram.read(address);
            cacheData[address] = {CacheState::EXCLUSIVE, address, data}; // Update cache
        }
     printCacheLineState(address); // Display the state of the cache line
     return data;
    }
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    directory.updateEntry(address, this->id, CacheState::MODIFIED);
    cacheData[address] = {CacheState::MODIFIED, address, data};
}

void Cache::printCacheLineState(uint64_t address) {
    auto it = cacheData.find(address);
    if (it != cacheData.end()) {
        std::cout << "Address: " << address << " State: ";
        switch (it->second.state) {
            case CacheState::MODIFIED: std::cout << "M"; break;
            case CacheState::EXCLUSIVE: std::cout << "E"; break;
            case CacheState::SHARED: std::cout << "S"; break;
            case CacheState::INVALID: std::cout << "I"; break;
            default: std::cout << "Unknown";
        }
        std::cout << std::endl;
    } else {
        std::cout << "Address: " << address << " State: Not in Cache" << std::endl;
    }
}
void Cache::handleNetworkMessage(const Message& message) {
    switch (message.type) {
        case MessageType::ReadMiss:
            if (getCurrentState(message.address) == CacheState::MODIFIED) {
                // If the current state is MODIFIED, write back the data and change state to SHARED
                networkNode->sendMessage(Message(MessageType::DataWriteBack, message.address, readFromCache(message.address), id, message.sourceID));
                setCurrentState(message.address, CacheState::SHARED);
            }
            break;

        case MessageType::WriteMiss:
            if (getCurrentState(message.address) == CacheState::MODIFIED || getCurrentState(message.address) == CacheState::EXCLUSIVE) {
                // For WriteMiss, if the state is MODIFIED or EXCLUSIVE, write back and invalidate
                networkNode->sendMessage(Message(MessageType::DataWriteBack, message.address, readFromCache(message.address), id, message.sourceID));
                setCurrentState(message.address, CacheState::INVALID);
            }
            break;

        case MessageType::Invalidate:
            // Invalidate the cache line if it's in the cache
            setCurrentState(message.address, CacheState::INVALID);
            break;

        case MessageType::DataWriteBack:
            // Update the cache with the new data if the cache line is present
            if (getCurrentState(message.address) != CacheState::INVALID) {
                writeToCache(message.address, message.data);
                setCurrentState(message.address, CacheState::SHARED);
            }
            break;

        case MessageType::DataValueReply:
            // Update the cache line with the data from the directory
            writeToCache(message.address, message.data);
            if (getCurrentState(message.address) == CacheState::INVALID) {
                setCurrentState(message.address, CacheState::SHARED);
            }
            break;

        case MessageType::Fetch:
            // Respond to a fetch request by sending the data back to the directory
            if (getCurrentState(message.address) == CacheState::MODIFIED || getCurrentState(message.address) == CacheState::EXCLUSIVE) {
                networkNode->sendMessage(Message(MessageType::DataValueReply, message.address, readFromCache(message.address), id, -1));
                setCurrentState(message.address, CacheState::SHARED);
            }
            break;

        default:
            // Default action or unrecognized message type
            break;
    }
}


