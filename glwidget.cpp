//-------------------------------------------------------------------------------------------
//   Qtgl for graphics class
//-------------------------------------------------------------------------------------------

#include "glwidget.h"



const double torad = M_PI/180.0;
const double lim=0.5;
GLfloat vertices[4][2] = {{100.0, 100.0}, {400.0, 100.0}, {400.0, 400.0}, {100.0, 400.0}}; //  vertex coords
GLubyte indices[] = {0,1,1,2,2,3,3,0};

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    initViewPointsList();
    startup();
}

GLWidget::~GLWidget()
{    

}

void GLWidget::initViewPointsList()
{
    // TODO: add some more view points
    addViewPoint(0, 0, halfLength); // front centre
    addViewPoint(halfLength, 0, 0); // right centre
    addViewPoint(0, 0, -halfLength); // back centre
    addViewPoint(-halfLength, 0, 0); // left centre
    addViewPoint(0, halfLength, 0); // top centre
    addViewPoint(0, -halfLength, 0); // buttom centre

    addViewPoint(halfLength, halfLength, halfLength); // front top-right vertex
    addViewPoint(0, 0, 0); // cube centre
}

void GLWidget::addViewPoint(double x, double y, double z)
{
    QList<double> viewPoint;
    viewPoint << x << y << z;
    viewPoints.append(viewPoint);
}

void GLWidget::startup()
{
    winw=width();  // width returns width of window
    winh=height();
    button = 0;
    cerr << "Glwidget\n";
    version=MYVERSION;
    xangle= yangle= zangle=0.0;
    scale = 1.5;
    object =0; // in this version no display list
    xfrom=yfrom=zfrom=5.0;
    xto=yto=zto=0.0;
    xup=zup=0.0;
    yup=1.0;
    cMode=0;
    mMode=0;
    filled=false;
    selectedPoint=-1;
    angle = M_PI/4.0;
    elevation = M_PI/4.0;
    radius = 10.0;
    isFrame = false;
    isCube = false;
    cylinderR = 0.1;
}

void GLWidget::clear()
{
     updateGL();
}

void GLWidget::initializeGL()
{
    int i;
    QImage buf(256, 256, QImage::Format_RGB32);

    GLfloat whiteDir[4] = {2.0, 2.0, 2.0, 1.0};
    GLfloat whiteAmb[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightPos[4] = {30.0, 30.0, 30.0, 1.0};

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, whiteAmb);

    glMaterialfv(GL_FRONT, GL_DIFFUSE, whiteDir);
    glMaterialfv(GL_FRONT, GL_SPECULAR, whiteDir);
    glMaterialf(GL_FRONT, GL_SHININESS, 20.0);

    glLightfv(GL_LIGHT0, GL_DIFFUSE, whiteDir);		// enable diffuse
    glLightfv(GL_LIGHT0, GL_SPECULAR, whiteDir);	// enable specular
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    glShadeModel( GL_SMOOTH );

    /* Set up the textures
     comented out for this simple version
    for (i=0; i<6; i++) {
    makeSpots(i, &buf);
    tex[i] = QGLWidget::convertToGLFormat( buf );  // flipped 32bit RGBA
    }

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glEnable( GL_TEXTURE_2D );
    */

    // Set up various other stuff
    glClearColor( 0.6, 0.6, 0.6, 0.0 ); // Let OpenGL clear to black
    glEnable( GL_CULL_FACE );  	// don't need Z testing for convex objects
    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
    //glEnable(GL_COLOR_MATERIAL);
    initLight();
    // No display list in this simple version
    //object = makeDice( );	// Generate an OpenGL display list
}

void GLWidget::redraw()
{
    updateGL();
}

/*
void GLWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawLine(10, 10, 100, 100);
}
*/
void GLWidget::paintGL()
{
    /*
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    if(cMode == 0)
        glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    else if(cMode == 1)
        glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 1500.0f);
    glMatrixMode( GL_MODELVIEW );
    */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(xfrom,yfrom,zfrom, xto, yto, zto, xup, yup, zup);
   // glTranslatef( 0.0, 0.0, -10.0 );
   // glScalef( scale, scale, scale );

   // glCallList( object );   no display list this version just make the cube
    //drawGround();
    //makeDice();
    scene.init(filled, xangle, yangle, zangle, isFrame, isCube, isCylinder, cylinderR);
    scene.draw();

}

