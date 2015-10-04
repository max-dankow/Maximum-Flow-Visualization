#ifndef MAXFLOWVISUALIZER_H
#define MAXFLOWVISUALIZER_H

#include <QMainWindow>
#include <QtCore>
#include <QtGui>
#include <QGLWidget>
#include <QTime>
#include <vector>
#include <random>
#include <math.h>
#include "visablevertex.h"
#include "algorithmrelabeltofront.h"
#include "forcedirectedgraphdrawing.h"

class MaxFlowVisualizer : public QGLWidget
{
    Q_OBJECT
public:
    MaxFlowVisualizer(Network network, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
    //void keyPressEvent(QKeyEvent *event);
private:
    const int ANIMATION_STEP_DELAY_MS = 5;
    QTimer *animationTimer;
    void showVertecies(QPainter &painter);
    void showEdges(QPainter &painter);
    void drawVertex(const VisableVertex &vertex, QPainter &painter) const;
    void drawEdge(const Edge &edge, QPainter &painter) const;
    std::vector<VisableVertex> verteciesList;
    AlgorithmRelabelToFront relabelToFrontAlgo;
    ForceDirectedNetworkDrawing networkPlacer;
private slots:
    void animationStep();
};

#endif // MAXFLOWVISUALIZER_H
