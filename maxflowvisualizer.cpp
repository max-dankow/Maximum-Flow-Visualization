#include "maxflowvisualizer.h"
#include <QDesktopWidget>
#include <QApplication>
#include <string>
#include <iostream>

MaxFlowVisualizer::MaxFlowVisualizer(Network network, QWidget *parent)
        : QWidget(parent), relabelToFrontAlgo(network), networkPlacer(network) {
    setWindowState(Qt::WindowFullScreen);
    QRect screen = QApplication::desktop()->screenGeometry();
    resize(screen.width(), screen.height());
    //все вершины располагаются в (0,0), далее уже networkPlacer раскидает их
    verteciesList.resize(relabelToFrontAlgo.getNetwork().getVerticesNumber());
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex) {
        int newRadius = VisableVertex::DEFAULT_VERTEX_RADIUS;
        verteciesList[vertex] = VisableVertex(vertex, 0, 0, newRadius,
                                              newRadius, width() - RIGHT_BAR_OF_HEIGHTS_WIDTH -
                                                         VisableVertex::DEFAULT_VERTEX_RADIUS,
                                              newRadius, height() - newRadius);
    }
    networkPlacer.throwVerticesRandomly(verteciesList);
    state = PLANARIZATION;
    algoStepsCount = 0;
    animationTimer = new QTimer(this);
    connect(animationTimer, SIGNAL(timeout()), this, SLOT(animationStep()));
    animationTimer->start(ANIMATION_STEP_DELAY_MS);
    setMouseTracking(true);
}

void MaxFlowVisualizer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    // Включает сглаживание.
    painter.setRenderHint(QPainter::Antialiasing);
    showEdges(painter);
    showVertecies(painter);
    drawHeightsBar(painter);
}

void MaxFlowVisualizer::keyPressEvent(QKeyEvent *event) {
    QWidget::keyPressEvent(event);
    switch (event->key()) {
        case Qt::Key_Left:
            // Oткат алгоритма на один элементарный шаг назад.
            // Т.к. даже для очень больших графов сам алгоритм работает меньше секунды,
            // а визуализатор расчитан на сравнительно маленькие графы (около 30 вершин),
            // то перезапуск алгоритма с начала до n-1 шага будет эффективнен и надежен.
            if ((state == ALGORITHM_RUN || state == ALGORITHM_TERM) && algoStepsCount > 0) {
                relabelToFrontAlgo.init();
                --algoStepsCount;
                lastAlgoAction = relabelToFrontAlgo.doSteps(algoStepsCount);
                if (lastAlgoAction.getType() == AlgoAction::ACTION_TERMINATE) {
                    state = ALGORITHM_TERM;
                }
                else {
                    state = ALGORITHM_RUN;
                }
                update();
            }
            break;
        case Qt::Key_Right:
            // Выполняет один элементарный шаг алгоритма.
            if (state == ALGORITHM_RUN) {
                lastAlgoAction = relabelToFrontAlgo.doStep();
                ++algoStepsCount;
                if (lastAlgoAction.getType() == AlgoAction::ACTION_TERMINATE) {
                    state = ALGORITHM_TERM;
                }
                update();
            }
            break;
        case Qt::Key_Space:
            // Остановка размещения графа.
            if (state == PLANARIZATION) {
                state = ALGIRITHM_INIT;
            }
            break;
        case Qt::Key_R:
            // Полный перезапуск замещения графа.
            networkPlacer.throwVerticesRandomly(verteciesList);
            state = PLANARIZATION;
            break;
    }
}

// Просто переключение между полноэкранным режимом и максимимзацией окна.
void MaxFlowVisualizer::mouseDoubleClickEvent(QMouseEvent *event) {
    QWidget::mouseDoubleClickEvent(event);
    if (isFullScreen()) {
        setWindowState(Qt::WindowMaximized);
    } else {
        setWindowState(Qt::WindowFullScreen);
    }
}

// При нажатии клавиши мыши может начаться перетаскивание вершины.
// Обработчик анализирует, на какую вершину попадает курсор и,
// если он попадает на вершину, то текущее состояние системы запоминается
// и визуализаор переходит в состояние перетаскивания вершины.
void MaxFlowVisualizer::mousePressEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    rememberState = state;
    vertexToDrag = getVertexUnderCursor(event->pos());
    if (vertexToDrag != verteciesList.size()) {
        state = VERTEX_DRAGING;
    }
}

// Обработчик этого события отвечает только за сдвиг перетаскиваемой вершины,
// если система находится соответственно в состоянии перетаскивания.
void MaxFlowVisualizer::mouseMoveEvent(QMouseEvent *event) {
    QWidget::mouseMoveEvent(event);
    if (state == VERTEX_DRAGING) {
        if (vertexToDrag != verteciesList.size()) {
            double xDelta = event->pos().x() - verteciesList[vertexToDrag].getCenterCoordX();
            double yDelta = event->pos().y() - verteciesList[vertexToDrag].getCenterCoordY();
            verteciesList[vertexToDrag].move(xDelta, yDelta);
            update();
        }
    }
}

