#include "maxflowvisualizer.h"
#include <QDesktopWidget>
#include <QApplication>
#include <string>
#include <iostream>

MaxFlowVisualizer::MaxFlowVisualizer(Network network, QWidget* parent)
    : QGLWidget(parent), relabelToFrontAlgo(network), networkPlacer(network)
{
    setWindowState(Qt::WindowFullScreen);
    QApplication::desktop()->screenGeometry();
    QRect screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(), screen.height());
    //все вершины располагаются в (0,0), далее уже networkPlacer раскидает их
    verteciesList.resize(relabelToFrontAlgo.getNetwork().getVerticesNumber());
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex)
    {
        int newRadius = VisableVertex::DEFAULT_VERTEX_RADIUS;
        verteciesList[vertex] = VisableVertex(0, 0, newRadius,
                                              newRadius, width() - newRadius,
                                              newRadius, height() - newRadius);
    }
    networkPlacer.throwVerticesRandomly(verteciesList);
    state = Planarization;
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationStep()));
    animationTimer->start(ANIMATION_STEP_DELAY_MS);
}

void MaxFlowVisualizer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    showEdges(painter);
    showVertecies(painter);
}

void MaxFlowVisualizer::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    // todo: разделить анализ нажатий по состояниям визуализатора
    switch (event->key()) {
    case Qt::Key_Right:
        if (state == Planarization)
        {
            networkPlacer.doStep(verteciesList);
            update();
        }
        if (state == AlgoritmRunning)
        {
            if (relabelToFrontAlgo.doStep())
            {
                state = DoNothing;
            }
            update();
        }
        break;
    case Qt::Key_Space:
        if (state == Planarization)
        {
            state = Scaling;
        }
        break;
    case Qt::Key_R:
        //if (state == Planarization)
        //{
            networkPlacer.throwVerticesRandomly(verteciesList);
            state = Planarization;
        //}
        break;
    default:
        break;
    }
}

void MaxFlowVisualizer::mouseDoubleClickEvent(QMouseEvent *e)
{
  QWidget::mouseDoubleClickEvent(e);
  if(isFullScreen())
  {
     setWindowState(Qt::WindowMaximized);
  } else
  {
     setWindowState(Qt::WindowFullScreen);
  }
}

void MaxFlowVisualizer::showVertecies(QPainter &painter)
{
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++ vertex)
    {
        drawVertex(verteciesList[vertex], painter);
    }
}

void MaxFlowVisualizer::showEdges(QPainter &painter)
{
    for (VertexIndex vertex = 0; vertex < relabelToFrontAlgo.getNetwork().getVerticesNumber(); ++vertex)
    {
        auto adjacentEdgesList = relabelToFrontAlgo.getNetwork().getEdgesListFromVertex(vertex);
        for (Edge edge : adjacentEdgesList)
        {
            if (edge.getCapacity() != 0)
            {
                drawEdge(edge, painter);
            }
        }
    }
}

void MaxFlowVisualizer::drawVertex(const VisableVertex &vertex, QPainter& painter) const
{
    QPen pen1(Qt::black, 2);
    painter.setBrush(QBrush(Qt::blue));
    pen1.setWidth(3);
    painter.setPen(pen1);
    QPoint centerPoint(vertex.getCenterCoordX(), vertex.getCenterCoordY());
    painter.drawEllipse(centerPoint, vertex.getRadius(), vertex.getRadius());
}

void MaxFlowVisualizer::drawEdge(const Edge &edge, QPainter &painter) const
{
    QPen pen1;
    pen1.setWidth(3);
    if (relabelToFrontAlgo.getVertexHeight(edge.getFirstVertexIndex())
            == relabelToFrontAlgo.getVertexHeight(edge.getSecondVertexIndex()) + 1)
    {
        pen1.setColor(Qt::black);
    }
    else
    {
        pen1.setColor(Qt::gray);
    }
    if (edge.getCapacity() == edge.getFlow())
    {
        pen1.setColor(Qt::darkBlue);
    }
    //painter.setBrush(QBrush(Qt::lightGray));
    painter.setPen(pen1);
    QPoint pointFrom(verteciesList[edge.getFirstVertexIndex()].getCenterCoordX(),
            verteciesList[edge.getFirstVertexIndex()].getCenterCoordY());
    QPoint pointTo(verteciesList[edge.getSecondVertexIndex()].getCenterCoordX(),
            verteciesList[edge.getSecondVertexIndex()].getCenterCoordY());
    double length = sqrt(pow(pointFrom.x() - pointTo.x(), 2)
                         + pow(pointFrom.y() - pointTo.y(), 2));
    long vertexRaduis = verteciesList[edge.getSecondVertexIndex()].getRadius();
    QPoint offsetVector((pointFrom.x() - pointTo.x()) * vertexRaduis / length,
                        (pointFrom.y() - pointTo.y()) * vertexRaduis / length);
    QPoint arrow((pointFrom.x() - pointTo.x()) * 20 / length,
                        (pointFrom.y() - pointTo.y()) * 20 / length);
    // Нарисовать стрелку (сами маленькие боковые линии)
    painter.setPen(pen1);
    painter.drawLine(pointFrom, pointTo);
    painter.setPen(pen1);
    painter.translate(pointTo.x(), pointTo.y());
    painter.translate(offsetVector.x(), offsetVector.y());
    painter.rotate(30);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.rotate(-60);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.resetTransform();
    QPen penForEdgeInfo;
    if (edge.getCapacity() == edge.getFlow())
    {
        penForEdgeInfo.setColor(Qt::gray);
    }
    else
    {
        penForEdgeInfo.setColor(Qt::darkGreen);
    }
    painter.setPen(penForEdgeInfo);
    std::string edgeInfo = "(" + std::to_string(edge.getFlow()) + " | "+ std::to_string(edge.getCapacity()) + ")";
    painter.drawText(pointFrom.x() + (pointTo.x() - pointFrom.x()) / 2,
                     pointFrom.y() + (pointTo.y() - pointFrom.y()) / 2,
                     edgeInfo.c_str());
}

void MaxFlowVisualizer::animationStep()
{
    switch (state) {
    case Planarization:
        for (int i = 0; i < 10; ++i)
        {
            if (networkPlacer.doStep(verteciesList))
            {
                state = Scaling;
            }
        }
        break;
    case Scaling:
        state = AlgorithmInit;
        break;
    case AlgorithmInit:
        // todo: оставить тут только планаризацию, все остальное вынести на клавиатуру
        relabelToFrontAlgo.init();
        state = AlgoritmRunning;
        break;
    case AlgoritmRunning:
        /*if (relabelToFrontAlgo.doStep())
        {
            state = DoNothing;
        }*/
        break;
    case DoNothing:
        // todo
        break;
    default:
        assert(false);
    }
    update();
}

