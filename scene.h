#ifndef SCENE_H
#define SCENE_H

#include <QtOpenGL>

const double halfLength=0.8;    // half length of cube sides
class Scene
{
public:
    Scene();

    // Initialise a scene
    void init(bool isFilled, double xangle, double yangle, double zangle);
    // Make dice, draw ground and axis
    void draw();

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
};

#endif // SCENE_H