/* 2D */
void GLWidget::resizeGL(int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    setPerspectiveView();
    /*
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    if(cMode == 0)
        glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    else if(cMode == 1)
        glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 1500.0f);
    glMatrixMode( GL_MODELVIEW );
    //glOrtho(w/(float)h, w/(float)h, -1.0f, 1.0f, -1.0f, 1.0f);
    //qDebug() << "aspect: " << w/(float)h;
    */

}

void GLWidget::initLight()
{
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 128.0 };
    // GLfloat light_position[] = { 0.3, 0.3, 0.8, 0.0 };
    GLfloat light_position[] = { 0.2, 0.2, 0.9, 0.0 };
    GLfloat light_ambient[] = { 0.05, 0.05, 0.05, 1.0 };
    // GLfloat light_specular[] = { 0.1, 0.1, 0.1, 1.0 };
    GLfloat light_specular[] = { 0.99, 0.99, 0.99, 1.0 };
    GLfloat light_diffuse[] = { 0.7, 0.7, 0.7, 1.0 };

    // glClearColor(1.0, 1.0, 0.9, 1.0);
     glShadeModel (GL_SMOOTH);
    // glShadeModel (GL_FLAT);

    // MATERIAL
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

    // LIGHT0
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);

    glColorMaterial(GL_FRONT, GL_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glEnable(GL_DEPTH_TEST);
    // glDisable(GL_LIGHTING);

}

// communication with the window widget
void GLWidget::rotx(int a)
{
    xangle =  (double)a;
    // std::cerr << " x angle "<<xangle<<"\n";
    updateGL();
}
void GLWidget::roty(int a)
{
    yangle =  (double)a;
    updateGL();
}

void GLWidget::rotz(int a)
{
    zangle =  (double)a;
    updateGL();
}


void GLWidget::setxFrom(int a)
{
    xfrom=a;
    updateGL();
}

void GLWidget::setyFrom(int a)
{
    yfrom=a;
    updateGL();
}
void GLWidget::setzFrom(int a)
{
    zfrom=a;
    updateGL();
}

// mouse routines for camera control to be implemented
void GLWidget::mousePressEvent( QMouseEvent *e )
{

    if(e->buttons()==Qt::LeftButton)
    {
        mouseX = e->pos().x();
        mouseY = e->pos().y();

        double widgetX = (mouseX - xDiff) / coordToL;
        double widgetY = (yDiff - mouseY) / coordToL;

        // selectedPoint = -1;
        if(cMode == 0)
        {
            // get current radius
            radius = sqrt(pow(xfrom, 2.0) + pow(yfrom, 2.0) + pow(zfrom, 2.0));
        }
        else
        {
            switch (mMode)
            {
            /*
            // case 0 is removed, 'move point' acts same as 'select poing'
            case 0: // Select Point
            {
                startPoint = scene.isSelected(cMode, widgetX, widgetY);
                scene.setFramePos(startPoint, tForFrame);

            }
                break;
            */
            case 1: // Add point
            {
                scene.addPoint(cMode, widgetX, widgetY);
                selectedPoint = scene.getPointsL() - 1;
            }
                break;
            case 2: // Move Point
            {
                selectedPoint = scene.isSelected(cMode, widgetX, widgetY);

                //qDebug() << "selectedPoint: " << selectedPoint;
            }
                break;
            case 3: // Delete Point
            {
                selectedPoint = scene.isSelected(cMode, widgetX, widgetY);

                if(selectedPoint != -1)
                {
                    scene.deletePoint(selectedPoint);
                    // Remove Frenet Frame
                    //startPoint = 0;
                    //scene.setFramePos(0, 0);
                }
            }
                break;
            }
        }
    }
    else if(e->buttons()==Qt::RightButton)
    {
        mouseY = e->pos().y();

        // Store camera position to view to point vector
        cameraToPoint.clear();
        cameraToPoint.append(xfrom - xto);
        cameraToPoint.append(yfrom - yto);
        cameraToPoint.append(zfrom - zto);
    }
    else if(e->buttons()==Qt::MiddleButton)
    {
        if(cMode != 0) return;
        selectedPoint +=1;
        if(selectedPoint == scene.getPointsL())
        {
            selectedPoint = -1;
            setLookTo(0, 0, 0);
        }
        else
        {
            QVector3D p = scene.getSelectedPoint(selectedPoint);
            setLookTo(p.x(), p.y(), p.z());
        }

    }
    updateGL();
}

