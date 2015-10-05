#ifndef VISABLEVERTEX_H
#define VISABLEVERTEX_H

struct ForceVector{
    ForceVector(double xComponent = 0, double yComponent = 0) :
        xComponent(xComponent), yComponent(yComponent){}
    ForceVector operator -(const ForceVector &forceVector);
    ForceVector operator +(const ForceVector &forceVector);
    ForceVector operator *(double value);
    ForceVector& operator +=(const ForceVector &forceVector);
    ForceVector getNormalVector() const;
    double getLength() const;
    double xComponent, yComponent;
};

class VisableVertex
{
public:
    static const int DEFAULT_VERTEX_RADIUS = 15;
    VisableVertex(long centerCoordX, long centerCoordY, long radius, long xMinimumLimit,
                  long xMaximumLimit, long yMinimumLimit, long yMaximumLimit):
        centerCoordX(centerCoordX), centerCoordY(centerCoordY), radius(radius),
        xMinimumLimit (xMinimumLimit), xMaximumLimit (xMaximumLimit),
        yMinimumLimit(yMinimumLimit), yMaximumLimit(yMaximumLimit){}
    VisableVertex(){}
    long getCenterCoordY() const;
    void setCenterCoordY(double value);
    long getCenterCoordX() const;
    void setCenterCoordX(double value);
    void move(double deltaX, double deltaY);
    long getRadius() const;
    void setRadius(long value);
    long getXMinimumLimit() const;
    long getXMaximumLimit() const;
    long getYMinimumLimit() const;
    long getYMaximumLimit() const;

private:
    double centerCoordX, centerCoordY, radius;
    long xMinimumLimit, xMaximumLimit, yMinimumLimit, yMaximumLimit;
};

#endif // VISABLEVERTEX_H
