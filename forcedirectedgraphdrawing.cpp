#include "forcedirectedgraphdrawing.h"

const double DEFAULT_SPRING_PARAMETER = 0.5;
const double DEFAULT_COULOMB_PARAMETER = 500;
const double DEFAULT_TIME_DELTA = 3;
const double DEFAULT_SPRING_LENGTH = 150;
const double STOP_FORCE_MODULE = 0.025;

static std::random_device rd;
static std::default_random_engine randomGenerator(rd());

void ForceDirectedNetworkDrawing::throwVerticesRandomly(std::vector<VisableVertex> &vertecies)
{
    if (vertecies.size() == 0)
    {
        return;
    }
    // Для отределения диапозона случайных координат берем любую(первую) из вершин,
    // т.к. для остальных допустимые диапозоны такие же
    VisableVertex vertexInstance = vertecies[0];
    std::uniform_int_distribution<int> xCoordRandom(vertexInstance.getXMinimumLimit(),
                                                    vertexInstance.getXMaximumLimit());
    std::uniform_int_distribution<int> yCoordRandom(vertexInstance.getYMinimumLimit(),
                                                    vertexInstance.getYMaximumLimit());
    for (VertexIndex vertex = 0; vertex < vertecies.size(); ++vertex)
    {
        int newCoordX = xCoordRandom(randomGenerator);
        int newCoordY = yCoordRandom(randomGenerator);
        vertecies[vertex].setCenterCoordX(newCoordX);
        vertecies[vertex].setCenterCoordY(newCoordY);
    }
}

void ForceDirectedNetworkDrawing::placeVertecies(std::vector<VisableVertex> &vertecies)
{
    int iterationCounter = 0;
    while (!doStep(vertecies) && iterationCounter < 10000)
    {
        ++iterationCounter;
    };
}

bool ForceDirectedNetworkDrawing::doStep(std::vector<VisableVertex> &vertecies)
{
    std::vector<ForceVector> forces(vertecies.size());
    for (VertexIndex vertexFrom = 0; vertexFrom < vertecies.size(); ++vertexFrom)
    {
        auto adjacentEdgesList = network.getEdgesListFromVertex(vertexFrom);
        for (Edge edge : adjacentEdgesList)
        {
            VertexIndex vertexTo = edge.getSecondVertexIndex();
            forces[vertexFrom] += calculateSpringForce(vertecies[vertexFrom].getCenterCoordX(),
                                                       vertecies[vertexFrom].getCenterCoordY(),
                                                       vertecies[vertexTo].getCenterCoordX(),
                                                       vertecies[vertexTo].getCenterCoordY());
        }
        for (VertexIndex vertexTo = 0; vertexTo < vertecies.size(); ++vertexTo)
        {
            if (vertexFrom == vertexTo)
            {
                continue;
            }
            forces[vertexFrom] += calculateCoulombForce(vertecies[vertexFrom].getCenterCoordX(),
                                                        vertecies[vertexFrom].getCenterCoordY(),
                                                        vertecies[vertexTo].getCenterCoordX(),
                                                        vertecies[vertexTo].getCenterCoordY());
        }
    }
    double maxForceModule;
    for (VertexIndex vertex = 0; vertex < vertecies.size(); ++vertex)
    {
        maxForceModule += forces[vertex].getLength();
        vertecies[vertex].move(timeDelta * forces[vertex].xComponent,timeDelta * forces[vertex].yComponent);
    }
    return maxForceModule < STOP_FORCE_MODULE;
}

ForceVector ForceDirectedNetworkDrawing::calculateSpringForce
(double firstVertexX, double firstVertexY, double secondVertexX, double secondVertexY) const
{
    ForceVector firstVector(firstVertexX, firstVertexY);
    ForceVector secondVector(secondVertexX, secondVertexY);
    ForceVector deltaVector = firstVector - secondVector;
    double springForceModule = springParameter * log(deltaVector.getLength() / normalSpringLength);
    ForceVector result = deltaVector.getNormalVector() * (-1) * springForceModule;
    return result;
}

ForceVector ForceDirectedNetworkDrawing::calculateCoulombForce
(double firstVertexX, double firstVertexY, double secondVertexX, double secondVertexY) const
{
    ForceVector firstVector(firstVertexX, firstVertexY);
    ForceVector secondVector(secondVertexX, secondVertexY);
    ForceVector deltaVector = firstVector - secondVector;
    double coulombForceModule = coulombParameter / (pow(deltaVector.getLength(), 2.0));
    return deltaVector.getNormalVector() * coulombForceModule;
}
