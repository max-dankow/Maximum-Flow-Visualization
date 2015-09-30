#include "mainwindow.h"

MaxFlowVisualizer::MaxFlowVisualizer(QWidget* parent) : QGLWidget(parent)
{
    addVertex(10, 10);
    addVertex(10, 100);
    /*point=0;
    geese_size=30;
    wax=600; way=600; // начальный размер окна
    singling=false;*/
    setFormat(QGLFormat(QGL::DoubleBuffer)); // Двойная буферизация
    glDepthFunc(GL_LEQUAL); // Буфер глубины
    /*QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(geese_coord()));
    timer->start(750);*/
}

void MaxFlowVisualizer::addVertex(GLdouble coordX, GLdouble coordY)
{
    vertecies.push_back(CVertexGL(coordX, coordY));
}

void MaxFlowVisualizer::initializeGL()
{
    qglClearColor(Qt::white);
}

void MaxFlowVisualizer::resizeGL(int nWidth, int nHeight)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, (GLint)nWidth, (GLint)nHeight);
    /*wax = nWidth;
    way = nHeight;*/
}

void MaxFlowVisualizer::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // чистим буфер изображения и буфер глубины
    glMatrixMode(GL_PROJECTION); // устанавливаем матрицу
    glLoadIdentity(); // загружаем матрицу
    //glOrtho(0,wax,way,0,1,0); // подготавливаем плоскости для матрицы
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    qglColor(Qt::white);
    //renderText(10, 10 , 0, QString::fromUtf8("Вы набрали %1 очков:").arg(point), QFont() , 2000);

    drawAll(); // Рисуем объект

    swapBuffers();
}

void MaxFlowVisualizer::keyPressEvent(QKeyEvent *ke)
{
    /*switch (ke->key())
    {

          case Qt::Key_Space:
                geese_coord();
                break;
    }
    updateGL();*/
}

void MaxFlowVisualizer::mouseMoveEvent(QMouseEvent *me)
{
    /*// Получаем координаты курсора
    cax=me->x();
    cay=me->y();
    updateGL();*/
}

void MaxFlowVisualizer::mousePressEvent(QMouseEvent *me)
{
    /*// При нажатии левой кнопки мыши определяем попали мы в гуся или нет?
    if(me->button()==Qt::LeftButton){
        if(me->x()>gdx&&me->x()<gdx+geese_size){
            if(me->y()>gdy&&me->y()<gdy+geese_size){
                point++;
                geese_coord();
            }
        }
    }

    // Если была нажата левая кнопка мыши, получаем начальные координаты выделения
    if(me->button()==Qt::LeftButton){
        singling=true;
        cbx=me->x();
        cby=me->y();
        updateGL();
    } else {
        singling=false;
    }
    updateGL();*/
}

void MaxFlowVisualizer::mouseReleaseEvent(QMouseEvent *me)
{
    // Если отпускаем левую кнопку мыши - удалить выделение
    /*if(singling==true&&me->button()==Qt::LeftButton){
        singling=false;
    }
    updateGL();*/
}


void MaxFlowVisualizer::drawCircle(GLdouble coordX, GLdouble coordY)
{
    glBegin(GL_POLYGON);
        glColor3f(0,0,0);
        int sides = 36;
        double radius = 10;
        for (int angle = 0; angle < 360; angle += 360 / sides)
          {
            double heading = angle * 3.1415926535897932384626433832795 / 180;
            glVertex2d(cos(heading) * radius + coordX, sin(heading) * radius + coordY);
          }
    glEnd();
    glFlush();
}

void MaxFlowVisualizer::drawAll()
{
    for (CVertexGL vertex : vertecies)
    {
        drawCircle(vertex.getCoordX(), vertex.getCoordY());
    }

}
