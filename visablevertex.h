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
    VisableVertex(long centerCoordX = 0, long centerCoordY = 0, long radius = DEFAULT_VERTEX_RADIUS):
        centerCoordX(centerCoordX), centerCoordY(centerCoordY), radius(radius){}
    long getCenterCoordY() const;
    //void setCenterCoordY(long value);
    long getCenterCoordX() const;
    void move(double deltaX, double deltaY);
    //void setCenterCoordX(long value);
    long getRadius() const;
    void setRadius(long value);
private:
    static const int DEFAULT_VERTEX_RADIUS = 15;
    double centerCoordX, centerCoordY, radius;
};

#endif // VISABLEVERTEX_H
