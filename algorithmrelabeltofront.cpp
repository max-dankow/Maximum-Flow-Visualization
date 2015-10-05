#include "algorithmrelabeltofront.h"

void AlgorithmRelabelToFront::initializePreflow()
{
  //инициализируем Высоту и Избыточный Поток всех вершин
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        verticesExcessFlow[vertex] = 0;
        verticesHeight[vertex] = 0;
    }
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        std::list<Edge>& edges = network.getEdgesListFromVertex(vertex);
        for (auto edgesIt = edges.begin(); edgesIt != edges.end(); ++edgesIt)
        {  
            if (edgesIt->getCapacity() == 0)
            {
                continue;
            }
            if (vertex == network.getSourceIndex())
            {  
              //пропускаем по ребру максимальный допустимый поток и соответсвенно меняем избыток
                network.setEdgeFlow(*edgesIt, edgesIt->getCapacity());
                verticesExcessFlow[edgesIt->getSecondVertexIndex()] += edgesIt->getCapacity();
                verticesExcessFlow[vertex] -= edgesIt->getCapacity();
            }
            else
            {
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

FlowType AlgorithmRelabelToFront::calculateMaxFlow()
{
    init();
    //auto currentVertexIt = verticesList.begin();
    while (!doStep());
    assert(network.getNetworkFlowAmount() == verticesExcessFlow[network.getSinkIndex()]);
    return network.getNetworkFlowAmount();
}

// Инициализирует все необходимые данные для алгоритма
void AlgorithmRelabelToFront::init()
{
    initializePreflow();
    for(VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        if (vertex != network.getSourceIndex() && vertex != network.getSinkIndex())
        {
            verticesList.push_back(vertex);
        }
    }
    for(VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        currentAdjacentEdge.push_back(network.getEdgesListFromVertex(vertex).begin());
    }
    currentVertexIt = verticesList.begin();
}

bool AlgorithmRelabelToFront::doStep()
{
    long oldHeight = verticesHeight[*currentVertexIt];
    dischargeVertex(*currentVertexIt);
    if (verticesHeight[*currentVertexIt] > oldHeight)
    {
        //переместрить вершину в начало
        VertexIndex vertex = *currentVertexIt;
        verticesList.erase(currentVertexIt);
        verticesList.push_front(vertex);
    }
    ++currentVertexIt;
    return currentVertexIt == verticesList.end();
}

Network &AlgorithmRelabelToFront::getNetwork()
{
    return network;
}

long AlgorithmRelabelToFront::getVertexHeight(VertexIndex vertex) const
{
    assert(vertex >= 0 && vertex < verticesHeight.size());
    return verticesHeight[vertex];
}


void AlgorithmRelabelToFront::relabelVertex(VertexIndex vertex)
{
    long minHeigth = -1;
    for (Edge adjacentEdge: network.getEdgesListFromVertex(vertex))
    {
        if (adjacentEdge.getCapacity() - adjacentEdge.getFlow() > 0
                && (minHeigth == -1 || verticesHeight[adjacentEdge.getSecondVertexIndex()] < minHeigth))
        {
            minHeigth = verticesHeight[adjacentEdge.getSecondVertexIndex()];
        }
    }
    assert(minHeigth != -1);
    verticesHeight[vertex] = minHeigth + 1;
}

void AlgorithmRelabelToFront::pushExcessFlow(Edge& edge)
{
    VertexIndex vertexFrom = edge.getFirstVertexIndex();
    VertexIndex vertexTo = edge.getSecondVertexIndex();
    FlowType deltaFlow = std::min(verticesExcessFlow[vertexFrom], edge.getCapacity() - edge.getFlow());
    network.addEdgeFlow(edge, deltaFlow);
  //todo: find out what is necessary to do with a back edge
    if (edge.getCapacity() != 0)
    {
        network.addEdgeFlow(network.getEdge(vertexTo, vertexFrom, 0), -deltaFlow);
    }
    else
    {
        network.addEdgeFlow(network.getEdge(vertexTo, vertexFrom, 1), -deltaFlow);
    }
    verticesExcessFlow[vertexFrom] -= deltaFlow;
    verticesExcessFlow[vertexTo] += deltaFlow;
}

void AlgorithmRelabelToFront::dischargeVertex(VertexIndex vertex)
{
    while (verticesExcessFlow[vertex] > 0)
    {
        auto adjacentEdge = currentAdjacentEdge[vertex];
        if (adjacentEdge == network.getEdgesListFromVertex(vertex).end())
        {
            relabelVertex(vertex);
            currentAdjacentEdge[vertex] = network.getEdgesListFromVertex(vertex).begin();
        }
        else
        {
            if ((adjacentEdge->getCapacity() - adjacentEdge->getFlow() > 0)
                    && (verticesHeight[vertex] == verticesHeight[adjacentEdge->getSecondVertexIndex()] + 1))
            {
                pushExcessFlow(*adjacentEdge);
            }
            else
            {
                ++currentAdjacentEdge[vertex];
            }
        }
    }
}