void GLWidget::mouseReleaseEvent( QMouseEvent *e)
{

    updateGL();
}

void GLWidget::mouseMoveEvent ( QMouseEvent *e )
{
    //if(cMode !=0) return;  // Only perspective mode is allowed to move camera

    if(e->buttons()==Qt::LeftButton)
    {
        // left-right controls azimuth angle and
        // up-down controls elevation
        int diffX = e->pos().x() - mouseX;
        int diffY = e->pos().y() - mouseY;

        if(cMode == 0)  // Perspective View
        {
            angle += diffX * mouseSpeed;
            elevation += diffY * mouseSpeed;

            QList<double> cameraP = getCameraPosition();
            xfrom = cameraP.at(0);
            yfrom = cameraP.at(1);
            zfrom = cameraP.at(2);
        }
        else    // 2D View
        {
            if(mMode == 2)  // Move
            {
                if(selectedPoint != -1) // A point is selected
                {
                    scene.movePoint(cMode, selectedPoint, diffX/coordToL, diffY/coordToL);
                }
            }
        }
        mouseX = e->pos().x();
        mouseY = e->pos().y();
    }
    else if(e->buttons()==Qt::RightButton)
    {
        // control the position of the camera
        // up to move toward view point
        // down to move away from view point
        int diffY = e->pos().y() - mouseY;

        xfrom -= cameraToPoint.at(0) * mouseSpeed * diffY;
        yfrom -= cameraToPoint.at(1) * mouseSpeed * diffY;
        zfrom -= cameraToPoint.at(2) * mouseSpeed * diffY;

        mouseY = e->pos().y();
    }
    updateGL();
}


void GLWidget::setFilled(bool a)
{
    filled=a;
    updateGL();
}

QList<double> GLWidget::getCameraPosition()
{
    QList<double> cameraP;
    double r = radius * cos(elevation);
    cameraP.clear();
    cameraP.append(r * cos(angle));
    cameraP.append(radius * sin(elevation));
    cameraP.append(r * sin(angle));

    return cameraP;
}

void GLWidget::setPerspectiveView()
{
    cMode = 0;

    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    glMatrixMode( GL_MODELVIEW );

    xfrom = yfrom = zfrom = 10.0;
    xup = zup = 0.0;
    yup = 1.0;
    xto = yto = zto = 0.0;
    updateGL();
}

void GLWidget::setOrthoView()
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho(-5.0f, 5.0f, -5.0f, 5.0f, 0.0f, 1500.0f);
    glMatrixMode( GL_MODELVIEW );
    xto = yto = zto = 0.0;
}

void GLWidget::setTopView()
{
    cMode = 1;

    setOrthoView();
    xfrom = zfrom = 0.0;
    yfrom = 50.0;
    xup = yup = 0.0;
    zup = -1.0;

    updateGL();
}

void GLWidget::setFrontView()
{
    cMode = 2;

    setOrthoView();
    xfrom = yfrom = 0.0;
    zfrom = 50.0;
    xup = zup = 0.0;
    yup = 1.0;

    updateGL();
}

void GLWidget::setRightView()
{
    cMode = 3;

    setOrthoView();
    yfrom = zfrom = 0.0;
    xfrom = 50.0;
    xup = zup = 0.0;
    yup = 1.0;

    updateGL();
}

void GLWidget::setMouseMode(int i)
{
    mMode = i;
}

void GLWidget::setLookTo(double x, double y, double z)
{
    xto = x;
    yto = y;
    zto = z;
}

void GLWidget::setFramePos(double t)
{
    scene.setFramePos(t);
    updateGL();
}

void GLWidget::showFrame(bool isToggled)
{
    isFrame = isToggled;
    updateGL();
}

void GLWidget::showCube(bool isToggled)
{
    isCube = isToggled;
    updateGL();
}

void GLWidget::showCylinder(bool isToggled)
{
    isCylinder = isToggled;
    updateGL();
}

void GLWidget::setCylinderR(double r)
{
    cylinderR = r;
    updateGL();
}
