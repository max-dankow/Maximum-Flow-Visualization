#ifndef MAXFLOWVISUALIZER_H
#define MAXFLOWVISUALIZER_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGLWidget>
#include <QTime>
#include "visablevertex.h"
#include "algorithmrelabeltofront.h"
#include <vector>

class MaxFlowVisualizer : public QGLWidget
{
    Q_OBJECT
public:
    MaxFlowVisualizer(Network network, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
private:
    const int ANIMATION_STEP_DELAY_MS = 5;
    QTimer *animationTimer;
    void drawVertex(const VisableVertex &vertex, QPainter &painter) const;
    void drawEdge(const Edge &edge, QPainter &painter) const;
    std::vector<VisableVertex> verteciesList;
    AlgorithmRelabelToFront relabelToFrontAlgo;
    long x, y;
private slots:
    void animationStep();
};

#endif // MAXFLOWVISUALIZER_H
