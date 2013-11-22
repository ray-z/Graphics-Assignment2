//-------------------------------------------------------------------------------------------
//  University of Bath Computer Science Department
//	FrameWork for OpenGL application under QT
//  Course title: Computer Graphics CM20219
//-------------------------------------------------------------------------------------------
//These two lines are header guardians against multiple includes
#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QProgressBar>
#include "foundation.h"
#include <QtGui>
#include <QtOpenGL>
#include <GL/glu.h>
#include "version.h"
#include <QList>
#include "scene.h"

const int maxw=1024;
const int maxh=1024;
const int unitLength=20;    // store unit length of mouse move
// const double halfLength=0.8;    // half length of cube sides
const double mouseSpeed=0.01;
// mouse coordinate to widget coordinate: (x-xDiff, yDiff-y)
const double xDiff = 285;
const double yDiff = 289;
// coordinate to real length ratio
const double coordToL = 57;

//This is our OpenGL Component we built it on top of QGLWidget
class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    //Constructor for GLWidget
    GLWidget(QWidget *parent = 0);

    //Destructor for GLWidget
    ~GLWidget();

    void clear();
    //void about();
    //void help();
    void rotx(int);
    void roty(int);
    void rotz(int);
    void setxFrom(int a);
    void setyFrom(int a);
    void setzFrom(int a);
    void setFilled(bool a);

    // Different camera view postions
    void setPerspectiveView();
    void setTopView();
    void setFrontView();
    void setRightView();

    // Different mouse mode
    void setMouseMode(int i);

protected:
    //Initialize the OpenGL Graphics Engine
    void initializeGL();

    //All our painting stuff are here
    void paintGL();

    //When user resizes main window, the scrollArea will be resized and it will call this function from
    //its attached GLWidget
    void resizeGL(int width, int height);
    //Handle mouse
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    // Draw ground
    //void paintEvent(QPaintEvent *);

private:
    void startup();
    GLdouble xfrom,yfrom,zfrom, xto, yto, zto;
    int winw, winh, button,imx,imy;
    int version;
    void dopan(int x, int y, bool click);
    void zoomWheel(int z);
    QImage *myimage;
    QImage timage;
    void drawFace(float w, float h);
    void displayImage();
    void drawCircle(double radius, double xcen, double ycen, bool line, int nseg);
    // for display list
    int object;
    // for the cube
    double xangle, yangle, zangle, scale;
    QImage tex[6]; // one for each face
    bool filled;

    // Functions
    void redraw();
    //void makeSpots(int tim, QImage *buf);
    //void drawCircle(int radius, int xcen, int ycen,  QImage *buf);
    //void drawFace( int tim, float w);
    //GLuint makeDice( );
    void initLight();

    // Store mouse x, y positions when click
    int mouseX;
    int mouseY;

    // Draw grid ground
    //void drawGround();

    // A list of 'View to' point
    QList<QList<double> > viewPoints;
    void initViewPointsList();
    void addViewPoint(double x, double y, double z);

    // Store camera position to view point vector
    QList<double> cameraToPoint;

    // Store angle and elevation
    double angle;
    double elevation;

    // Store radius
    double radius;

    // Return a list contains current camera position
    QList<double> getCameraPosition();

    /* Camera Mode:
     * 0 - Perspective View
     * 1 - Top View: x-z
     * 2 - Front View: x-y
     * 3 - Right View: z-y
    */
    int cMode;

    /* Mouse Mode:
     * 0 - Control Camera
     * 1 - Add Point
     * 2 - Move Point
     * 3 - Delete Point
    */

    // Set init ortognal view
    void setOrthoView();

    int mMode;

    // up vector
    float xup, yup, zup;

    Scene scene;

};


#endif
