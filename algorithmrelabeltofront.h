#ifndef ALGORITHMRELABELTOFRONT_H
#define ALGORITHMRELABELTOFRONT_H

#include <stdlib.h>
#include <vector>
#include "network.h"

// Содержит тип действия совершаемого алгоритмом за 1 шаг doStep.
// В соответствии с этим действием содержит дополнительную информацию о нем.
// Используется в визуализаторе для сообветствующего отображения действий алгоритма.
class AlgoAction {
public:
    enum ActionType {
        ACTION_NOTHING,
        ACTION_SELECT,
        ACTION_PUSH,
        ACTION_RELABEL,
        ACTION_TERMINATE
    };

    AlgoAction(ActionType actionType = ACTION_NOTHING) : type(actionType) { }

    AlgoAction(ActionType actionType, Edge edgeInfo) :
            type(actionType), edgeInfo(edgeInfo) { }

    AlgoAction(ActionType actionType, VertexIndex vertexInfo) :
            type(actionType), vertexInfo(vertexInfo) { }

    ActionType getType() const;

    VertexIndex getVertexInfo() const;

    Edge getEdgeInfo() const;

private:
    ActionType type;
    Edge edgeInfo;
    VertexIndex vertexInfo;
};

class AlgorithmRelabelToFront {
public:
    AlgorithmRelabelToFront(const Network &network) : network(network) {
        init();
    }

    AlgorithmRelabelToFront();

    FlowType calculateMaxFlow();

    AlgoAction init();

    AlgoAction doStep();

    AlgoAction doSteps(unsigned stepsCount);

    Network &getNetwork();

    long getVertexHeight(VertexIndex vertex) const;

    long getVertexExcessFlow(VertexIndex vertex) const;

private:
    void initializePreflow();

    void relabelVertex(VertexIndex vertex);

    void pushExcessFlow(Edge &edge);

    AlgoAction dischargeVertex(VertexIndex vertex);

    long oldHeightCurrentVertex;
    Network network;
    std::vector<long> verticesHeight;
    std::vector<FlowType> verticesExcessFlow;
    std::vector<std::list<Edge>::iterator> currentAdjacentEdge;
    std::list<VertexIndex> verticesList;
    std::list<VertexIndex>::iterator currentVertexIt;
};

#endif // ALGORITHMRELABELTOFRONT_H
