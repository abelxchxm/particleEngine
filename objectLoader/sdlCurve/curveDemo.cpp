#include <stdlib.h>
#include <math.h>

#include "GLEngine.h"
#include "Texture.h"
#include "Light.h"
#include "SDL/SDL.h"

#include "Button.h"
#include "ListBox.h"
#include "Slider.h"

const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;

GLfloat rotateX = 23.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -6.0f;

Uint8 *keys=NULL;

bool navigating = false;
Control *controlled = NULL;
MouseState state;

GLfloat ctrlPoints[4][3];

int hovered = -1;
int selected = -1;

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height,0.1f,200.0f);
}

GLvoid setOrtho(GLsizei width, GLsizei height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0, width, height, 0);
}

GLvoid initGL(GLsizei width, GLsizei height)
{
    iGLEngine->Initialize(width, height);

    establishProjectionMatrix(width, height);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glEnable(GL_PERSPECTIVE_CORRECTION_HINT);   

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    ctrlPoints[0][0] = 50;
    ctrlPoints[0][1] = 50;
    ctrlPoints[0][2] = 0;

    ctrlPoints[1][0] = 30;
    ctrlPoints[1][1] = windowHeight / 2;
    ctrlPoints[1][2] = 0;

    ctrlPoints[2][0] = 90;
    ctrlPoints[2][1] = windowHeight / 2;
    ctrlPoints[2][2] = 0;

    ctrlPoints[3][0] = windowWidth - 50;
    ctrlPoints[3][1] = 50;
    ctrlPoints[3][2] = 0;

}

GLvoid displayFPS()
{
    static long lastTime=SDL_GetTicks();

    static long loops = 0;

    static GLfloat fps = 0.0f;

    int newTime = SDL_GetTicks();

    if(newTime - lastTime > 100)
    {
        float newFPS = (float)loops / float(newTime - lastTime) * 1000.0f;

        fps = (fps + newFPS) / 2.0f;

        lastTime = newTime;

        loops = 0;
    }

    iGLEngine->drawText(630,20,"OpenGL Demo - %.2f", fps);

    loops++;
}

GLvoid drawControls()
{
    for (list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++)
    {
        Control *control = (*it);

        control->drawControl();

        if((controlled != NULL && controlled != control)||navigating)
            continue;

        if(control->updateControl(state))
        {
            controlled = control;

            //Handle events
        }
        else if( control == controlled)
        {
            controlled = NULL;
        }
        else
        {
            // Control has been updated...but no messages
        }
    }
}

GLvoid drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    setOrtho(windowWidth, windowHeight);

    glEnable(GL_MAP1_VERTEX_3);

    glDisable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0f, 3, 4,&ctrlPoints[0][0]);
    glLineWidth(1.5f);

    const int numSegs = 40;
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_STRIP);
    for(int i = 0; i <= numSegs; i++)
    { 
        glEvalCoord1f((float)i/ (float)numSegs);
    } 
    glEnd();

    for(int i = 0; i < 4; i++)
    {
        if( i == selected)
        {

            glPointSize(6.0f);
            glColor3f(1.0f, 0.0f, 0.0f);
        }
        else if( i == hovered)
        {
            glPointSize(5.5f);
            glColor3f(1.0f, 0.5f, 0.5f);
        }
        else
        {
            glPointSize(5.0f);
            glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        }

        glBegin(GL_POINTS);
        glVertex3f(ctrlPoints[i][0], ctrlPoints[i][1], ctrlPoints[i][2]);
        glEnd();
    }

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glDisable(GL_LIGHTING);

    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    displayFPS();

    glFlush();

    // This line IS  MODIFIED in other window interfaces
    SDL_GL_SwapBuffers();
}

const float rotateX_speed = 0.5f;
const float rotateY_speed = 0.5f;
const float zoom_speed = 0.01f;

GLboolean updateNavigation()
{
    hovered = -1;
    for(int i = 0; i < 4; i++)
    {
        if(fabs(ctrlPoints[i][0] - state.x) < 5 &&
                fabs(ctrlPoints[i][1] - state.y) < 5)
        {
            hovered = i;
            break;
        }
    }
    if(state.LeftButtonDown)
    {
        if(!navigating)
        {
            selected = hovered;
            SDL_WM_GrabInput(SDL_GRAB_ON);
        }

        if(selected != -1)
        {
            ctrlPoints[selected][0] = (GLfloat)state.x;
            ctrlPoints[selected][1] = (GLfloat)state.y;
        }

        navigating = true;
    }
    else if(navigating && !state.LeftButtonDown)
    {
        selected = -1;

        navigating = false;
        SDL_WM_GrabInput(SDL_GRAB_OFF);
    }

    if(keys[SDLK_ESCAPE])
        return true;

    return false;
}

int main(int argc, char **argv)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr,"Unable to initialize SDL %s", SDL_GetError());
        exit(1);
    } 

    if (SDL_SetVideoMode(windowWidth, windowHeight, 0, SDL_OPENGL) == NULL)
    {
        fprintf(stderr,"Unable to create OpenGL Scene: %s", SDL_GetError());
        exit(2);
    }

    initGL(windowWidth, windowHeight);

    int done = 0;

    while(!done)
    {
        SDL_GetMouseState(&state.x, &state.y);

        state.LeftButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(1);
        state.MiddleButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(2);
        state.RightButtonDown = SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(3);

        drawScene();

        SDL_Event event;
        while(SDL_PollEvent(&event))
        {
            if(event.type == SDL_QUIT)
                done=1;
            keys = SDL_GetKeyState(NULL);
        }
        if(updateNavigation())
            done = 1;
    }

    for(list<Control *>::iterator it = Control::controls.begin(); it != Control::controls.end(); it++)
    {
        delete (*it);

        it = Control::controls.begin();
    }

    iGLEngine->Uninitialize();

    SDL_Quit();

    return 1;
}
