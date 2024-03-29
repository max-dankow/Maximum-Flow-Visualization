#include "visablevertex.h"
#include "math.h"

long VisableVertex::getCenterCoordY() const {
    return centerCoordY;
}

void VisableVertex::setCenterCoordY(long value) {
    centerCoordY = value;
}

long VisableVertex::getCenterCoordX() const {
    return centerCoordX;
}

void VisableVertex::setCenterCoordX(long value) {
    centerCoordX = value;
}

void VisableVertex::move(double deltaX, double deltaY) {
    // Осществляем сдвиг только если в результате не выходим за допустимые границы.
    if (centerCoordX + deltaX < xMinimumLimit || centerCoordX + deltaX > xMaximumLimit) {
        deltaX = 0;
    }
    centerCoordX += deltaX;
    if (centerCoordY + deltaY < yMinimumLimit || centerCoordY + deltaY > yMaximumLimit) {
        deltaY = 0;
    }
    centerCoordY += deltaY;
}

long VisableVertex::getRadius() const {
    return radius;
}

long VisableVertex::getXMinimumLimit() const {
    return xMinimumLimit;
}

long VisableVertex::getXMaximumLimit() const {
    return xMaximumLimit;
}

long VisableVertex::getYMinimumLimit() const {
    return yMinimumLimit;
}

long VisableVertex::getYMaximumLimit() const {
    return yMaximumLimit;
}

VertexIndex VisableVertex::getVertexInGraphIndex() const {
    return vertexInGraphIndex;
}

ForceVector ForceVector::operator-(const ForceVector &forceVector) {
    double xResult = this->xComponent - forceVector.xComponent;
    double yResult = this->yComponent - forceVector.yComponent;
    return ForceVector(xResult, yResult);
}

ForceVector ForceVector::operator+(const ForceVector &forceVector) {
    double xResult = this->xComponent + forceVector.xComponent;
    double yResult = this->yComponent + forceVector.yComponent;
    return ForceVector(xResult, yResult);
}

ForceVector ForceVector::operator*(double value) {
    return ForceVector(xComponent * value, yComponent * value);
}

ForceVector &ForceVector::operator+=(const ForceVector &forceVector) {
    *this = *this + forceVector;
    return *this;
}

ForceVector ForceVector::getNormalVector() const {
    double length = getLength();
    return ForceVector(xComponent / length, yComponent / length);
}

double ForceVector::getLength() const {
    return sqrt(xComponent * xComponent + yComponent * yComponent);
}
