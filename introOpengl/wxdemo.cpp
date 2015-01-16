#include "wxdemo.h"

IMPLEMENT_APP(GLApp)

bool GLApp::OnInit()
{
    GLFrame *frame = new GLFrame(NULL,_T("OpenGL Demo"),wxDefaultPosition,wxSize(windowWidth,windowHeight));
    frame->Show(true);

    return true;
}

GLFrame::GLFrame(wxWindow *parent, const wxString &title, const wxPoint &pos, const wxSize &size, long style)
    : wxFrame(parent, wxID_ANY, title, pos, size, style)
{
    canvas = new GLCanvas(this);
}

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_IDLE (GLCanvas::OnIdle)
    EVT_PAINT (GLCanvas::OnPaint)
    EVT_ERASE_BACKGROUND (GLCanvas::OnEraseBackground)
    EVT_KEY_UP (GLCanvas::OnKeyUp)
    EVT_KEY_DOWN (GLCanvas::OnKeyDown)

    EVT_TIMER (wxID_ANY, GLCanvas::OnTimer)
END_EVENT_TABLE()

GLCanvas::GLCanvas(wxWindow *parent, wxWindowID id,
        const wxPoint &pos,
        const wxSize &size,
        long style, const wxString &name)
    : wxGLCanvas(parent, (wxGLCanvas *)NULL, id, pos, size, style | wxFULL_REPAINT_ON_RESIZE, name)
{
    cubeRotateX = 45.0f;
    cubeRotateY = 45.0f;
    
    initialized = false;

    for(int i= 0; i< 500; i++)
        keys[i] = false;
    timer = new wxTimer(this,wxID_ANY);
    timer->Start(1);
};

/*GLCanvas::~GLCanvas()
{
}*/

GLvoid GLCanvas::establishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0,0,width,height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height,0.1f,200.0f);
}

GLvoid GLCanvas::initGL(GLsizei width, GLsizei height)
{
    establishProjectionMatrix(width, height);
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
   glEnable(GL_PERSPECTIVE_CORRECTION_HINT);
}

GLvoid GLCanvas::displayFPS()
{
    static long lastTime=(long)wxGetLocalTimeMillis().GetLo();
    static long loops = 0;
    static GLfloat fps = 0.0f;
    int newTime = (long)wxGetLocalTimeMillis().GetLo();
    if(newTime - lastTime > 100)
    {
        float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;
        fps = (fps + newFPS) / 2.0f;
        char title[80];
        sprintf(title,"OpenGL Demo - %.2f", fps);
        GetParent()->SetLabel(title);
        lastTime = newTime;
        loops = 0;
    }
    loops++;
}

GLvoid GLCanvas::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(0,0,-5.0f);
    glRotatef(cubeRotateX,1,0,0);
    glRotatef(cubeRotateY,0,1,0);
    // Draw cube
    glBegin(GL_QUADS);
        // Top face
        glColor3f(1.0f,0.5f,0.0f);
        glVertex3f(1.0f,1.0f,-1.0f);
        glVertex3f(-1.0f,1.0f,-1.0f);
        glVertex3f(-1.0f,1.0f,1.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        // Bottom face
        glColor3f(0.0f,1.0f,0.0f);
        glVertex3f(1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,1.0f);
        glVertex3f(1.0f,-1.0f,1.0f);
        // Front face
        glColor3f(1.0f,0.0f,0.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        glVertex3f(-1.0f,1.0f,1.0f);
        glVertex3f(-1.0f,-1.0f,1.0f);
        glVertex3f(1.0f,-1.0f,1.0f);
        // Back face
        glColor3f(1.0f,1.0f,0.0f);
        glVertex3f(1.0f,1.0f,-1.0f);
        glVertex3f(-1.0f,1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(1.0f,-1.0f,-1.0f);
        // Left face 
        glColor3f(0.0f,0.0f,1.0f);
        glVertex3f(-1.0f,1.0f,1.0f);
        glVertex3f(-1.0f,1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glVertex3f(-1.0f,-1.0f,1.0f);
        // Right face 
        glColor3f(1.0f,0.0f,1.0f);
        glVertex3f(1.0f,1.0f,1.0f);
        glVertex3f(1.0f,1.0f,-1.0f);
        glVertex3f(1.0f,-1.0f,-1.0f);
        glVertex3f(1.0f,-1.0f,1.0f);
    glEnd();
    glFlush();
    // This line IS  MODIFIED in other window interfaces
    SwapBuffers();
    displayFPS();
}

GLboolean GLCanvas::checkKeys()
{
    static long lastTime = (long)wxGetLocalTimeMillis().GetLo();
    const GLfloat speed=1.0f;
    const long updateTime = 10;
    if(keys[WXK_ESCAPE])
        return true;
    long newTime = (long)wxGetLocalTimeMillis().GetLo();

    if(newTime - lastTime > updateTime){
        if(keys[WXK_LEFT])
            cubeRotateY -= speed;
        if(keys[WXK_RIGHT])
            cubeRotateY += speed;
        if(keys[WXK_UP])
            cubeRotateX -= speed;
        if(keys[WXK_DOWN])
            cubeRotateX += speed;
    }
    return false;
}

void GLCanvas::OnIdle(wxIdleEvent &event)
{
    Refresh();
}

void GLCanvas::OnPaint(wxPaintEvent &event)
{
    if(checkKeys())
        GetParent()->Close(true);
    wxPaintDC dc(this);

    //SetCurrent();
    if(!initialized)
    {
        initGL(GetSize().GetWidth(),GetSize().GetHeight());
        initialized = true;
    }

    drawScene();
}

void GLCanvas::OnEraseBackground(wxEraseEvent &event)
{
    // Do nothing here to prevent flickering
}

void GLCanvas::OnKeyDown(wxKeyEvent &event)
{
    long key = event.GetKeyCode();
    keys[key] = true;
    event.Skip();
}

void GLCanvas::OnKeyUp(wxKeyEvent &event)
{
    long key = event.GetKeyCode();
    keys[key] = false;
    event.Skip();
}

void GLCanvas::OnTimer(wxTimerEvent &event)
{
    Refresh();
}

