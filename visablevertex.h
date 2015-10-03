#ifndef VISABLEVERTEX_H
#define VISABLEVERTEX_H
#include <QtOpenGL>

class VisableVertex
{
public:
    VisableVertex(GLdouble coordX = 0, GLdouble coordY = 0):
        coordX(coordX), coordY(coordY){}
    GLdouble getCoordY() const;
    void setCoordY(GLdouble value);
    GLdouble getCoordX() const;
    void setCoordX(GLdouble value);

private:
    GLdouble coordX, coordY;
};

#endif // VISABLEVERTEX_H
