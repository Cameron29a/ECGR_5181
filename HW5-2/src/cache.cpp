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
      //  printCacheLineState(address); // Display the state of the cache line

}

uint64_t Cache::readFromCache(uint64_t address) {
    auto it = cacheData.find(address);
    if (it != cacheData.end() && it->second.state != CacheState::INVALID) {
        return it->second.data; // Cache hit
    } else {
        // Cache miss
        Message readMissMsg(MessageType::ReadMiss, address, 0, this->id, -1);
        directory.sendNetworkMessage(readMissMsg);
        // Assume the data is now fetched and updated in the cache
        return cacheData[address].data;
    }
}

void Cache::writeToCache(uint64_t address, uint64_t data) {
    auto it = cacheData.find(address);
    if (it != cacheData.end() && (it->second.state == CacheState::MODIFIED || it->second.state == CacheState::EXCLUSIVE)) {
        it->second.data = data; // Direct update in cache
    } else {
        // Write miss
        Message writeMissMsg(MessageType::WriteMiss, address, data, this->id, -1);
        directory.sendNetworkMessage(writeMissMsg);
        // Assume the directory handles the request and updates the cache as needed
    }
}


void Cache::printCacheLineState(uint64_t address,int cpuID) const {
    auto it = cacheData.find(address);
    if (it != cacheData.end()) {
        std::cout  << "CPU" << cpuID << " Address: " << address << " State: ";
        switch (it->second.state) {
            case CacheState::MODIFIED: std::cout << "M"; break;
            case CacheState::EXCLUSIVE: std::cout << "E"; break;
            case CacheState::SHARED: std::cout << "S"; break;
            case CacheState::INVALID: std::cout << "I"; break;
            default: std::cout << "Unknown";
        }
        std::cout << std::endl;
    } else {
        std::cout  << "CPU" << cpuID << " Address: " << address << " State: Not in Cache" << std::endl;
    }
}
void Cache::handleNetworkMessage(const Message& message) {
    switch (message.type) {
    case MessageType::ReadMiss:
        if (getCurrentState(message.address) == CacheState::MODIFIED || 
            getCurrentState(message.address) == CacheState::EXCLUSIVE) {
            // Write back the data and change state to SHARED
            networkNode->sendMessage(Message(MessageType::DataWriteBack, message.address, readFromCache(message.address), id, message.sourceID));
            setCurrentState(message.address, CacheState::SHARED);
        } else if (directory.isCachedElsewhere(message.address, id)) {
            // If the block is exclusive in another cache, set this cache's state to SHARED
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


