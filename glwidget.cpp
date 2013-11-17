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
    filled=false;

    angle = M_PI/4.0;
    elevation = M_PI/4.0;
    radius = 10.0;
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
    glClear( GL_COLOR_BUFFER_BIT );

    glLoadIdentity();
    gluLookAt(xfrom,yfrom,zfrom, xto, yto, zto, 0.0, 1.0, 0.0);
   // glTranslatef( 0.0, 0.0, -10.0 );
   // glScalef( scale, scale, scale );



   // glCallList( object );   no display list this version just make the cube
    drawGround();
    makeDice();
}

/* 2D */
void GLWidget::resizeGL( int w, int h )
{
    glViewport( 0, 0, (GLint)w, (GLint)h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glFrustum( -1.0, 1.0, -1.0, 1.0, 5.0, 1500.0 );
    glMatrixMode( GL_MODELVIEW );
}

void GLWidget::about()
{
    QString vnum;
    QString mess, notes;
    QString title="QtOpenGl-2 ";

    vnum.setNum (version );
    mess="Simple OpenGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\n\n News: No News.";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
}

void GLWidget::help()
{
    QString vnum;
    QString mess, notes;
    QString title="Cube ";

    vnum.setNum ( version );
    mess="Simple Interface to openGl and Qt by Brian Wyvill Release Version: ";
    mess = mess+vnum;
    notes = "\nThis version driven from the GLwidget. \n \
            Draws a cube, allows crude camera control and Euler angle rotations \n   ";
    mess = mess+notes;
    QMessageBox::information( this, title, mess, QMessageBox::Ok );
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

    // glEnable(GL_DEPTH_TEST);
    // glDisable(GL_LIGHTING);

}

GLuint GLWidget::makeDice( )
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

void GLWidget::drawFace( int tim, float w)
{
    // this version no texturing
 //   glTexImage2D( GL_TEXTURE_2D, 0, 3, tex[tim].width(), tex[tim].height(), 0,
           //       GL_RGBA, GL_UNSIGNED_BYTE, tex[tim].bits() );
    glLineWidth(2);
    if (filled) glBegin( GL_POLYGON ); else glBegin( GL_LINE_LOOP );
    //glTexCoord2f(0.0, 0.0);

    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(  -w,  -w, w );
   // glTexCoord2f(0.0, 1.0);
    glVertex3f(   w,  -w, w );
    //glTexCoord2f(1.0, 1.0);
    glVertex3f(   w,   w, w );
    //glTexCoord2f(1.0, 0.0);
    glVertex3f(  -w,   w, w );

    glEnd();

}

void GLWidget::drawGround()
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

void GLWidget::makeSpots(int tim, QImage *buf)
{
  int r=255, g=0, b=0;
  int rad=25;
  int w,h,i,j;

  w=buf->width();
  h=buf->height();

  // set red
  for (i=0; i<buf->width(); i++)
    for (j=0; j<buf->height(); j++)
      buf->setPixel(i,j, qRgb(r, g, b));

  switch(tim) {
  case 0: // value 1
    drawCircle(rad, w/2, h/2, buf);
    break;

  case 1: // value 2
    drawCircle(rad, w/4, h/4, buf);
    drawCircle(rad, 3*w/4, 3*h/4, buf);
    break;


  case 2: // value 3
    drawCircle(rad, w/4, h/4, buf);
    drawCircle(rad, w/2, h/2, buf);
    drawCircle(rad, 3*w/4, 3*h/4, buf);
    break;

  case 3: // value 4
    drawCircle(rad, w/4, h/4, buf);
    drawCircle(rad, w/4, 3*h/4, buf);
    drawCircle(rad, 3*w/4, 3*h/4, buf);
    drawCircle(rad, 3*w/4, h/4, buf);
    break;

  case 4: // value 5
    drawCircle(rad, w/4, h/4, buf);
    drawCircle(rad, w/4, 3*h/4, buf);
    drawCircle(rad, w/2, h/2, buf);
    drawCircle(rad, 3*w/4, 3*h/4, buf);
    drawCircle(rad, 3*w/4, h/4, buf);
    break;

  case 5: // value 6
    drawCircle(rad, w/4, h/4, buf);
    drawCircle(rad, w/4, h/2, buf);
    drawCircle(rad, w/4, 3*h/4, buf);

    drawCircle(rad, 3*w/4, h/4, buf);
    drawCircle(rad, 3*w/4, h/2, buf);
    drawCircle(rad, 3*w/4, 3*h/4, buf);
    break;

  default: std::cerr << " big oopsy \n";
    break;

  }
}

void GLWidget::drawCircle(int radius, int xcen, int ycen,  QImage *buf)
{
    int i,j,r2;

    r2=radius*radius;

    for(i=xcen-radius; i<xcen+radius; i++)
    for(j=ycen-radius; j<ycen+radius; j++) {
      if  ( (i-xcen)*(i-xcen) + (j-ycen)*(j-ycen) < r2)
        buf->setPixel(i,j,qRgb(255, 255, 255));
    }
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

 /*   if (df->getPan()) dopan(e->x(), height()-e->y() , true);
    else {*/

    //button =  e->button();

    if(e->buttons()==Qt::LeftButton)
    {
        mouseX = e->pos().x();
        mouseY = e->pos().y();

        // get current radius
        radius = sqrt(pow(xfrom, 2.0) + pow(yfrom, 2.0) + pow(zfrom, 2.0));
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
        // Change view point
        // All view points are pre-defined
        // View point is chosed by cycling through list
        xto = viewPoints.at(0).at(0);
        yto = viewPoints.at(0).at(1);
        zto = viewPoints.at(0).at(2);
        viewPoints.pop_front();
        viewPoints.append(viewPoints.at(0));
    }
    updateGL();
}

void GLWidget::mouseReleaseEvent( QMouseEvent *e)
{

    updateGL();
}

void GLWidget::mouseMoveEvent ( QMouseEvent *e )
{
    if(e->buttons()==Qt::LeftButton)
    {
        // left-right controls azimuth angle and
        // up-down controls elevation
        int diffX = e->pos().x() - mouseX;
        int diffY = e->pos().y() - mouseY;
        angle += diffX * mouseSpeed;
        elevation += diffY * mouseSpeed;

        QList<double> cameraP = getCameraPosition();
        xfrom = cameraP.at(0);
        yfrom = cameraP.at(1);
        zfrom = cameraP.at(2);

        //xfrom += diffX * mouseSpeed;
        //yfrom += diffY * mouseSpeed;

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