// Отвечает за завершение перетаскивания вершины при отпускании клавиши мыши.
// Восстанавливает состояние системы на момент нажатия мыши.
void MaxFlowVisualizer::mouseReleaseEvent(QMouseEvent *event) {
    QWidget::mousePressEvent(event);
    state = rememberState;
}

// Считает сколько уровней высот вершин следует реально отображать
// (максимальный занятый + 1).
int MaxFlowVisualizer::countUsedHeightLevels() const {
    size_t verteciesNumber = verteciesList.size();
    int levelCount = 0;
    for (VertexIndex vertex = 0; vertex < verteciesNumber; ++vertex) {
        int vertexHeight = relabelToFrontAlgo.getVertexHeight(vertex);
        if (levelCount < vertexHeight) {
            levelCount = vertexHeight;
        }
    }
    ++levelCount;
    return levelCount;
}

// Рисует панель высот вершин (справа).
void MaxFlowVisualizer::drawHeightsBar(QPainter &painter) {
    painter.save();
    painter.setPen(QPen(Qt::black, 3));
    painter.setBrush(Qt::lightGray);
    size_t verteciesNumber = verteciesList.size();
    int levelCount = countUsedHeightLevels();
    int borderOffset = VisableVertex::DEFAULT_VERTEX_RADIUS;
    // Для удобства перенесем систему коодинат в левый верхний угол будующей панели.
    painter.translate(width() - RIGHT_BAR_OF_HEIGHTS_WIDTH, borderOffset);
    unsigned barWidth = RIGHT_BAR_OF_HEIGHTS_WIDTH;
    int barHeight = height() - 2 * borderOffset;
    int bottomY = barHeight;
    double scaleHeight = barHeight / levelCount;
    // Рисует горизонтальные линии, соответсвующие уровням.
    for (size_t level = 0; level < levelCount; ++level) {
        int x = 0;
        painter.drawLine(x,
                         bottomY - scaleHeight * level,
                         x + barWidth,
                         bottomY - scaleHeight * level);
    }
    // Отображение вершин на панели высот.
    double scaleWidth = RIGHT_BAR_OF_HEIGHTS_WIDTH / verteciesNumber;
    for (size_t vertex = 0; vertex < verteciesNumber; ++vertex) {
        // Для того, чтобы все визуальные эффекты вершины (выделение, надписи и т.д.)
        // отображались и на боковой панели высот, мы в явном виде создаем новую
        // вершину - точную копию исходной (простой конструктор копирования)
        // и перемещаем ее на нужное место на панели высот.
        VisableVertex currentVertex(verteciesList[vertex]);
        currentVertex.setCenterCoordX(borderOffset + scaleWidth * vertex);
        currentVertex.setCenterCoordY(bottomY - scaleHeight * relabelToFrontAlgo.getVertexHeight(vertex));
        drawVertex(currentVertex, painter);
    }
    painter.restore();
}

void MaxFlowVisualizer::showVertecies(QPainter &painter) {
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex) {
        drawVertex(verteciesList[vertex], painter);
    }
}

void MaxFlowVisualizer::showEdges(QPainter &painter) {
    for (VertexIndex vertex = 0; vertex < relabelToFrontAlgo.getNetwork().getVerticesNumber(); ++vertex) {
        auto adjacentEdgesList = relabelToFrontAlgo.getNetwork().getEdgesListFromVertex(vertex);
        for (Edge edge : adjacentEdgesList) {
            if (edge.getCapacity() != 0) {
                drawEdge(edge, painter);
            }
        }
    }
}

void MaxFlowVisualizer::drawVertex(const VisableVertex &vertex, QPainter &painter) {
    VertexIndex vertexIndex = vertex.getVertexInGraphIndex();
    QPen pen(Qt::black, 2);
    // Если вершина была задействована в последнем шаге алгоритма
    // то выделяем ее красным цветом.
    if (((lastAlgoAction.getType() == AlgoAction::ACTION_SELECT
          || lastAlgoAction.getType() == AlgoAction::ACTION_RELABEL)
         && lastAlgoAction.getVertexInfo() == vertexIndex)) {
        pen.setColor(Qt::red);
    }
    if (((lastAlgoAction.getType() == AlgoAction::ACTION_PUSH)
         && (lastAlgoAction.getEdgeInfo().getFirstVertexIndex() == vertexIndex
             || lastAlgoAction.getEdgeInfo().getSecondVertexIndex() == vertexIndex))) {
        pen.setColor(Qt::red);
    }
    // Если в вершине есть избыток предпотока, то отметим ее синей заливкой.
    if (relabelToFrontAlgo.getVertexExcessFlow(vertexIndex) != 0) {
        painter.setBrush(QBrush(Qt::blue));
    }
    else {
        painter.setBrush(QBrush(Qt::lightGray));
    }
    // Исток заливаем зеленым, сток - красным.
    if (relabelToFrontAlgo.getNetwork().getSourceIndex() == vertexIndex) {
        painter.setBrush(QBrush(Qt::green));
    }
    if (relabelToFrontAlgo.getNetwork().getSinkIndex() == vertexIndex) {
        painter.setBrush(QBrush(Qt::red));
    }
    pen.setWidth(3);
    painter.setPen(pen);
    QPoint centerPoint(vertex.getCenterCoordX(), vertex.getCenterCoordY());
    painter.drawEllipse(centerPoint, vertex.getRadius(), vertex.getRadius());
    // В центре вершины выводим величину избыточного потока.
    std::string excessFlowStr = std::to_string(relabelToFrontAlgo.getVertexExcessFlow(vertexIndex));
    painter.drawText(vertex.getCenterCoordX() - vertex.getRadius() / 2,
                     vertex.getCenterCoordY() + vertex.getRadius() / 4,
                     excessFlowStr.c_str());
}

