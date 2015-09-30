#include "network.h"

Network::Network(size_t verticesNumber, VertexIndex sourceIndex, VertexIndex sinkIndex):
    verticesNumber(verticesNumber), sourceIndex(sourceIndex), sinkIndex(sinkIndex)
{
    edgesList.assign(verticesNumber, std::list<Edge>());
}

void Network::addEdge(const Edge &newEdge)
{
    assert(newEdge.firstVertexIndex < verticesNumber && newEdge.secondVertexIndex < verticesNumber);
    edgesList[newEdge.firstVertexIndex].push_back(newEdge);
}

bool Network::hasEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex)
{
    assert(firstVertexIndex < verticesNumber && secondVertexIndex < verticesNumber);
    Edge edgeToFind(firstVertexIndex, secondVertexIndex, 0, 0, 0);
    std::list<Edge>::iterator searchResultIt = find(edgesList[firstVertexIndex].begin(), edgesList[firstVertexIndex].end(), edgeToFind);
    return searchResultIt != edgesList[firstVertexIndex].end();
}

Edge& Network::getEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex)
{
    assert(firstVertexIndex < verticesNumber && secondVertexIndex < verticesNumber);
    Edge edgeToFind(firstVertexIndex, secondVertexIndex, 0, 0, 0);
    std::list<Edge>::iterator searchResultIt = find(edgesList[firstVertexIndex].begin(), edgesList[firstVertexIndex].end(), edgeToFind);
    assert(searchResultIt != edgesList[firstVertexIndex].end());
    return *searchResultIt;
}

size_t Network::getVerticesNumber() const
{
    return verticesNumber;
}

std::list<Edge> &Network::getEdgesListFromVertex(VertexIndex vertex)
{
    assert(vertex < verticesNumber);
    return edgesList[vertex];
}

void Network::addEdgeFlow(Edge &edge, FlowType deltaFlow)
{
    edge.setFlow(edge.getFlow() + deltaFlow);
}

void Network::setEdgeFlow(std::list<Edge>::iterator edge, FlowType newFlowValue)
{
    edge->setFlow(newFlowValue);
}

void Network::setEdgeFlow(Edge &edge, FlowType newFlowValue)
{
    edge.setFlow(newFlowValue);
}

void Network::setEdgeFlow(VertexIndex vertexFrom, VertexIndex vertexTo, FlowType newFlowValue)
{
    setEdgeFlow(getEdge(vertexFrom, vertexTo), newFlowValue);
}
VertexIndex Network::getSourceIndex() const
{
    return sourceIndex;
}
/*
void Network::setSourceIndex(const VertexIndex &value)
{
    sourceIndex = value;
}*/
VertexIndex Network::getSinkIndex() const
{
    return sinkIndex;
}
/*
void Network::setSinkIndex(const VertexIndex &value)
{
    sinkIndex = value;
}


*/


FlowType Edge::getFlow() const
{
    return flow;
}

void Edge::setFlow(const FlowType &value)
{
    flow = value;
}

VertexIndex Edge::getFirstVertexIndex() const
{
    return firstVertexIndex;
}

void Edge::setFirstVertexIndex(const size_t &value)
{
    firstVertexIndex = value;
}

VertexIndex Edge::getSecondVertexIndex() const
{
    return secondVertexIndex;
}

void Edge::setSecondVertexIndex(const size_t &value)
{
    secondVertexIndex = value;
}
FlowType Edge::getCapacity() const
{
    return capacity;
}

void Edge::setCapacity(const FlowType &value)
{
    capacity = value;
}
