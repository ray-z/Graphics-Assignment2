#include "scene.h"

Scene::Scene()
{
}

void Scene::init(bool f, double x, double y, double z, bool isFrame, bool isCube, bool isCylinder)
{
    isFilled = f;
    xangle = x;
    yangle = y;
    zangle = z;
    showFrame = isFrame;
    showCube = isCube;
    showCylinder = isCylinder;
}

void Scene::draw()
{
    drawGround();
    drawPoints();
    if(showFrame) drawFrame(tForFrame);
    if(showCylinder) drawCylinder();
    makeDice();
}

QVector3D Scene::getSelectedPoint(int i)
{
    return points.at(i);
}

int Scene::getPointsL()
{
    return points.length();
}

void Scene::setFramePos( double t)
{
    //pointIndex = i;
    tForFrame = t;
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
    selectedPoint = points.length()-1;
}

void Scene::drawPoints()
{
    if(points.length() == 0) return;
    //glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    glColor3f(0.0, 0.0, 0.0);
    glPointSize(10.0);
    glBegin(GL_POINTS);
    foreach(QVector3D p, points)
    {
        glVertex3f(p.x(), p.y(), p.z());
    }
    glEnd();

    if(selectedPoint != -1)
    {
        glColor3f(1.0, 0.0, 0.0);
        glPointSize(15.0);
        glBegin(GL_POINTS);
        glVertex3f(points.at(selectedPoint).x(),
                   points.at(selectedPoint).y(),
                   points.at(selectedPoint).z());
        glEnd();
    }

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
}

void Scene::drawFrame(double sliderT)
{
    if(points.length() < 4) return;

    // convert sliderT to t
    double unitL = 1.0 / (points.length()-3);
    int i = sliderT/unitL + 1;
    double t = (sliderT - unitL*(i-1)) / unitL;

    // convert sliderT to t
    //double unitL = 1.0 / (points.length()-3);
    //int i = sliderT/unitL + 1;
    //double t = (sliderT - unitL*(i-1)) / unitL;

    QList<QVector3D> listTBN = getPTBN(i, t);
    QVector3D pPos = listTBN.at(0);
    QVector3D pointT = listTBN.at(1);
    QVector3D pointN = listTBN.at(2);
    QVector3D pointB = listTBN.at(3);

    /*
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
    */


    // Draw Frenet Frame
    //QVector3D pPos = getPointPos(i, t);
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

    // test
//    glBegin(GL_POINTS);
//    QVector3D q(pointN.x(), pointN.y(), pointN.z());
//    QMatrix4x4 m;

//    for(double d=0; d < 360; d++)
//    {

//        m.rotate(d, pointT.x(), pointT.y(), pointT.z());
//        q = q*m;

//        glVertex3f(q.x()+pPos.x(), q.y()+pPos.y(), q.z()+pPos.z());
//    }
//    glEnd();

//    QVector3D zAxis(0, 0, 1);
//    QVector3D testX2(pointT.x(), 0, pointT.z());
//    QVector3D yAxis(0, 1, 0);
//    QVector3D testX2(0, pointT.y(), pointT.z());
//    double angleX = qAcos(testX.dotProduct(testX, pointT)) * 180 / 3.14;
//    double angleY = qAcos(testY.dotProduct(testY, pointT)) * 180 / 3.14;
//    QVector3D zV(0, 0, 1);
//    QVector3D testP(1, 0, 1);
//    QVector3D xzV(pointT.x(), pointT.y(), pointT.z());
//    QVector3D yzV(pointT.x(), pointT.y(), pointT.z());
//    qDebug() << testP.dotProduct(zV, testP) * 180 / 3.14;
//    double zxAngle = acos(zV.dotProduct(zV, xzV)) * 180 / 3.14;
//    double zyAngle = acos(zV.dotProduct(zV, yzV)) * 180 / 3.14;

//    GLUquadricObj *quadratic;
//    quadratic = gluNewQuadric();

//    glRotatef(zxAngle, 0, 1, 0);
//    glRotatef(zyAngle, 0, 0,  1);
    //glTranslatef(pPos.x(), pPos.y(), pPos.z());

//    gluCylinder(quadratic,0.1f,0.1f,3.0f,32,32);

//    QVector3D plane1(3, 5, 0);
//    double d1 = vectorV.dotProduct(vectorVQ, vectorV);
//    qDebug() << d1 / (vectorV.length()*vectorVQ.length());
//    QVector3D plane2(2, 1, 0);
//    double d = plane1.dotProduct(plane1, plane2);
//    qDebug() << d / (plane1.length()*plane2.length());

    // show Cube
    if(showCube) drawCube(pPos, pointT, pointB, pointN);

    // show Cylinder
    // test

//    GLUquadricObj *quadratic;
//    quadratic = gluNewQuadric();
//    QVector3D zAxis(0, 0, 1);
//    QVector3D rotateV = rotateV.crossProduct(zAxis, pointT);
//    double angle = acos(zAxis.dotProduct(zAxis, pointT)) * 180.0 / M_PI;
//    glTranslatef(pPos.x(), pPos.y(), pPos.z());
//    glRotatef(angle, rotateV.x(), rotateV.y(), rotateV.z());

//    gluCylinder(quadratic,0.1f,0.1f,1.0f,32,32);



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

QList<QVector3D> Scene::getPTBN(int i, double t)
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

    QVector3D pPos = getPointPos(i, t);

    QList<QVector3D> listTBN;
    listTBN << pPos << pointT << pointB << pointN;

    return listTBN;
}