void MaxFlowVisualizer::drawEdge(const Edge &edge, QPainter &painter) const {
    QPen pen;
    pen.setWidth(3);
    // Ребро, которое считается "допустимым" по алгоритму, выделяем черным.
    if (relabelToFrontAlgo.getVertexHeight(edge.getFirstVertexIndex())
        == relabelToFrontAlgo.getVertexHeight(edge.getSecondVertexIndex()) + 1) {
        pen.setColor(Qt::black);
    }
    else {
        pen.setColor(Qt::gray);
    }
    if (edge.getFlow() != 0) {
        pen.setColor(Qt::darkBlue);
    }
    // Если в последнем действии алгоритма произошло проталкивание по этому ребру,
    // то выделим его красным.
    if (lastAlgoAction.getType() == AlgoAction::ACTION_PUSH && lastAlgoAction.getEdgeInfo() == edge) {
        pen.setColor(Qt::red);
    }
    painter.setPen(pen);
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
    // Рисование стрелки (самих маленьких боковых линий).
    // Перенесем систему координат в точку конца ребра (ориенированного).
    // Возьмем маленький отрезок ребра, и нарисуем его, повернув при этом систуму координат
    // на +30 и -30 градусов соответственно относительно исходного ребра.
    // Получилась стрелка ->
    // После этого восстановим систему координат.
    painter.setPen(pen);
    painter.drawLine(pointFrom, pointTo);
    painter.translate(pointTo.x(), pointTo.y());
    // Еще нужно отступить от конца ребра на радиус вершины
    // (т.к. ребро идет от центра к центру).
    painter.translate(offsetVector.x(), offsetVector.y());
    painter.rotate(30);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.rotate(-60);
    painter.drawLine(QPoint(0, 0), arrow);
    painter.resetTransform();
    // Выводим информацию о ребре (flow | capacity) по середине ребра.
    if (state == ALGORITHM_RUN || (state == ALGORITHM_TERM && edge.flow != 0)) {
        QPen penForEdgeInfo;
        penForEdgeInfo.setColor(Qt::darkGreen);
        painter.setPen(penForEdgeInfo);
        std::string edgeInfo = "(" + std::to_string(edge.getFlow()) + " | " + std::to_string(edge.getCapacity()) + ")";
        painter.drawText(pointFrom.x() + (pointTo.x() - pointFrom.x()) / 2,
                         pointFrom.y() + (pointTo.y() - pointFrom.y()) / 2,
                         edgeInfo.c_str());
    }
}

// По координатам курсора получает уникальный номер вершины, которая
// попадает под него. В противном случае возвращает номер вешины,
// следующей за последней в списке вершин (то есть verticesList.size()).
VertexIndex MaxFlowVisualizer::getVertexUnderCursor(QPoint cursorPosition) {
    QVector2D cursor(cursorPosition);
    for (VertexIndex vertex = 0; vertex < verteciesList.size(); ++vertex) {
        QVector2D currentVertexCenter(verteciesList[vertex].getCenterCoordX(), verteciesList[vertex].getCenterCoordY());
        QVector2D distanceVector(currentVertexCenter - cursor);
        if (distanceVector.length() < verteciesList[vertex].getRadius()) {
            return vertex;
        }
    }
    return verteciesList.size();
}

// Вызывается при срабатывании таймера. В зависимости от состояния делает
// соответсвующие изменения и перерисовывает окно.
void MaxFlowVisualizer::animationStep() {
    switch (state) {
        case PLANARIZATION:
            for (int i = 0; i < PLANARIZATION_SPEED; ++i) {
                if (networkPlacer.doStep(verteciesList)) {
                    state = ALGIRITHM_INIT;
                }
            }
            break;
        case ALGIRITHM_INIT:
            lastAlgoAction = relabelToFrontAlgo.init();
            algoStepsCount = 0;
            state = ALGORITHM_RUN;
            break;
            // Все остальные действия зависят не от таймера, а от событий клавиатуры и мыши.
            // Поэтому тут они не обрабатываются. Однако при некоторых условиях система может
            // вернуться в состояния, трубующие таймера. Поэтому оставляем таймер работать.
    }
    update();
}

