#ifndef ALGORITHMRELABELTOFRONT_H
#define ALGORITHMRELABELTOFRONT_H

#include <stdlib.h>
#include <vector>
#include "network.h"

class AlgorithmRelabelToFront
{
public:
    AlgorithmRelabelToFront(const Network &network):network(network)
    {
        verticesExcessFlow.assign(network.getVerticesNumber(), 0);
        verticesHeight.assign(network.getVerticesNumber(), 0);
        currentAdjacentEdge.reserve(network.getVerticesNumber());
    }
    AlgorithmRelabelToFront();
    FlowType calculateMaxFlow();
    void init();
    bool doStep();
    Network &getNetwork();
    long getVertexHeight(VertexIndex vertex) const;
private:
    Network network;
    std::vector<long> verticesHeight;
    std::vector<FlowType> verticesExcessFlow;
    std::vector<std::list<Edge>::iterator> currentAdjacentEdge;
    std::list<VertexIndex> verticesList;
    std::list<VertexIndex>::iterator currentVertexIt;
    void initializePreflow();
    void relabelVertex(VertexIndex vertex);
    void pushExcessFlow(Edge& edge);
    void dischargeVertex(VertexIndex vertex);
};

#endif // ALGORITHMRELABELTOFRONT_H
