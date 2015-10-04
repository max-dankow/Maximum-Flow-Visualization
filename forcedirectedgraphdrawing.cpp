#include "forcedirectedgraphdrawing.h"

const double DEFAULT_SPRING_PARAMETER = 1;
const double DEFAULT_COULOMB_PARAMETER = 400;
const double DEFAULT_TIME_DELTA = 10;
const double DEFAULT_SPRING_LENGTH = 200;

void ForceDirectedNetworkDrawing::placeVertecies(std::vector<VisableVertex> &vertecies)
{
    while (isNetworkStable())
    {
        doStep(vertecies);
    }
}

void ForceDirectedNetworkDrawing::doStep(std::vector<VisableVertex> &vertecies)
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
    for (VertexIndex vertex = 0; vertex < vertecies.size(); ++vertex)
    {
        vertecies[vertex].move(timeDelta * forces[vertex].xComponent,timeDelta * forces[vertex].yComponent);
    }
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

bool ForceDirectedNetworkDrawing::isNetworkStable()
{
    // todo: checking by kinetic energy sum
    return false;
}
