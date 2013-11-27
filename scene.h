#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>
#include <QList>
#include <math.h>
#include <QMatrix4x3>

const double halfLength=0.8;    // half length of cube sides
const double selectAccuracy=0.2;    // different range allowed between mouse pointer and point

class Scene
{
public:
    Scene();

    // Initialise a scene
    void init(bool isFilled, double xangle, double yangle, double zangle,
              bool isFrame, bool isCube, bool isCylinder);
    // Make dice, draw ground and axis
    void draw();
    // Add/Select/Delete points in scene
    void addPoint(int cMode, double h, double v);
    void movePoint(int cMode, int i, double h, double v);
    void deletePoint(int i);

    // Check if a point is selected
    // return index of point if selected, else return -1
    int isSelected(int cMode, double h, double v);
    // Get selected point position
    QVector3D getSelectedPoint(int i);
    // Get total points length
    int getPointsL();
    // Set Frame position
    void setFramePos( double t);


private:
    // Draw grid ground
    void drawGround();
    // Make a dice for scene
    GLuint makeDice();
    // Draw face of cube
    void drawFace( int tim, float w);
    // Cube color on/off
    bool isFilled;
    // Cube angles
    double xangle, yangle, zangle;
    // A list to store points
    QList<QVector3D> points;

    void drawPoints();
    // Draw Catmull-Rom Spline
    void drawSpline();
    // Draw Frenet Frame at selected point
    void drawFrame(double sliderT);

    //Points on the Catmull-Rom spline
    QVector3D getPointPos(int i, double t);

    // Store Frame position
    //int pointIndex;
    double tForFrame;

    // Draw cube w.r.t Frenet Frame
    void drawCube(QVector3D vCentre, QVector3D vT, QVector3D vB, QVector3D vN);

    // Draw Cylinder
    void drawCylinder();


    // on/off frame and cube
    bool showFrame, showCube, showCylinder;

    // select point index
    int selectedPoint;
};

#endif // SCENE_H
