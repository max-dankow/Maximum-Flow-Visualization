#include "visablevertex.h"
#include "math.h"

long VisableVertex::getCenterCoordY() const
{
    return centerCoordY;
}

long VisableVertex::getCenterCoordX() const
{
    return centerCoordX;
}

void VisableVertex::move(double deltaX, double deltaY)
{
    centerCoordX += deltaX;
    centerCoordY += deltaY;
}

long VisableVertex::getRadius() const
{
    return radius;
}

void VisableVertex::setRadius(long value)
{
    radius = value;
}

ForceVector ForceVector::operator -(const ForceVector &forceVector)
{
    double xResult = this->xComponent - forceVector.xComponent;
    double yResult = this->yComponent - forceVector.yComponent;
    return ForceVector(xResult, yResult);
}

ForceVector ForceVector::operator +(const ForceVector &forceVector)
{
    double xResult = this->xComponent + forceVector.xComponent;
    double yResult = this->yComponent + forceVector.yComponent;
    return ForceVector(xResult, yResult);
}

ForceVector ForceVector::operator *(double value)
{
    return ForceVector(xComponent * value, yComponent * value);
}

ForceVector &ForceVector::operator +=(const ForceVector &forceVector)
{
    *this = *this + forceVector;
    return *this;
}

ForceVector ForceVector::getNormalVector() const
{
    double length = getLength();
    return ForceVector(xComponent / length, yComponent / length);
}

double ForceVector::getLength() const
{
    return sqrt(xComponent * xComponent + yComponent * yComponent);
}
