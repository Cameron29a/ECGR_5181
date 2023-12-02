#ifndef NETWORKNODE_H
#define NETWORKNODE_H

#include <vector>

class NetworkNode {
    std::vector<NetworkNode*> connectedNodes;  // List of nodes this node is connected to

public:
    virtual void sendMessage(/* message parameters */) = 0;
    virtual void receiveMessage(/* message parameters */) = 0;

    void connectToNode(NetworkNode* node) {
        connectedNodes.push_back(node);
    }

};

#endif //NETWORKNODE_H
