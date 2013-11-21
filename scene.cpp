#include "scene.h"

Scene::Scene()
{
}

void Scene::init(bool f, double x, double y, double z)
{
    isFilled = f;
    xangle = x;
    yangle = y;
    zangle = z;

}

void Scene::draw()
{
    drawGround();
    drawPoints();
    makeDice();

}

GLuint Scene::makeDice()
{

    GLuint list;
    float w = halfLength;

    // list = glGenLists( 1 );
    // glNewList( list, GL_COMPILE );   no display list this version

    // Rotation
    glRotatef( xangle, 1.0, 0.0, 0.0 );
    glRotatef( yangle, 0.0, 1.0, 0.0 );
    glRotatef( zangle, 0.0, 0.0, 1.0 );

    // one
    drawFace(0,  w);

    // six
    glPushMatrix();
    glRotatef( 180.0, 1.0, 0.0, 0.0 );
    drawFace(5, w);
    glPopMatrix();

    // four on left
    glPushMatrix();
    glRotatef( -90.0, 0.0, 1.0, 0.0 );
    drawFace(3, w);
    glPopMatrix();

    // three on right
    glPushMatrix();
    glRotatef( 90.0, 0.0, 1.0, 0.0 );
    drawFace(2, w);
    glPopMatrix();

    // two
    glPushMatrix();
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    drawFace(1, w);
    glPopMatrix();

    // five
    glPushMatrix();
    glRotatef( -90.0, 1.0, 0.0, 0.0 );
    drawFace(4, w);
    glPopMatrix();

  //  glEndList();

  //  return list;
    return 0;
}

void Scene::drawFace( int tim, float w)
{
    // this version no texturing
 //   glTexImage2D( GL_TEXTURE_2D, 0, 3, tex[tim].width(), tex[tim].height(), 0,
           //       GL_RGBA, GL_UNSIGNED_BYTE, tex[tim].bits() );
    glOrtho(1.15, 1.15, -1.0f, 1.0f, -1.0f, 1.0f);
    glLineWidth(2);
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    //glTexCoord2f(0.0, 0.0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(-w, -w, w );
   // glTexCoord2f(0.0, 1.0);
    glVertex3f(w, -w, w );
    //glTexCoord2f(1.0, 1.0);
    glVertex3f(w, w, w );
    //glTexCoord2f(1.0, 0.0);
    glVertex3f(-w, w, w );

    glEnd();

}

void Scene::drawGround()
{
    // Draw axis
    glLineWidth(3);
    glColor3f(0.0, 0.0, 0.0);

    glBegin(GL_LINES);
    // X axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(5.0, 0.0, 0.0);
    // Y axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 5.0, 0.0);
    // Z axis
    glVertex3f(0.0, 0.0, 0.0);
    glVertex3f(0.0, 0.0, 5.0);
    glEnd();

    // Draw ground
    glLineWidth(1);
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_LINE_LOOP);
    glVertex3f(-10.0, 0.0, -10.0);
    glVertex3f(10.0, 0.0, -10.0);
    glVertex3f(10.0, 0.0, 10.0);
    glVertex3f(-10.0, 0.0, 10.0);
    glEnd();

    // Draw grid
    glBegin(GL_LINES);
    for(double i=-10; i<10; i+=0.5)
    {
        // x
        glVertex3f(-10.0, 0.0, i);
        glVertex3f(10.0, 0.0, i);
        // z
        glVertex3f(i, 0.0, -10.0);
        glVertex3f(i, 0.0, 10.0);
    }
    glEnd();
}

void Scene::addPoint(double x, double y, double z)
{
    selectedPoint << x << y << z;
    points << selectedPoint;
    selectedPoint.clear();
}

void Scene::drawPoints()
{
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);

    glPointSize(10.0);


    glBegin(GL_POINTS);
    foreach(QList<double> p, points)
    {
        glVertex3f(p.at(0), p.at(1), p.at(2));
    }
    glEnd();

    if(points.length() > 3) drawSpline();
}

void Scene::drawSpline()
{
    int numP = points.length();
    float xNew, yNew, zNew;   //Points on the Catmull-Rom spline

    // init a list of x, y, z
    float x[numP], y[numP], z[numP];
    for(int l = 0; l < numP; l++)
    {
        x[l] = points.at(l).at(0);
        y[l] = points.at(l).at(1);
        z[l] = points.at(l).at(2);
    }

    glBegin(GL_LINE_STRIP);
    for(int i = 1; i < numP-2; i++)
    {
        for(int k = 0;  k < 50; k++){    //50 points
           float t = k*0.02;  //Interpolation parameter
           xNew = x[i] + 0.5*t*(-x[i-1]+x[i+1])
               + t*t*(x[i-1] - 2.5*x[i] + 2*x[i+1] - 0.5*x[i+2])
               + t*t*t*(-0.5*x[i-1] + 1.5*x[i] - 1.5*x[i+1] + 0.5*x[i+2]);
           yNew = y[i] + 0.5*t*(-y[i-1]+y[i+1])
               + t*t*(y[i-1] - 2.5*y[i] + 2*y[i+1] - 0.5*y[i+2])
               + t*t*t*(-0.5*y[i-1] + 1.5*y[i] - 1.5*y[i+1] + 0.5*y[i+2]);
           zNew = z[i] + 0.5*t*(-z[i-1]+z[i+1])
               + t*t*(z[i-1] - 2.5*z[i] + 2*z[i+1] - 0.5*z[i+2])
               + t*t*t*(-0.5*z[i-1] + 1.5*z[i] - 1.5*z[i+1] + 0.5*z[i+2]);
           glVertex3f(xNew, yNew, zNew);
       }
    }
    glEnd();
}

