#ifndef VISABLEVERTEX_H
#define VISABLEVERTEX_H
#include <QtOpenGL>

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
    long centerCoordX, centerCoordY, radius;
    //double velocity, acceleration;
};

#endif // VISABLEVERTEX_H
