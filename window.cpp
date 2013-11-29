//-------------------------------------------------------------------------------------------
//  This is a demo. program for open gl vertex arrays
//-------------------------------------------------------------------------------------------
#include <QtGui>
#include <QtOpenGL>
#include "window.h"
//#include "dfuncs.h"

#define BIT0 0x00000001   // alpha
#define BIT1 0x00000002   // no buttons
#define BIT2 0x00000004   // set don't use native dialog

//extern dfuncs *df;

//------------------------------------------------------------------------------------
// Creates and initializes the main window for application
//------------------------------------------------------------------------------------
Window::Window(QWidget *parent):QDialog(parent)
{
    //We create an instance of GLWidget component we built in glwidget.h
    m_glWidget = new GLWidget();

    //Setup application interface. Creates all the required components and sliders.
    setupUi(this);
    disablePtMode();
    disableChkbox();
    //We need to attach our m_glWidget to glWidgetArea
    //All our drawings will be on glWidgetArea
    glWidgetArea->setWidget(m_glWidget);


}

void Window::resizeEvent( QResizeEvent * )
{
  cerr << "new size "<< width() SEP height() NL;
}

void Window::clear()
{
    m_glWidget->clear();
}

/*
void Window::aboutBut()
{
    m_glWidget->about();
}

void Window::helpBut()
{
    m_glWidget->help();
}
*/

void Window::xrot(int x)
{
    m_glWidget->rotx(x);
}

void Window::yrot(int y)
{
    m_glWidget->roty(y);
}

void Window::zrot(int z)
{
    m_glWidget->rotz(z);
}

void Window::xFrom(int a)
{
    m_glWidget->setxFrom(a);
}

void Window::yFrom(int a)
{
    m_glWidget->setyFrom(a);
}

void Window::zFrom(int a)
{
    m_glWidget->setzFrom(a);
}

void Window::filledOn(bool a)
{
    m_glWidget->setFilled(a);
}

void Window::perspectiveView()
{
    m_glWidget->setPerspectiveView();
    disablePtMode();
}

void Window::topView()
{
    m_glWidget->setTopView();
    enablePtMode();
}

void Window::frontView()
{
    m_glWidget->setFrontView();
    enablePtMode();
}

void Window::rightView()
{
    m_glWidget->setRightView();
    enablePtMode();
}

void Window::ctrlCamera()
{
    m_glWidget->setPerspectiveView();
}

void Window::addPt()
{
    m_glWidget->setMouseMode(1);
}

void Window::movePt()
{
    m_glWidget->setMouseMode(2);
}

void Window::deletePt()
{
    m_glWidget->setMouseMode(3);
}

void Window::enablePtMode()
{

    this->rb_addPt->setEnabled(true);
    this->rb_movePt->setEnabled(true);
    this->rb_deletePt->setEnabled(true);
}

void Window::disablePtMode()
{
    this->rb_addPt->setEnabled(false);
    this->rb_movePt->setEnabled(false);
    this->rb_deletePt->setEnabled(false);
    ctrlCamera();
}

void Window::moveFrame(int i)
{
    m_glWidget->setFramePos(i/100.0);
}

void Window::showFrame(bool isToggled)
{
    if(isToggled) enableChkbox();
    else disableChkbox();
    m_glWidget->showFrame(isToggled);
}

void Window::showCube(bool isToggled)
{
    m_glWidget->showCube(isToggled);
}

void Window::showCylinder(bool isToggled)
{
    m_glWidget->showCylinder(isToggled);
}

void Window::enableChkbox()
{
    this->chkbox_cube->setEnabled(true);
    this->chkbox_cylinder->setEnabled(true);
    this->slider_t->setEnabled(true);
}

void Window::disableChkbox()
{
    this->chkbox_cube->setEnabled(false);
    this->chkbox_cylinder->setEnabled(false);
    this->slider_t->setEnabled(false);

    this->chkbox_cube->setChecked(false);
    this->chkbox_cylinder->setChecked(false);

}
