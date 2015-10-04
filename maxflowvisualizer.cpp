#include "maxflowvisualizer.h"
#include <string>
MaxFlowVisualizer::MaxFlowVisualizer(Network network, QWidget* parent)
    : QGLWidget(parent), relabelToFrontAlgo(network), networkPlacer(network)
{
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationStep()));
    resize(900, 600);
    verteciesList.resize(relabelToFrontAlgo.getNetwork().getVerticesNumber());
    std::default_random_engine randomGenerator;
    std::uniform_int_distribution<int> xCoordRandom(200, width() - 10);
    std::uniform_int_distribution<int> yCoordRandom(200, height() - 10);
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex)
    {
        int newCoordX = xCoordRandom(randomGenerator);
        int newCoordY = yCoordRandom(randomGenerator);
        verteciesList[vertex] = VisableVertex(newCoordX, newCoordY);
    }
    animationTimer->start(ANIMATION_STEP_DELAY_MS);
    state = Planarization;
}

void MaxFlowVisualizer::paintEvent(QPaintEvent *e)
{
    // todo: show current network state
    QPainter painter(this);
    showEdges(painter);
    showVertecies(painter);
    painter.drawText(width()-10, height()-10, (std::to_string(verteciesList.size())).c_str());
}
/*
void MaxFlowVisualizer::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Right)
    {
        networkPlacer.doStep(verteciesList);
        update();
    }
}*/

void MaxFlowVisualizer::showVertecies(QPainter &painter)
{
    for (VisableVertex vertex : verteciesList)
    {
        drawVertex(vertex, painter);
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
    // todo: use capacity and flow info to improve quality of the visualization
    QPen pen1(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    pen1.setWidth(3);
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
    //draw arrow ->
    painter.setPen(QPen(Qt::black, 3));
    painter.drawLine(pointFrom, pointTo);
    painter.setPen(QPen(Qt::black, 3));
    painter.translate(pointTo.x(), pointTo.y());
    painter.translate(offsetVector.x(), offsetVector.y());
    painter.rotate(30);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.rotate(-60);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.resetTransform();
}

void MaxFlowVisualizer::animationStep()
{
    switch (state) {
    case Planarization:
        if (networkPlacer.doStep(verteciesList))
        {
            state = Scaling;
        }
        break;
    default:
        break;
    }
    update();
}

