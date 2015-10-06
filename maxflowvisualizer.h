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

class MaxFlowVisualizer : public QWidget
{
    Q_OBJECT
    enum States{
        PLANARIZATION,
        SCALING,
        ALGIRITHM_INIT,
        ALGORITHM_RUN,
        DO_NOTHING
    };

public:
    MaxFlowVisualizer(Network network, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);
private:
    const int ANIMATION_STEP_DELAY_MS = 5;
    States state;
    AlgoAction lastAlgoActioin;
    QTimer *animationTimer;
    std::vector<VisableVertex> verteciesList;
    AlgorithmRelabelToFront relabelToFrontAlgo;
    ForceDirectedNetworkDrawing networkPlacer;
    void showVertecies(QPainter &painter);
    void showEdges(QPainter &painter);
    void drawVertex(const VisableVertex &vertex, QPainter &painter) const;
    void drawEdge(const Edge &edge, QPainter &painter) const;
private slots:
    void animationStep();
};

#endif // MAXFLOWVISUALIZER_H
