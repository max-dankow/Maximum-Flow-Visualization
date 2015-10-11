#include <QApplication>
#include <QtCore>
#include <QtGui>
#include <iostream>
#include "maxflowvisualizer.h"
#include "algorithmrelabeltofront.h"
#include <random>

Network readInput()
{
    size_t verticesNumber, egdesNumber;
    std::cin >> verticesNumber >> egdesNumber;
    Network inputNetwork(verticesNumber, 0, verticesNumber - 1);
    for (size_t i = 0; i < egdesNumber; ++i)
    {
        VertexIndex firstVertex, secondVertex;
        FlowType capacity;
        std::cin >> firstVertex >> secondVertex >> capacity;
        inputNetwork.addEdge(Edge(firstVertex - 1, secondVertex - 1, 1, capacity, 0));
        inputNetwork.addEdge(Edge(secondVertex - 1, firstVertex - 1, 1, 0, 0));
    }
    return inputNetwork;
}

std::random_device rd;
std::default_random_engine randomGenerator(rd());

Network generateRandomNetwork()
{
    std::uniform_int_distribution<int> verteciesNumberRandom(10, 20);
    std::uniform_int_distribution<int> edgesNumberRandom(20, 30);
    std::uniform_int_distribution<FlowType> capacityRandom(1, 20);
    int verteciesNumber = verteciesNumberRandom(randomGenerator);
    int edgesNumber = edgesNumberRandom(randomGenerator);
    std::uniform_int_distribution<VertexIndex> vertexRandom(0, verteciesNumber - 1);
    Network network(verteciesNumber, 0, verteciesNumber - 1);

    for (int i = 0; i < edgesNumber; ++i)
    {
        VertexIndex vertexFrom = vertexRandom(randomGenerator);
        VertexIndex vertexTo = vertexRandom(randomGenerator);
        FlowType capacity = capacityRandom(randomGenerator);
        if (vertexFrom != vertexTo && !network.hasEdge(vertexFrom, vertexTo, capacity) && !network.hasEdge(vertexTo, vertexFrom, capacity))
        {
            network.addEdge(Edge(vertexFrom, vertexTo, 1, capacity, 0));
            network.addEdge(Edge(vertexTo, vertexFrom, 1, 0, 0));
        }
    }
    AlgorithmRelabelToFront maxFlowSolver(network);
    if (maxFlowSolver.calculateMaxFlow() != 0)
    {
        return network;
    }
    else
    {
        return generateRandomNetwork();
    }
}

int main(int argc, char *argv[])
{
    freopen("input.txt", "r", stdin);
    Network inputNetwork = readInput();
    AlgorithmRelabelToFront maxFlowSolver(inputNetwork);
    std::cout << maxFlowSolver.calculateMaxFlow() << "\n";

    QApplication a(argc, argv);
    MaxFlowVisualizer mainWindow(generateRandomNetwork());
    mainWindow.show();

    return a.exec();
}
