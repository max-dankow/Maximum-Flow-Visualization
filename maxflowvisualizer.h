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
        // Состояние пошагового исполнения силового алгоритма рамещения вершин.
        PLANARIZATION,
        // Состояние перетаксивания вершины мышкой.
        VERTEX_DRAGING,
        // Единажды вызывается инициализация данных алгоритма.
        ALGIRITHM_INIT,
        // Состояние пошагового исполнения самого алгоритма и визуализации.
        ALGORITHM_RUN,
        // Состояние простого отображения сети без выполнения действий.
        DO_NOTHING
    };

public:
    MaxFlowVisualizer(Network network, QWidget *parent = 0);
protected:
    void paintEvent(QPaintEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    const int ANIMATION_STEP_DELAY_MS = 5;
    // Ширина диаграммы высот вершин (справа).
    const int RIGHT_BAR_OF_HEIGHTS_WIDTH = 500;
    States state;
    // Используется для восстановления состояния state визуализатора
    // после перетаскивания вершины мышкой.
    States rememberState;
    unsigned algoStepsCount;
    AlgoAction lastAlgoAction;
    QTimer *animationTimer;
    std::vector<VisableVertex> verteciesList;
    AlgorithmRelabelToFront relabelToFrontAlgo;
    // Объект класса механизма, красиво размещающего вершины графа на плоскости.
    ForceDirectedNetworkDrawing networkPlacer;
    // Рисует справа диаграмму с высотами вершин.
    void drawHeightsBar(QPainter &painter);
    void showVertecies(QPainter &painter);
    void showEdges(QPainter &painter);
    void drawVertex(const VisableVertex &vertex, QPainter &painter);
    void drawEdge(const Edge &edge, QPainter &painter) const;
    VertexIndex getVertexUnderCursor(QPoint cursorPosition);
    // Уникальный номер вершины, перетаскиваемой мышкой.
    VertexIndex vertexToDrag;
private slots:
    void animationStep();
};

#endif // MAXFLOWVISUALIZER_H
