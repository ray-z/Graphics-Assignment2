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
    makeDice();
    drawGround();
    drawPoints();
    //drawFrame();

}

QVector3D Scene::getSelectedPoint(int i)
{
    return points.at(i);
}

int Scene::getPointsL()
{
    return points.length();
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
    glRotatef( 0, 1.0, 0.0, 0.0 );
    glRotatef( 0, 0.0, 1.0, 0.0 );
    glRotatef( 0, 0.0, 0.0, 1.0 );
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

void Scene::addPoint(int cMode, double h, double v)
{
    /* Top view: (h, 0, -v)
     * Front view: (h, v, 0)
     * Right view: (0, v, -h)
     */
    QVector3D newPoint(0.0, 0.0, 0.0);
    switch (cMode)
    {
    case 1:
    {
        //newPoint(h, 0.0, -v);
        newPoint.setX(h);
        newPoint.setZ(-v);
    }
        break;
    case 2:
    {
        //newPoint(h, v, 0.0);
        newPoint.setX(h);
        newPoint.setY(v);
    }
        break;
    case 3:
    {
        //newPoint(0.0, v, -h);
        newPoint.setY(v);
        newPoint.setZ(-h);
    }
        break;
    }
    points << newPoint;
}

void Scene::drawPoints()
{
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(1.0, 0.0, 0.0);

    glPointSize(10.0);


    glBegin(GL_POINTS);
    foreach(QVector3D p, points)
    {
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();

    if(points.length() > 3) drawSpline();
}



void Scene::drawSpline()
{
    int numP = points.length();
    glColor3f(0.0, 0.0, 0.0);
    glLineWidth(1);
    glBegin(GL_LINE_STRIP);
    for(int i = 1; i < numP-2; i++)
    {
        for(double t=0; t<1; t+=0.02)
        {
            QVector3D p = getPointPos(i, t);

            glVertex3f(p.x(), p.y(), p.z());
        }
    }
    glEnd();
    for(int i = 1; i < numP-2; i++)
    {
        drawFrame(i,0);
    }
}

void Scene::drawFrame(int i, double t)
{
    // Catmull-Rom Spline Equation: P = A*t*t*t + B*t*t + C*t + D;
    QVector3D vectorA, vectorB, vectorC;
    vectorA = (-0.5*points.at(i-1) + 1.5*points.at(i) - 1.5*points.at(i+1) + 0.5*points.at(i+2));
    vectorB = (points.at(i-1) - 2.5*points.at(i) + 2*points.at(i+1) - 0.5*points.at(i+2));
    vectorC = (-0.5*points.at(i-1)+0.5*points.at(i+1));

    // V = 3*A*t*t + 2*B*t + C
    // Q = 6*A*t + 2*B
    QVector3D vectorV, vectorQ, vectorVQ, vectorVQV;
    vectorV = 3*vectorA*t*t + 2*vectorB*t + vectorC;
    vectorQ = 6*vectorA*t + 2*vectorB;
    vectorVQ = vectorVQ.crossProduct(vectorV, vectorQ);
    vectorVQV = vectorVQV.crossProduct(vectorVQ, vectorV);
    // T = V/|V|
    // B = V*Q/|V*Q|
    // N = V*Q*V/|V*Q*V|
    // Not a real vector, to store point position only
    QVector3D pointT, pointN, pointB;
    pointT = vectorV.normalized();
    pointB = vectorVQ.normalized();
    pointN = vectorVQV.normalized();

    // Draw Frenet Frame
    QVector3D pPos = getPointPos(i, t);
    glLineWidth(2);
    // T: Red
    glColor3f(1.0, 0.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(pPos.x(), pPos.y(), pPos.z());
    glVertex3f(pPos.x()+pointT.x(), pPos.y()+pointT.y(), pPos.z()+pointT.z());
    glEnd();
    // B: Green
    glColor3f(0.0, 1.0, 0.0);
    glBegin(GL_LINES);
    glVertex3f(pPos.x(), pPos.y(), pPos.z());
    glVertex3f(pPos.x()+pointB.x(), pPos.y()+pointB.y(), pPos.z()+pointB.z());
    glEnd();
    // N: Blue
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_LINES);
    glVertex3f(pPos.x(), pPos.y(), pPos.z());
    glVertex3f(pPos.x()+pointN.x(), pPos.y()+pointN.y(), pPos.z()+pointN.z());
    glEnd();


    /*
    int numP = points.length();
    // init a list of x, y, z

    double x[numP], y[numP], z[numP];
    for(int l = 0; l < numP; l++)
    {
        x[l] = points.at(l).at(0);
        y[l] = points.at(l).at(1);
        z[l] = points.at(l).at(2);
    }

    float xv, yv, zv, xt, yt, zt;
    for(int i = 1; i < numP-2; i++)
    {
        double t = 0.5;

        float xa = -0.5*x[i-1] + 1.5*x[i] - 1.5*x[i+1] + 0.5*x[i+2];
        float xb = x[i-1] - 2.5*x[i] + 2*x[i+1] - 0.5*x[i+2];
        float xc = -0.5*x[i-1]+0.5*x[i+1];

        float ya = -0.5*y[i-1] + 1.5*y[i] - 1.5*y[i+1] + 0.5*y[i+2];
        float yb = y[i-1] - 2.5*y[i] + 2*y[i+1] - 0.5*y[i+2];
        float yc = -0.5*y[i-1]+0.5*y[i+1];

        float za = -0.5*z[i-1] + 1.5*z[i] - 1.5*z[i+1] + 0.5*z[i+2];
        float zb = z[i-1] - 2.5*z[i] + 2*z[i+1] - 0.5*z[i+2];
        float zc = -0.5*z[i-1]+0.5*z[i+1];
        glBegin(GL_LINE_STRIP);
        for(int i = 1; i < numP-2; i++)
        {
            for(int k = 0;  k < 50; k++)    //50 points
            {
                double t = k*0.02;  //Interpolation parameter
                xv = (3*xa*t*t + 2*xb*t + xc);
                yv = (3*ya*t*t + 2*yb*t + yc);
                zv = (3*za*t*t + 2*zb*t + zc);

                xt = xv/sqrt(xv*xv + yv*yv + zv*zv);
                yt = yv/sqrt(xv*xv + yv*yv + zv*zv);
                zt = zv/sqrt(xv*xv + yv*yv + zv*zv);

            glVertex3f(xt, yt, zt);
            }
        }
        glEnd();

    }
    */
}

int Scene::isSelected(int cMode, double h, double v)
{
    switch (cMode)
    {
    case 1: // Top view: x-z
    {
        for(int i=0; i<points.length(); i++)
        {
            //qDebug() << fabs(h - points.at(i).at(0)) << ", " << fabs(v - points.at(i).at(2));

            if(fabs(h - points.at(i).x()) < selectAccuracy &&
                    fabs(-v - points.at(i).z()) < selectAccuracy)
            {
                return i;
            }
        }
    }
        break;
    case 2: // Front view: x-y
    {
        for(int i=0; i<points.length(); i++)
        {
            //qDebug() << fabs(h - points.at(i).at(0)) << ", " << fabs(v - points.at(i).at(2));

            if(fabs(h - points.at(i).x()) < selectAccuracy &&
                    fabs(v - points.at(i).y()) < selectAccuracy)
            {
                return i;
            }
        }
    }
        break;
    case 3: // Right view: z-y
    {
        for(int i=0; i<points.length(); i++)
        {
            //qDebug() << fabs(h - points.at(i).at(0)) << ", " << fabs(v - points.at(i).at(2));

            if(fabs(v - points.at(i).y()) < selectAccuracy &&
                    fabs(-h - points.at(i).z()) < selectAccuracy)
            {
                return i;
            }
        }
    }
        break;
    }
    return -1;
}

void Scene::movePoint(int cMode, int i, double h, double v)
{
    QVector3D selectedPoint=points.at(i);
    switch (cMode)
    {
    case 1: // Top view: x-z (h, 0, -v)
    {
        //selectedPoint << points.at(i).at(0) + h << points.at(i).at(1) << points.at(i).at(2) + v;
        selectedPoint.setX(selectedPoint.x() + h);
        selectedPoint.setZ(selectedPoint.z() + v);
    }
        break;
    case 2: // Front view: x-y (h, v, 0)
    {
        //selectedPoint << points.at(i).at(0) + h << points.at(i).at(1) - v << points.at(i).at(2);
        selectedPoint.setX(selectedPoint.x() + h);
        selectedPoint.setY(selectedPoint.y() - v);
    }
        break;
    case 3: // Right view: z-y (0, v, -h)
    {
        //selectedPoint << points.at(i).at(0) << points.at(i).at(1) - v << points.at(i).at(2) - h;
        selectedPoint.setY(selectedPoint.y() - v);
        selectedPoint.setZ(selectedPoint.z() - h);
    }
        break;
    }
    points.replace(i, selectedPoint);
}

void Scene::deletePoint(int i)
{
    points.removeAt(i);
}

QVector3D Scene::getPointPos(int i, double t)
{
    QVector3D pointPos;
    pointPos = points.at(i) + t*(-0.5*points.at(i-1)+0.5*points.at(i+1))
        + t*t*(points.at(i-1) - 2.5*points.at(i) + 2*points.at(i+1) - 0.5*points.at(i+2))
        + t*t*t*(-0.5*points.at(i-1) + 1.5*points.at(i) - 1.5*points.at(i+1) + 0.5*points.at(i+2));
    return pointPos;
}

