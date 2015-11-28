#include "algorithmrelabeltofront.h"

void AlgorithmRelabelToFront::initializePreflow() {
    verticesExcessFlow.assign(network.getVerticesNumber(), 0);
    verticesHeight.assign(network.getVerticesNumber(), 0);
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex) {
        std::list<Edge> &edges = network.getEdgesListFromVertex(vertex);
        for (auto edgesIt = edges.begin(); edgesIt != edges.end(); ++edgesIt) {
            if (edgesIt->getCapacity() == 0) {
                continue;
            }
            if (vertex == network.getSourceIndex()) {
                //пропускаем по ребру максимальный допустимый поток и соответсвенно меняем избыток
                network.setEdgeFlow(*edgesIt, edgesIt->getCapacity());
                verticesExcessFlow[edgesIt->getSecondVertexIndex()] += edgesIt->getCapacity();
                verticesExcessFlow[vertex] -= edgesIt->getCapacity();
            }
            else {
                network.setEdgeFlow(*edgesIt, 0);
            }
            network.setEdgeFlow(network.getEdge(edgesIt->getSecondVertexIndex(),
                                                edgesIt->getFirstVertexIndex(),
                                                0),
                                -edgesIt->getFlow());
        }
    }
    verticesHeight[network.getSourceIndex()] = network.getVerticesNumber();
}

FlowType AlgorithmRelabelToFront::calculateMaxFlow() {
    init();
    AlgoAction action(AlgoAction::ACTION_NOTHING);
    while (action.getType() != AlgoAction::ACTION_TERMINATE) {
        action = doStep();
    }
    assert(network.getNetworkFlowAmount() == verticesExcessFlow[network.getSinkIndex()]);
    return network.getNetworkFlowAmount();
}

// Инициализирует все необходимые данные для алгоритма
AlgoAction AlgorithmRelabelToFront::init() {
    network.clearFlow();
    initializePreflow();
    verticesList.clear();
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex) {
        if (vertex != network.getSourceIndex() && vertex != network.getSinkIndex()) {
            verticesList.push_back(vertex);
        }
    }
    currentAdjacentEdge.clear();
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex) {
        currentAdjacentEdge.push_back(network.getEdgesListFromVertex(vertex).begin());
    }
    currentVertexIt = verticesList.begin();
    oldHeightCurrentVertex = verticesHeight[*currentVertexIt];
    return AlgoAction(AlgoAction::ACTION_SELECT, *currentVertexIt);
}

AlgoAction AlgorithmRelabelToFront::doStep() {
    // Если при попытке провести discharge произошло какое-то содержательное
    // действие (push или relabel), то очередной шаг равершен.
    AlgoAction tryDischarge = dischargeVertex(*currentVertexIt);
    if (tryDischarge.getType() != AlgoAction::ACTION_NOTHING) {
        return tryDischarge;
    }
    // Иначе продолжаем алгоритм.
    if (verticesHeight[*currentVertexIt] > oldHeightCurrentVertex) {
        // Переместрить вершину в начало.
        VertexIndex vertex = *currentVertexIt;
        verticesList.erase(currentVertexIt);
        verticesList.push_front(vertex);
        currentVertexIt = verticesList.begin();
    }
    ++currentVertexIt;
    if (currentVertexIt == verticesList.end()) {
        return AlgoAction(AlgoAction::ACTION_TERMINATE);
    }
    else {
        oldHeightCurrentVertex = verticesHeight[*currentVertexIt];
        return AlgoAction(AlgoAction::ACTION_SELECT, *currentVertexIt);
    }
}

AlgoAction AlgorithmRelabelToFront::doSteps(unsigned stepsCount) {
    AlgoAction lastAction;
    for (unsigned step = 0; step < stepsCount; ++step) {
        lastAction = doStep();
        //assert(lastAction.getType() != AlgoAction::ACTION_NOTHING);
        if (lastAction.getType() == AlgoAction::ACTION_TERMINATE) {
            return lastAction;
        }
    }
    return lastAction;
}

void AlgorithmRelabelToFront::relabelVertex(VertexIndex vertex) {
    const long NOT_FOUND = -1;
    long minHeigth = NOT_FOUND;
    for (Edge adjacentEdge: network.getEdgesListFromVertex(vertex)) {
        if (adjacentEdge.getCapacity() - adjacentEdge.getFlow() > 0
            && (minHeigth == NOT_FOUND || verticesHeight[adjacentEdge.getSecondVertexIndex()] < minHeigth)) {
            minHeigth = verticesHeight[adjacentEdge.getSecondVertexIndex()];
        }
    }
    assert(minHeigth != NOT_FOUND);
    verticesHeight[vertex] = minHeigth + 1;
}

void AlgorithmRelabelToFront::pushExcessFlow(Edge &edge) {
    VertexIndex vertexFrom = edge.getFirstVertexIndex();
    VertexIndex vertexTo = edge.getSecondVertexIndex();
    FlowType deltaFlow = std::min(verticesExcessFlow[vertexFrom], edge.getCapacity() - edge.getFlow());
    network.addEdgeFlow(edge, deltaFlow);
    if (edge.getCapacity() != 0) {
        network.addEdgeFlow(network.getEdge(vertexTo, vertexFrom, 0), -deltaFlow);
    }
    else {
        network.addEdgeFlow(network.getEdge(vertexTo, vertexFrom, 1), -deltaFlow);
    }
    verticesExcessFlow[vertexFrom] -= deltaFlow;
    verticesExcessFlow[vertexTo] += deltaFlow;
}

AlgoAction AlgorithmRelabelToFront::dischargeVertex(VertexIndex vertex) {
    while (verticesExcessFlow[vertex] > 0) {
        auto adjacentEdge = currentAdjacentEdge[vertex];
        if (adjacentEdge == network.getEdgesListFromVertex(vertex).end()) {
            relabelVertex(vertex);
            currentAdjacentEdge[vertex] = network.getEdgesListFromVertex(vertex).begin();
            return AlgoAction(AlgoAction::ACTION_RELABEL, vertex);
        }
        else {
            if ((adjacentEdge->getCapacity() - adjacentEdge->getFlow() > 0)
                && (verticesHeight[vertex] == verticesHeight[adjacentEdge->getSecondVertexIndex()] + 1)) {
                pushExcessFlow(*adjacentEdge);
                return AlgoAction(AlgoAction::ACTION_PUSH, *adjacentEdge);
            }
            else {
                ++currentAdjacentEdge[vertex];
            }
        }
    }
    return AlgoAction(AlgoAction::ACTION_NOTHING);
}

Network &AlgorithmRelabelToFront::getNetwork() {
    return network;
}

long AlgorithmRelabelToFront::getVertexHeight(VertexIndex vertex) const {
    assert(vertex >= 0 && vertex < verticesHeight.size());
    return verticesHeight[vertex];
}

long AlgorithmRelabelToFront::getVertexExcessFlow(VertexIndex vertex) const {
    assert(vertex >= 0 && vertex < verticesHeight.size());
    return verticesExcessFlow[vertex];
}

AlgoAction::ActionType AlgoAction::getType() const {
    return type;
}

VertexIndex AlgoAction::getVertexInfo() const {
    assert(type == ACTION_SELECT || type == ACTION_RELABEL);
    return vertexInfo;
}

Edge AlgoAction::getEdgeInfo() const {
    assert(type == ACTION_PUSH);
    return edgeInfo;
}
