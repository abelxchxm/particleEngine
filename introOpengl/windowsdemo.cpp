#include <wx/window.h>
#include <wx/wx.h>
#include <wx/glcanvas.h>

#ifndef DWORD
#define WINAPI
typedef unsigned long DWORD;
typedef short WCHAR;
typedef void * HANDLE;
#define MAX_PATH    PATH_MAX
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int BOOL;
#endif

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

HDC hdc= NULL;
HGLRC hRC = NULL;
HWND hWnd = NULL;
HINSTANCE hInstance;

const GLsizei windowWidth = 500;
const GLsizei windwHeight = 500;

LRESULT CALLBACK WndProc ( HWND hWnd,
        UINT uMsg,
        WPARAM wParam,
        LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_CLOSE;
        {
            PostQuitMessage(0);
        }
        default:
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
    return 0;
}

int killWindow()
{
    if(hRC)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        WRC = NULL;
    }
    if(hDC && ReleaseDC(hWnd, hDC))
    {
        // Send error message
        return FALSE;
    }
    if(!(hWnd && DestroyWindow(hWnd)))
    {
        // Send error message
        return FALSE;
    }

    UnregisterClass("OpenGL", hInstance);

    return TRUE;
}



BOOL createGLWindow(char *title, int width, int height, int bits)
{
    GLuint PixelFormat;
    WNDCLASS wc;
    DWORD dwExStyle;
    DWORD dwStyle;
    RECT WindowRect;
    WindowRect.left = 0;
    WindowRect.right = width;
    WindowRect.top = 0;
    WindowRect.botton = height;
    hInstance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wclpfnWndProc = (WNDPROC) WndProc;

    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL,IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = "OpenGL";
    RegisterClass(&wc);
    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPEDWINDOW;
    AdjustWindowRectExt(&WindowRect, dwStyle, FALSE, dwExStyle);
    hWnd = CreateWindowEx(dwExStyle,
            "OpenGL",
            title,
            dwStyle,
            WS_CLIPSIBLINGS |
            WS_CLIPCHILDREN,
            0, 0, width, height,
            NULL,
            NULL,
            hInstance,
            NULL);

    static PIXELFORMATDESCRIPTOR pfd 
    {
        sizeof(PIXELFORMATDESCRIPTOR),
            1,
            PDF_DRAW_TO_WINDOW |
            PDF_SUPPORT_OPENGL |
            PDF_DOUBLEBUFFER,
            PDF_TYPE_RGBA,
            bits,
            0, 0, 0, 0, 0, 0,
            0,
            0,
            0,
            0, 0, 0, 0,
            16,
            0,
            0,
            PDF_MAIN_PLANE,
            0,
            0, 0, 0
    };

    hDC = GetDC(hWnd);

    PixelFormat = ChoosePixelFormat(hDC, &pfd);
    SetPixelFormat(hDC, PixelFormat, &pfd);

    hRC = wglCreateContext(hDC);
    wglMakeCurrent(hDC, hRC);
    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance,
        HINSTANCE hPrevInstance,
        LPSTR lpCmdLine,
        int nCmdShow)
{
    MSG msg;
    int done = 0;

    if(!createGLWindow("OpenGL Demo", windowWidth, windowHeight))
    {
        killWindow();
        return 0;
    }

    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) )
        {
            if(msg.message == WM_QUIT)
                done = TRUE;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
    }

    killWindow();
}
