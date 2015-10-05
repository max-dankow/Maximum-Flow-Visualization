#ifndef FORCEDIRECTEDGRAPHDRAWING_H
#define FORCEDIRECTEDGRAPHDRAWING_H
#include "network.h"
#include "visablevertex.h"

extern const double DEFAULT_SPRING_PARAMETER;
extern const double DEFAULT_COULOMB_PARAMETER;
extern const double DEFAULT_TIME_DELTA;
extern const double DEFAULT_SPRING_LENGTH;

class ForceDirectedNetworkDrawing
{
public:
    ForceDirectedNetworkDrawing(const Network &network,
                                double springParameter = DEFAULT_SPRING_PARAMETER,
                                double normalSpringLength = DEFAULT_SPRING_LENGTH,
                                double coulombParameter = DEFAULT_COULOMB_PARAMETER,
                                double timeDelta = DEFAULT_TIME_DELTA) :
        network(network), springParameter(springParameter),
        normalSpringLength(normalSpringLength),
        coulombParameter(coulombParameter),
        timeDelta(timeDelta){}
    void throwVerticesRandomly(std::vector<VisableVertex> &vertecies);
    void placeVertecies(std::vector<VisableVertex> &vertecies);
    bool doStep(std::vector<VisableVertex> &vertecies);
private:
    Network network;
    double springParameter, normalSpringLength, coulombParameter, timeDelta;
    ForceVector calculateSpringForce(double firstVertexX, double firstVertexY, double secondVertexX, double secondVertexY) const;
    ForceVector calculateCoulombForce(double firstVertexX, double firstVertexY, double secondVertexX, double secondVertexY) const;
};

#endif // FORCEDIRECTEDGRAPHDRAWING_H
