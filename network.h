#ifndef NETWORK_H
#define NETWORK_H

#include <list>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>

typedef size_t VertexIndex;
typedef long FlowType;

struct Edge
{
    Edge(VertexIndex newFirstVertex, VertexIndex newSecondVertex,
          double newWeight, double newCapacity, double newFlow):
        firstVertexIndex(newFirstVertex), secondVertexIndex(newSecondVertex),
        weight(newWeight), capacity(newCapacity), flow(newFlow){}
    Edge(){}

    bool operator <(const Edge &other) const
    {
        return weight < other.weight;
    }
    bool operator ==(const Edge &other) const
    {
        return (firstVertexIndex == other.firstVertexIndex) &&
               (secondVertexIndex == other.secondVertexIndex) &&
               (capacity == other.capacity);
    }

    VertexIndex firstVertexIndex, secondVertexIndex;
    double weight;
    FlowType capacity, flow;

public:
    FlowType getCapacity() const;
    void setCapacity(const FlowType &value);
    FlowType getFlow() const;
    void setFlow(const FlowType &value);
    VertexIndex getFirstVertexIndex() const;
    void setFirstVertexIndex(const size_t &value);
    VertexIndex getSecondVertexIndex() const;
    void setSecondVertexIndex(const size_t &value);
};

class Network
{
public:
    Network(size_t verticesNumber, VertexIndex sourceIndex, VertexIndex sinkIndex);
    Network();
    void addEdge(const Edge &newEdge);
    bool hasEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity);
    Edge &getEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity);
    size_t getVerticesNumber() const;
    FlowType getEdgeFlowAmount(VertexIndex vertexFrom, VertexIndex vertexTo) const;
    FlowType getNetworkFlowAmount() const;
    std::list<Edge>& getEdgesListFromVertex(VertexIndex vertex);

    void addEdgeFlow(Edge& edge, FlowType deltaFlow);
    void setEdgeFlow(std::list<Edge>::iterator edge, FlowType newFlowValue);
    void setEdgeFlow(Edge& edge, FlowType newFlowValue);
    void setEdgeFlow(VertexIndex vertexFrom, VertexIndex vertexTo, FlowType capacity, FlowType newFlowValue);
    VertexIndex getSourceIndex() const;
    VertexIndex getSinkIndex() const;
private:
    size_t verticesNumber;
    VertexIndex sourceIndex, sinkIndex;
    std::vector<std::list<Edge> > edgesList;
};

#endif // NETWORK_H
