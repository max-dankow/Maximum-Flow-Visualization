#ifndef NETWORK_H
#define NETWORK_H

#include <list>
#include <vector>
#include <stdlib.h>
#include <assert.h>
#include <algorithm>

typedef size_t VertexIndex;
typedef long long FlowType;

class Edge {
public:
    Edge(VertexIndex firstVertex, VertexIndex secondVertex,
         double weight, FlowType capacity, FlowType flow) :
            firstVertexIndex(firstVertex),
            secondVertexIndex(secondVertex),
            weight(weight),
            capacity(capacity),
            flow(flow) { }

    Edge() { }

    bool operator<(const Edge &other) const {
        return weight < other.weight;
    }

    bool operator==(const Edge &other) const {
        // Условие capacity == 0 определяет, является ли ребро неявным обратным,
        // т.е. было добавлено нами для возвращения потока.
        return (firstVertexIndex == other.firstVertexIndex) &&
               (secondVertexIndex == other.secondVertexIndex) &&
               (((capacity == 0 && other.capacity == 0) || (capacity != 0 && other.capacity != 0)));
    }

    FlowType getCapacity() const;

    FlowType getFlow() const;

    void setFlow(const FlowType &value);

    VertexIndex getFirstVertexIndex() const;

    VertexIndex getSecondVertexIndex() const;

private:
    VertexIndex firstVertexIndex, secondVertexIndex;
    double weight;
    FlowType capacity, flow;
};

class Network {
public:
    Network(size_t verticesNumber, VertexIndex sourceIndex, VertexIndex sinkIndex);

    Network();

    void addEdge(const Edge &newEdge);

    bool hasEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity);

    Edge &getEdge(VertexIndex firstVertexIndex, VertexIndex secondVertexIndex, FlowType capacity);

    size_t getVerticesNumber() const;

    FlowType getEdgeFlowAmount(VertexIndex vertexFrom, VertexIndex vertexTo) const;

    FlowType getNetworkFlowAmount() const;

    std::list<Edge> &getEdgesListFromVertex(VertexIndex vertex);

    void addEdgeFlow(Edge &edge, FlowType deltaFlow);

    void setEdgeFlow(std::list<Edge>::iterator edge, FlowType newFlowValue);

    void setEdgeFlow(Edge &edge, FlowType newFlowValue);

    void setEdgeFlow(VertexIndex vertexFrom, VertexIndex vertexTo, FlowType capacity, FlowType newFlowValue);

    void clearFlow();

    VertexIndex getSourceIndex() const;

    VertexIndex getSinkIndex() const;

private:
    size_t verticesNumber;
    VertexIndex sourceIndex, sinkIndex;
    std::vector<std::list<Edge> > edgesList;
};

#endif // NETWORK_H
