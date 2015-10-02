#include <QApplication>
#include <iostream>
#include "mainwindow.h"
#include "algorithmrelabeltofront.h"

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

int main(/*int argc, char *argv[]*/)
{
    freopen("input.txt", "r", stdin);
    Network inputNetwork = readInput();
    AlgorithmRelabelToFront maxFlowSolver(inputNetwork);
    std::cout << maxFlowSolver.calculateMaxFlow() << "\n";
    //readInput();
    /*
    QApplication a(argc, argv);
    MaxFlowVisualizer mainWindow;
    mainWindow.show();

    return a.exec();*/
}
