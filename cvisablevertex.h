#ifndef CVISABLEVERTEX_H
#define CVISABLEVERTEX_H
#include <QtOpenGL>

class CVertexGL
{
public:
    CVertexGL(GLdouble coordX, GLdouble coordY):
        coordX(coordX), coordY(coordY){}
    GLdouble getCoordY() const;
    void setCoordY(GLdouble value);

    GLdouble getCoordX() const;
    void setCoordX(GLdouble value);

private:
    GLdouble coordX, coordY;
};

#endif // CVISABLEVERTEX_H
