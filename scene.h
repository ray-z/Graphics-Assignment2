#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>
#include <QList>
#include <math.h>

const double halfLength=0.8;    // half length of cube sides
const double selectAccuracy=0.2;    // different range allowed between mouse pointer and point

class Scene
{
public:
    Scene();

    // Initialise a scene
    void init(bool isFilled, double xangle, double yangle, double zangle);
    // Make dice, draw ground and axis
    void draw();
    // Add/Select/Delete points in scene
    void addPoint(int cMode, double h, double v);
    void movePoint(int cMode, int i, double h, double v);
    void deletePoint(int i);

    // Check if a point is selected
    // return index of point if selected, else return -1
    int isSelected(int cMode, double h, double v);

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
    QList<QList<double> > points;

    void drawPoints();
    void drawSpline();
    void drawFrame();

};

#endif // SCENE_H
