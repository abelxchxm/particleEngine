#include <stdlib.h>

#include "Shader.h"

#include "GLEngine.h"
#include "SDL/SDL.h"

#include "Button.h"
#include "ListBox.h"
#include "Slider.h"

const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;

GLfloat rotateX = 23.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -26.0f;

Uint8 *keys=NULL;

bool navigating = false;
Control *controlled = NULL;
MouseState state;

Texture *texFloor = NULL;
Texture *texBox = NULL;

const GLfloat boxHeight = 2.0f;

Shader *lambertShader = NULL;

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

//    Light *mainLight = NULL;

//    mainLight = new Light(LIGHT_POINT);
//    mainLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
//    mainLight->setPosition(13.34f, 18.857f, 27.251f);

    glClearDepth(1.0f);
    glClearStencil(0);

    glewInit();

    texFloor = new Texture("data/ground.tga", "Surface Texture");
    texBox = new Texture("data/surface.tga", "Surface Texture");

    if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
    {
        lambertShader = new Shader("lambert.vert","lambert.frag");

        lambertShader->useShader();

        lambertShader->setAttribute1i("cloudTexture", 0);
        lambertShader->setAttribute4f("Ambient", 0.4f, 0.4f, 0.4f, 1.0f);
        lambertShader->setAttribute4f("Diffuse", 1.0f, 0.4f, 0.4f, 1.0f);
        lambertShader->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);
    }
    else
    {
        std::cout << "shaders are not working " << std::endl;
    }
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

GLvoid drawCube()
{
    lambertShader->useShader();

    glActiveTextureARB(GL_TEXTURE0_ARB);
    glBindTexture(GL_TEXTURE_2D,texBox->texID);
    glEnable(GL_TEXTURE_2D);

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
    
    Shader::disableShaders();
}

void drawFloor()
{
    const GLfloat size = 7.0f;

    glBindTexture(GL_TEXTURE_2D, texFloor->texID);

    glBegin(GL_QUADS);
        glNormal3f(0.0f, 1.0f, 0.0f);
        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-size, 0.0f, size);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-size, 0.0f, -size);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f(size, 0.0f, -size);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f(size, 0.0f, size);
    glEnd();
}

GLvoid drawScene()
{
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // PASS 1
    establishProjectionMatrix(windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,zoom);
    glRotatef(rotateX,1,0,0);
    glRotatef(rotateY,0,1,0);

    glColorMask(0, 0, 0, 0);
    glEnable(GL_STENCIL_TEST);

    glStencilFunc(GL_ALWAYS, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

    glDisable(GL_DEPTH_TEST);
    drawFloor();
    glEnable(GL_DEPTH_TEST);

    glColorMask(1, 1, 1, 1);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glStencilFunc(GL_EQUAL, 1, 1);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

    glScalef(1.0f, -1.0f, 1.0f);
    
    for(int i = 0; i < (int)Light::lights.size(); i++)
        Light::lights[i]->updateLight();

    glTranslatef(0.0f, boxHeight, 0.0f);

    lambertShader->setAttribute3f("fvLightPosition", 50.3f, -18.9f, 27.2f);

    glDisable(GL_BLEND);
    drawCube();
    glEnable(GL_BLEND);

    lambertShader->setAttribute3f("fvLightPosition", 50.3f, 18.9f, 27.2f);

    glDisable(GL_STENCIL_TEST);

    glPopMatrix();

    for(int i = 0; i < (int)Light::lights.size(); i++)
        Light::lights[i]->updateLight();

    glEnable(GL_BLEND);
    glDisable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 0.5f);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawFloor();
    glDisable(GL_BLEND);

    //glEnable(GL_LIGHTING);
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    glTranslatef(0.0f, boxHeight, 0.0f);
    drawCube();

    glDisable(GL_TEXTURE_2D);

    // PASS 2
    
    glDisable(GL_LIGHTING);
    setOrtho(windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    displayFPS();

    drawControls();

    glFlush();

    // This line IS  MODIFIED in other window interfaces
    SDL_GL_SwapBuffers();
}

const float rotateX_speed = 0.5f;
const float rotateY_speed = 0.5f;
const float zoom_speed = 0.01f;

GLboolean updateNavigation()
{
    static int lastX = -1;
    static int lastY = -1;

    if(lastX == -1 && lastY == -1)
    {
        lastX = state.x;
        lastY = state.y;
    }
    int changeX = lastX - state.x;
    int changeY = lastY - state.y;

    lastX = state.x;
    lastY = state.y;

    if (state.LeftButtonDown && controlled == NULL)
    {
        SDL_WM_GrabInput(SDL_GRAB_ON);
        rotateX -= (float)changeY * rotateX_speed;
        rotateY -= (float)changeX * rotateY_speed;

        navigating = true;
    }
    else if(state.RightButtonDown && controlled == NULL)
    {
        SDL_WM_GrabInput(SDL_GRAB_ON);
        zoom -= (float)changeX * zoom_speed;

        navigating = true;
    }
    else
    {
        SDL_WM_GrabInput(SDL_GRAB_OFF);
        navigating = false;
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

   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
   SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
   SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 1);
    
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
