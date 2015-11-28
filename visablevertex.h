#ifndef VISABLEVERTEX_H
#define VISABLEVERTEX_H

#include "algorithmrelabeltofront.h"

struct ForceVector {
    ForceVector(double xComponent = 0, double yComponent = 0) :
            xComponent(xComponent), yComponent(yComponent) { }

    ForceVector operator-(const ForceVector &forceVector);

    ForceVector operator+(const ForceVector &forceVector);

    ForceVector operator*(double value);

    ForceVector &operator+=(const ForceVector &forceVector);

    ForceVector getNormalVector() const;

    double getLength() const;

    double xComponent, yComponent;
};

class VisableVertex {
public:
    static const int DEFAULT_VERTEX_RADIUS = 15;

    VisableVertex(VertexIndex vertexIndex, long centerCoordX, long centerCoordY,
                  long radius, long xMinimumLimit, long xMaximumLimit,
                  long yMinimumLimit, long yMaximumLimit) :
            vertexInGraphIndex(vertexIndex),
            centerCoordX(centerCoordX),
            centerCoordY(centerCoordY),
            radius(radius),
            xMinimumLimit(xMinimumLimit),
            xMaximumLimit(xMaximumLimit),
            yMinimumLimit(yMinimumLimit),
            yMaximumLimit(yMaximumLimit) { }

    VisableVertex(const VisableVertex &visableVertex) = default;

    VisableVertex() { }

    long getCenterCoordX() const;

    long getCenterCoordY() const;

    void setCenterCoordX(long value);

    void setCenterCoordY(long value);

    void move(double deltaX, double deltaY);

    long getRadius() const;

    long getXMinimumLimit() const;

    long getXMaximumLimit() const;

    long getYMinimumLimit() const;

    long getYMaximumLimit() const;

    VertexIndex getVertexInGraphIndex() const;

private:
    VertexIndex vertexInGraphIndex;
    long centerCoordX, centerCoordY, radius;
    long xMinimumLimit, xMaximumLimit, yMinimumLimit, yMaximumLimit;
};

#endif // VISABLEVERTEX_H
