#include "maxflowvisualizer.h"

MaxFlowVisualizer::MaxFlowVisualizer(Network network, QWidget* parent)
    : QGLWidget(parent), relabelToFrontAlgo(network)
{
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationStep()));
    resize(900, 600);
    x = 300;
    y = 200;
    // todo: init visable vertex positions
    verteciesList.resize(relabelToFrontAlgo.getNetwork().getVerticesNumber());
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex)
    {

    }
    animationTimer->start(ANIMATION_STEP_DELAY_MS);
}

void MaxFlowVisualizer::paintEvent(QPaintEvent *e)
{
    // todo: show current network state
    QPainter painter(this);
    drawVertex(VisableVertex(100 + x, 100), painter);
    drawVertex(VisableVertex(550 - x, 200), painter);
}

void MaxFlowVisualizer::drawVertex(const VisableVertex &vertex, QPainter& painter) const
{
    const int VERTEX_RADIUS = 30;
    QPen pen1(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    pen1.setWidth(3);
    painter.setPen(pen1);
    painter.drawEllipse(vertex.getCoordX(), vertex.getCoordY(), VERTEX_RADIUS, VERTEX_RADIUS);
}

void MaxFlowVisualizer::drawEdge(const Edge &edge, QPainter &painter) const
{
    // todo: use capacity and flow info to improve quality of the visualization
    QPen pen1(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    pen1.setWidth(3);
    painter.setPen(pen1);
    painter.drawLine(verteciesList[edge.getFirstVertexIndex()].getCoordX(),
            verteciesList[edge.getFirstVertexIndex()].getCoordY(),
            verteciesList[edge.getSecondVertexIndex()].getCoordX(),
            verteciesList[edge.getSecondVertexIndex()].getCoordY());
}

void MaxFlowVisualizer::animationStep()
{
    //todo: update flow network state: a step of algo
    x++;
    update();
}
