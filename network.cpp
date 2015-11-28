#include "network.h"

Network::Network(size_t verticesNumber, VertexIndex sourceIndex, VertexIndex sinkIndex) :
        verticesNumber(verticesNumber), sourceIndex(sourceIndex), sinkIndex(sinkIndex) {
    edgesList.assign(verticesNumber, std::list<Edge>());
}

void Network::addEdge(const Edge &newEdge) {
    assert(newEdge.getFirstVertexIndex() < verticesNumber
           && newEdge.getSecondVertexIndex() < verticesNumber);
    edgesList[newEdge.getFirstVertexIndex()].push_back(newEdge);
}

bool Network::hasEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity) {
    assert(firstVertexIndex < verticesNumber && secondVertexIndex < verticesNumber);
    Edge edgeToFind(firstVertexIndex, secondVertexIndex, 0, capacity, 0);
    std::list<Edge>::iterator searchResultIt = find(edgesList[firstVertexIndex].begin(),
                                                    edgesList[firstVertexIndex].end(), edgeToFind);
    return searchResultIt != edgesList[firstVertexIndex].end();
}

Edge &Network::getEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity) {
    assert(firstVertexIndex < verticesNumber && secondVertexIndex < verticesNumber);
    Edge edgeToFind(firstVertexIndex, secondVertexIndex, 0, capacity, 0);
    std::list<Edge>::iterator searchResultIt = find(edgesList[firstVertexIndex].begin(),
                                                    edgesList[firstVertexIndex].end(), edgeToFind);
    assert(searchResultIt != edgesList[firstVertexIndex].end());
    return *searchResultIt;
}

size_t Network::getVerticesNumber() const {
    return verticesNumber;
}

FlowType Network::getNetworkFlowAmount() const {
    FlowType flowSum = 0;
    for (Edge edge : edgesList[sourceIndex]) {
        flowSum += edge.getFlow();
    }
    return flowSum;
}

std::list<Edge> &Network::getEdgesListFromVertex(VertexIndex vertex) {
    assert(vertex < verticesNumber);
    return edgesList[vertex];
}

void Network::addEdgeFlow(Edge &edge, FlowType deltaFlow) {
    edge.setFlow(edge.getFlow() + deltaFlow);
}

void Network::setEdgeFlow(std::list<Edge>::iterator edge, FlowType newFlowValue) {
    edge->setFlow(newFlowValue);
}

void Network::setEdgeFlow(Edge &edge, FlowType newFlowValue) {
    edge.setFlow(newFlowValue);
}

void Network::setEdgeFlow(VertexIndex vertexFrom, VertexIndex vertexTo, FlowType capacity, FlowType newFlowValue) {
    setEdgeFlow(getEdge(vertexFrom, vertexTo, capacity), newFlowValue);
}

void Network::clearFlow() {
    for (VertexIndex vertex = 0; vertex < getVerticesNumber(); ++vertex) {
        for (auto edgeIt = edgesList[vertex].begin(); edgeIt != edgesList[vertex].end(); ++edgeIt) {
            edgeIt->setFlow(0);
        }
    }
}

VertexIndex Network::getSourceIndex() const {
    return sourceIndex;
}

VertexIndex Network::getSinkIndex() const {
    return sinkIndex;
}

FlowType Edge::getFlow() const {
    return flow;
}

void Edge::setFlow(const FlowType &value) {
    flow = value;
}

VertexIndex Edge::getFirstVertexIndex() const {
    return firstVertexIndex;
}

VertexIndex Edge::getSecondVertexIndex() const {
    return secondVertexIndex;
}

FlowType Edge::getCapacity() const {
    return capacity;
}
