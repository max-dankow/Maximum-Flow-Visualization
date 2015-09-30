#ifndef ALGORITHMRELABELTOFRONT_H
#define ALGORITHMRELABELTOFRONT_H

#include <stdlib.h>
#include <vector>
#include "network.h"

class algorithmRelabelToFront
{
public:
    algorithmRelabelToFront(Network network):network(network){}
    void initializePreflow();
    void calculateMaxFlow();
private:
    Network network;
    std::vector<long> verticesHeight;
    std::vector<FlowType> verticesExcessFlow;
    std::vector<std::list<Edge>::iterator> currentAdjacentEdges;
    void relabelVertex(VertexIndex vertex);
    void pushExcessFlow(Edge& edge);
    void dischargeVertex(VertexIndex vertex);
};

#endif // ALGORITHMRELABELTOFRONT_H
