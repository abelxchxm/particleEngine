#include <stdlib.h>

#include "Texture.h"
#include "SDL/SDL.h"

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

GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;

Uint8 *keys=NULL;

Texture *texture = NULL;

GLuint gridList = 0;

GLvoid establishProjectionMatrix(GLsizei width, GLsizei height)
{
    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    gluPerspective(45.0f, (GLfloat)width / (GLfloat)height,0.1f,200.0f);
}

GLvoid initGL(GLsizei width, GLsizei height)
{
    establishProjectionMatrix(width, height);

    glShadeModel(GL_SMOOTH);

    glClearColor(0.0f,0.0f,0.0f,1.0f);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
    glEnable(GL_PERSPECTIVE_CORRECTION_HINT);   

    glEnable(GL_TEXTURE_2D);
    //glEnable(GL_LIGHTING);

    texture = new Texture("data/surface.tga", "Surface Texture");
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
        char title[80];
        sprintf(title,"OpenGL Demo - %.2f", fps);
        SDL_WM_SetCaption(title,NULL);
        lastTime = newTime;
        loops = 0;
    }

    loops++;
}

GLvoid drawCube()
{
    // Draw cube
    glBegin(GL_QUADS);
        // Top face
        glTexCoord2f(1.0f,1.0f); glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,1.0f,1.0f);

        // Bottom face
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);

        // Front face
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);

        // Back face
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,-1.0f);

        // Left face 
        glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);

        // Right face
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);
    glEnd();
}

GLvoid drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,-60.0f);
    glRotatef(cubeRotateX,1,0,0);
    glRotatef(cubeRotateY,0,1,0);

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture->texID);

    if(gridList == 0)
    {
        gridList = glGenLists(1);
        glNewList( gridList, GL_COMPILE_AND_EXECUTE);
        
        for(GLfloat x = -20.0f; x < 20.0f; x += 2.5f)
        {
            for(GLfloat y = -20.0f; y < 20.0f; y += 2.5f)
            {
                glMatrixMode(GL_MODELVIEW);
                glPushMatrix();
                
                glTranslatef(x, y, 0.0f);
                drawCube();

                glPopMatrix();
            }
        }

        glEndList();
    }
    else
    {
        glCallList(gridList);
    }

    glFlush();

    // This line IS  MODIFIED in other window interfaces
    SDL_GL_SwapBuffers();

    displayFPS();
}

GLboolean checkKeys()
{
    static long lastTime = SDL_GetTicks();
    const GLfloat speed=1.0f;
    const long updateTime = 10;
    if(keys[SDLK_ESCAPE])
        return true;
    long newTime = SDL_GetTicks();

    if(newTime - lastTime > updateTime){
        if(keys[SDLK_LEFT])
            cubeRotateY -= speed;
        if(keys[SDLK_RIGHT])
            cubeRotateY += speed;
        if(keys[SDLK_UP])
            cubeRotateX -= speed;
        if(keys[SDLK_DOWN])
            cubeRotateX += speed;
    }
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
       drawScene();
       SDL_Event event;
       while(SDL_PollEvent(&event))
       {
           if(event.type == SDL_QUIT)
               done=1;
           keys = SDL_GetKeyState(NULL);
       }
       if(checkKeys())
           done = 1;
   }

   SDL_Quit();

   return 1;
}
