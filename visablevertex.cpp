#include "visablevertex.h"

long VisableVertex::getCenterCoordY() const
{
    return centerCoordY;
}

/*void VisableVertex::setCenterCoordY(long value)
{
    centerCoordY = value;
}*/
long VisableVertex::getCenterCoordX() const
{
    return centerCoordX;
}

void VisableVertex::move(double deltaX, double deltaY)
{
    centerCoordX += deltaX;
    centerCoordY += deltaY;
}

/*void VisableVertex::setCenterCoordX(long value)
{
    centerCoordX = value;
}*/

long VisableVertex::getRadius() const
{
    return radius;
}

void VisableVertex::setRadius(long value)
{
    radius = value;
}
