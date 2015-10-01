#include "algorithmrelabeltofront.h"

void AlgorithmRelabelToFront::initializePreflow()
{
  //инициализируем Высоту и Избыточный Поток всех вершин
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        verticesExcessFlow[vertex] = 0;
        verticesHeight[vertex] = 0;
    }
  //перебираем все вершины
    for (VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        std::list<Edge>& edges = network.getEdgesListFromVertex(vertex);
      //перебираем все ребра, исходящие из из очередной вершины
        for (auto edgesIt = edges.begin(); edgesIt != edges.end(); ++edgesIt)
        {  
            if (edgesIt->getCapacity() == 0)
            {
                continue;
            }
          //если это ребро исходит из истока, то
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

void AlgorithmRelabelToFront::calculateMaxFlow()
{
    //инициализируем предпоток
    initializePreflow();
    //L - список всех вершин графа, кроме стока и истока
    std::list<VertexIndex> verticesList;
    for(VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        if (vertex != network.getSourceIndex() && vertex != network.getSinkIndex())
        {
            verticesList.push_back(vertex);
        }
    }
    //проинициализируем указатели current каждой вершины current[u] = 1;
    for(VertexIndex vertex = 0; vertex < network.getVerticesNumber(); ++vertex)
    {
        currentAdjacentEdges.push_back(network.getEdgesListFromVertex(vertex).begin());
    }
    //начиная с первой вершины в списке
    auto currentVertexIt = verticesList.begin();
    //пока не null
    while (currentVertexIt != verticesList.end())
    {
        long oldHeight = verticesHeight[*currentVertexIt];
        //разгружаем вершину
        dischargeVertex(*currentVertexIt);
        //если высота изменилась, то перемещаем u в начало списка
        if (verticesHeight[*currentVertexIt] > oldHeight)
        {
            //переместрить вершину в начало
            VertexIndex vertex = *currentVertexIt;
            verticesList.erase(currentVertexIt);
            verticesList.push_front(vertex);
        }
        ++currentVertexIt;
    }
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
    //d = min(e(u), c(u, v) - f(u, v));
    FlowType deltaFlow = std::min(verticesExcessFlow[vertexFrom], edge.getCapacity() - edge.getFlow());
    //    f(u, v) += d;
    network.addEdgeFlow(edge, deltaFlow);
    //    f(v, u) = -f(u, v);
    if (network.hasEdge(vertexTo, vertexFrom, 0))
    {
        network.setEdgeFlow(network.getEdge(vertexTo, vertexFrom, 0), -edge.getFlow());
    }
    //    e(u) -= d;
    verticesExcessFlow[vertexFrom] -= deltaFlow;
    //    e(v) += d;
    verticesExcessFlow[vertexTo] += deltaFlow;
}

void AlgorithmRelabelToFront::dischargeVertex(VertexIndex vertex)
{
    while (verticesExcessFlow[vertex] > 0)
    {
        auto adjacentEdge = currentAdjacentEdges[vertex];
        if (adjacentEdge == network.getEdgesListFromVertex(vertex).end())
        {
            relabelVertex(vertex);
            currentAdjacentEdges[vertex] = network.getEdgesListFromVertex(vertex).begin();
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
                ++currentAdjacentEdges[vertex];
            }
        }
    }
}

