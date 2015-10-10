#include "maxflowvisualizer.h"
#include <QDesktopWidget>
#include <QApplication>
#include <string>
#include <iostream>

MaxFlowVisualizer::MaxFlowVisualizer(Network network, QWidget* parent)
    : QWidget(parent), relabelToFrontAlgo(network), networkPlacer(network)
{
    setWindowState(Qt::WindowFullScreen);
    QRect screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(), screen.height());
    //все вершины располагаются в (0,0), далее уже networkPlacer раскидает их
    verteciesList.resize(relabelToFrontAlgo.getNetwork().getVerticesNumber());
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex)
    {
        int newRadius = VisableVertex::DEFAULT_VERTEX_RADIUS;
        verteciesList[vertex] = VisableVertex(vertex, 0, 0, newRadius,
                                              newRadius, width() - RIGHT_BAR_OF_HEIGHTS_WIDTH - VisableVertex::DEFAULT_VERTEX_RADIUS,
                                              newRadius, height() - newRadius);
    }
    networkPlacer.throwVerticesRandomly(verteciesList);
    state = PLANARIZATION;
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationStep()));
    animationTimer->start(ANIMATION_STEP_DELAY_MS);
    setMouseTracking(true);
}

void MaxFlowVisualizer::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    showEdges(painter);
    showVertecies(painter);
    drawHeightsBar(painter);
}

void MaxFlowVisualizer::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    // todo: разделить анализ нажатий по состояниям визуализатора
    switch (event->key()) {
    case Qt::Key_Right:
        if (state == PLANARIZATION)
        {
            networkPlacer.doStep(verteciesList);
            update();
        }
        if (state == ALGORITHM_RUN)
        {
            lastAlgoAction = relabelToFrontAlgo.doStep();
            if (lastAlgoAction.getType() == AlgoAction::ACTION_TERMINATE)
            {
                state = DO_NOTHING;
            }
            update();
        }
        break;
    case Qt::Key_Space:
        if (state == PLANARIZATION)
        {
            state = SCALING;
        }
        break;
    case Qt::Key_R:
        //if (state == Planarization)
        //{
            networkPlacer.throwVerticesRandomly(verteciesList);
            state = PLANARIZATION;
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

void MaxFlowVisualizer::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    rememberState = state;
    vertexToDrag = getVertexUnderCursor(event->pos());
    state = VERTEX_DRAGING;
}

void MaxFlowVisualizer::mouseMoveEvent(QMouseEvent *event)
{
    QWidget::mouseMoveEvent(event);
    if (state == VERTEX_DRAGING)
    {
        if (vertexToDrag != verteciesList.size())
        {
            double xDelta = event->pos().x() - verteciesList[vertexToDrag].getCenterCoordX();
            double yDelta = event->pos().y() - verteciesList[vertexToDrag].getCenterCoordY();
            verteciesList[vertexToDrag].move(xDelta, yDelta);
            update();
        }
    }
}

void MaxFlowVisualizer::mouseReleaseEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    state = rememberState;
}

void MaxFlowVisualizer::drawHeightsBar(QPainter &painter)
{
    painter.save();
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::lightGray);
    int borderOffset = VisableVertex::DEFAULT_VERTEX_RADIUS;
    painter.translate(width() - RIGHT_BAR_OF_HEIGHTS_WIDTH, borderOffset);
    unsigned barWidth = RIGHT_BAR_OF_HEIGHTS_WIDTH;
    int verteciesNumber = verteciesList.size();
    int barHeight = height() - 2 * borderOffset;
    int bottomY = barHeight;
    double scaleHeight = barHeight / ( 2 * verteciesNumber);
    for (size_t level = 0; level < 2 * verteciesNumber + 1; ++level)
    {
        int x = 0;
        painter.drawLine(x,
                         bottomY - scaleHeight * level,
                         x + barWidth,
                         bottomY - scaleHeight * level);
    }
    double scaleWidth = RIGHT_BAR_OF_HEIGHTS_WIDTH / verteciesNumber;
    for (size_t vertex = 0; vertex < verteciesNumber; ++vertex)
    {
        int x = borderOffset;
        VisableVertex currentVertex = verteciesList[vertex];
        currentVertex.setCenterCoordX(x + scaleWidth * vertex);
        currentVertex.setCenterCoordY(bottomY - scaleHeight * relabelToFrontAlgo.getVertexHeight(vertex));
        drawVertex(currentVertex, painter);
    }
    painter.restore();
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

void MaxFlowVisualizer::drawVertex(const VisableVertex &vertex, QPainter& painter)
{
    QPen pen1(Qt::black, 2);
    if (lastAlgoAction.getType() == AlgoAction::ACTION_SELECT
        && lastAlgoAction.getVertexInfo() == vertex.getVertexInGraphIndex())
    {
        pen1.setColor(Qt::red);
    }
    painter.setBrush(QBrush(Qt::blue));
    if (relabelToFrontAlgo.getNetwork().getSourceIndex() == vertex.getVertexInGraphIndex())
    {
        painter.setBrush(QBrush(Qt::green));
    }
    if (relabelToFrontAlgo.getNetwork().getSinkIndex() == vertex.getVertexInGraphIndex())
    {
        painter.setBrush(QBrush(Qt::red));
    }
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
    if (lastAlgoAction.getType() == AlgoAction::ACTION_PUSH && lastAlgoAction.getEdgeInfo() == edge)
    {
        pen1.setColor(Qt::red);
    }
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
    if (state == ALGORITHM_RUN || state == DO_NOTHING)
    {
        penForEdgeInfo.setColor(Qt::darkGreen);
        painter.setPen(penForEdgeInfo);
        std::string edgeInfo = "(" + std::to_string(edge.getFlow()) + " | "+ std::to_string(edge.getCapacity()) + ")";
        painter.drawText(pointFrom.x() + (pointTo.x() - pointFrom.x()) / 2,
                         pointFrom.y() + (pointTo.y() - pointFrom.y()) / 2,
                         edgeInfo.c_str());
    }
}

/* По координатам курсора получает уникальный номер вершины, которая
 * попадает под него. В противном случае возвращает номер вешины,
 * следующей за последней в списке вершин (то есть verticesList.size()).
 */
VertexIndex MaxFlowVisualizer::getVertexUnderCursor(QPoint cursorPosition)
{
    QVector2D cursor(cursorPosition);
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex)
    {
        QVector2D currentVertexCenter(verteciesList[vertex].getCenterCoordX(), verteciesList[vertex].getCenterCoordY());
        QVector2D distanceVector(currentVertexCenter - cursor);
        if (distanceVector.length() < verteciesList[vertex].getRadius())
        {
            return vertex;
        }
    }
    return verteciesList.size();
}

void MaxFlowVisualizer::animationStep()
{
    switch (state) {
    case PLANARIZATION:
        for (int i = 0; i < 10; ++i)
        {
            if (networkPlacer.doStep(verteciesList))
            {
                state = SCALING;
            }
        }
        break;
    case SCALING:
        state = ALGIRITHM_INIT;
        break;
    case ALGIRITHM_INIT:
        // todo: оставить тут только планаризацию, все остальное вынести на клавиатуру
        lastAlgoAction = relabelToFrontAlgo.init();
        state = ALGORITHM_RUN;
        break;
    case ALGORITHM_RUN:
        /*if (relabelToFrontAlgo.doStep())
        {
            state = DoNothing;
        }*/
        break;
    case VERTEX_DRAGING:
        // todo ))))))
        break;
    case DO_NOTHING:
        // todo
        break;
    default:
        assert(false);
    }
    update();
}

