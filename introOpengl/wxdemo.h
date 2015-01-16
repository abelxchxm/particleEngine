#ifndef GL_CANVAS_H
#define GL_CANVAS_H
#include <wx/window.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>
#ifdef WIN32
    #define WIN32_LEAN_AND_MEAN
    #include<windows.h>
#endif
#if defined(__APPLE__) && defined(__MATCH__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

const GLsizei windowWidth = 500;
const GLsizei windowHeight = 500;

class GLApp : public wxApp
{
    public:
        bool OnInit();
};

class GLCanvas;

class GLFrame : public wxFrame
{
public:
    GLFrame(wxWindow *parent, const wxString &title, const wxPoint &pos,
            const wxSize &size, long style = wxDEFAULT_FRAME_STYLE);
private:
    GLCanvas *canvas;
};

class GLCanvas: public wxGLCanvas  
{
public:
    GLCanvas(wxWindow *parent, wxWindowID id = wxID_ANY,
            const  wxPoint &pos=wxDefaultPosition,
            const wxSize &size = wxDefaultSize,
            long style = 0, const wxString &name = _T("GLCanvas"));
    ~GLCanvas(){}

    // Main OpenGL member functions
    GLvoid establishProjectionMatrix(GLsizei width, GLsizei height);
    GLvoid initGL(GLsizei width, GLsizei height);
    GLvoid displayFPS();
    GLvoid drawScene();
    GLboolean checkKeys();

    // Event handles
    void OnIdle(wxIdleEvent &event);

    void OnPaint(wxPaintEvent &event);
    void OnEraseBackground(wxEraseEvent &event);

    void OnKeyDown(wxKeyEvent &event);
    void OnKeyUp(wxKeyEvent &event);

    void OnTimer(wxTimerEvent &event);
private:
    bool keys[500];

    float cubeRotateX;
    float cubeRotateY;
    bool initialized;
    wxTimer *timer;
    DECLARE_EVENT_TABLE();
};

#endif
