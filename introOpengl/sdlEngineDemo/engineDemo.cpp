#include <stdlib.h>

#include "GLEngine.h"
#include "Texture.h"
#include "Light.h"
#include "SDL/SDL.h"


const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;

GLfloat cubeRotateX = 45.0f;
GLfloat cubeRotateY = 45.0f;

Uint8 *keys=NULL;

Light *mainLight = NULL;
Texture *texture = NULL;

//GLuint gridList = 0;

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
    //Light::Initialize();

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

    mainLight = new Light(LIGHT_SPOT);
    mainLight->setDiffuse(2.0, 2.0, 2.0, 1.0);
    mainLight->setPosition(0, 5, 0);

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

        lastTime = newTime;

        loops = 0;
    }

    iGLEngine->drawText(5,5,"OpenGL Demo - %.2f", fps);

    loops++;
}

GLvoid drawCube()
{
    // Draw cube
    glBegin(GL_QUADS);
        // Top face
        glNormal3f(0, 1, 0);
        glTexCoord2f(2.0f,2.0f); glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,2.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(2.0f,0.0f); glVertex3f(1.0f,1.0f,1.0f);

        // Bottom face
        glNormal3f(0, -1, 0);
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,-1.0f,-1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);

        // Front face
        glNormal3f(0, 0, 1);
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);

        // Back face
        glNormal3f(0, 0, -1);
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,-1.0f);

        // Left face 
        glNormal3f(1, 0, 0);
        glTexCoord2f(1.0f,1.0f);glVertex3f(-1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(-1.0f,-1.0f,1.0f);

        // Right face
        glNormal3f(-1, 0, 0);
        glTexCoord2f(1.0f,1.0f);glVertex3f(1.0f,1.0f,1.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(1.0f,1.0f,-1.0f);
        glTexCoord2f(0.0f,0.0f); glVertex3f(1.0f,-1.0f,-1.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(1.0f,-1.0f,1.0f);
    glEnd();
}

GLvoid drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // PASS 1
    establishProjectionMatrix(windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glDisable(GL_BLEND);

    glTranslatef(0,0,-5.0f);
    glRotatef(cubeRotateX,1,0,0);
    glRotatef(cubeRotateY,0,1,0);

    for (int i = 0; i < (int)Light::lights.size(); i ++)
        Light::lights[i]->updateLight();

    glColor3f(1.0f, 1.0f, 1.0f);

    glBindTexture(GL_TEXTURE_2D, texture->texID);

    drawCube();

    // PASS 2
    
    glDisable(GL_LIGHTING);
    setOrtho(windowWidth, windowHeight);

    displayFPS();

    glFlush();

    // This line IS  MODIFIED in other window interfaces
    SDL_GL_SwapBuffers();
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

   //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
   //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

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

   iGLEngine->Uninitialize();

   SDL_Quit();

   return 1;
}