void Scene::drawCube(QVector3D vCentre, QVector3D vT, QVector3D vB, QVector3D vN)
{
    /*
     * All followings are points instead of vectors:
     * Back: vCenter-VB-vBack-vT
     * Bottom: vCenter-vT-vBottom-vN
     * Left: vCenter-vN-vLeft-vB
     * Front: vN-vLeft-vFront-vBottom
     * Top: vB-vBack-vFront-vLeft
     * Right: vT-vBottom-vFront-vBack
     */
    QVector3D vBack = vB + vT;
    QVector3D vBottom = vT + vN;
    QVector3D vLeft = vB + vN;
    QVector3D vFront = vB + vBottom;

    vT += vCentre;
    vB += vCentre;
    vN += vCentre;
    vBack += vCentre;
    vBottom += vCentre;
    vLeft += vCentre;
    vFront += vCentre;

    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(1);
    // Back
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vCentre.x(), vCentre.y(), vCentre.z());
    glVertex3f(vB.x(), vB.y(), vB.z());
    glVertex3f(vBack.x(), vBack.y(), vBack.z());
    glVertex3f(vT.x(), vT.y(), vT.z());
    glEnd();
    // Bottom
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vCentre.x(), vCentre.y(), vCentre.z());
    glVertex3f(vT.x(), vT.y(), vT.z());
    glVertex3f(vBottom.x(), vBottom.y(), vBottom.z());
    glVertex3f(vN.x(), vN.y(), vN.z());
    glEnd();
    // Left
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vCentre.x(), vCentre.y(), vCentre.z());
    glVertex3f(vN.x(), vN.y(), vN.z());
    glVertex3f(vLeft.x(), vLeft.y(), vLeft.z());
    glVertex3f(vB.x(), vB.y(), vB.z());
    glEnd();
    // Front
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vN.x(), vN.y(), vN.z());
    glVertex3f(vLeft.x(), vLeft.y(), vLeft.z());
    glVertex3f(vFront.x(), vFront.y(), vFront.z());
    glVertex3f(vBottom.x(), vBottom.y(), vBottom.z());
    glEnd();
    // Top
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vB.x(), vB.y(), vB.z());
    glVertex3f(vBack.x(), vBack.y(), vBack.z());
    glVertex3f(vFront.x(), vFront.y(), vFront.z());
    glVertex3f(vLeft.x(), vLeft.y(), vLeft.z());
    glEnd();
    // Right
    if (isFilled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    glVertex3f(vT.x(), vT.y(), vT.z());
    glVertex3f(vBottom.x(), vBottom.y(), vBottom.z());
    glVertex3f(vFront.x(), vFront.y(), vFront.z());
    glVertex3f(vBack.x(), vBack.y(), vBack.z());
    glEnd();
}

void Scene::drawCylinder()
{
    glColor3f(0.0, 1.0, 1.0);
    QVector3D zAxis(0, 0, 1);
    GLUquadricObj *quadratic;
    quadratic = gluNewQuadric();
    for(int i=1; i<points.length()-2; i++)
    {
        for(double t=0; t<1; t+=0.02)
        {
            QList<QVector3D> listPTBN = getPTBN(i, t);
            QVector3D vP = listPTBN.at(0);
            QVector3D vT = listPTBN.at(1);

            QVector3D rotateV = rotateV.crossProduct(zAxis, vT);
            double angle = acos(zAxis.dotProduct(zAxis, vT)) * 180.0 / M_PI;
            glPushMatrix();
            glTranslatef(vP.x(), vP.y(), vP.z());
            glRotatef(angle, rotateV.x(), rotateV.y(), rotateV.z());

            gluCylinder(quadratic,0.1f,0.1f,0.2f,32,32);
            glPopMatrix();
        }
    }

    /*
    glLineWidth(10);
    glColor3f(0.0, 1.0, 1.0);

    for(int i=1; i<points.length()-2; i++)
    {
        for(double t=0; t<1; t+=0.02)
        {
            QList<QVector3D> listPTBN = getPTBN(i, t);
            QVector3D vP = listPTBN.at(0);
            QVector3D vT = listPTBN.at(1);
            QVector3D vN = listPTBN.at(3);

            glBegin(GL_LINE_LOOP);
            for(double d=0; d<360; d++)
            {
                QVector3D newP = vN;
                QMatrix4x4 m;
                // Rotate around T
                m.rotate(d, vT.x(), vT.y(), vT.z());
                newP = newP * m;
                glVertex3f(newP.x()+vP.x(), newP.y()+vP.y(), newP.z()+vP.z());
            }
            glEnd();
        }
    }
    */
}

int Scene::isSelected(int cMode, double h, double v)
{
    selectedPoint = -1;
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
                selectedPoint = i;
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
                selectedPoint = i;
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
                selectedPoint = i;
            }
        }
    }
        break;
    }
    return selectedPoint;
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

