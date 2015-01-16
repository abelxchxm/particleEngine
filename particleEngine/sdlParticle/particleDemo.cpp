#include <cstdlib>

#include "GLEngine.h"
#include "Texture.h"
#include "Light.h"
#include "SDL/SDL.h"

#include "Button.h"
#include "ListBox.h"
#include "Slider.h"

#include "Emitter.h"

const GLsizei windowWidth = 800;
const GLsizei windowHeight = 650;

GLfloat rotateX = 45.0f;
GLfloat rotateY = 45.0f;
GLfloat zoom = -56.0f;

Uint8 *keys=NULL;

Light *mainLight = NULL;

bool navigating = false;
Control *controlled = NULL;
MouseState state;

vector<Texture *> textureList;
vector<Emitter *> emitters;

ListBox *lstTextures = NULL;
ListBox *lstEmitters = NULL;

Slider *sldLife;
Slider *sldLifeRange;
Slider *sldSize;
Slider *sldSizeRange;
Slider *sldSpread;
Slider *sldSaturation;
Slider *sldEmissionRadius;
Slider *sldEmissionRate;
Slider *sldGravity;
Slider *sldOffsetX;
Slider *sldOffsetY;
Slider *sldOffsetZ;
Slider *sldVortex;
Slider *sldAlpha;

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

void updateControls()
{
    Emitter *currentEmitter = emitters[lstEmitters->getIndex()];
    
    sldLife->setValue(&currentEmitter->life);
    sldLifeRange->setValue(&currentEmitter->lifeRange);
    sldSize->setValue(&currentEmitter->size);
    sldSizeRange->setValue(&currentEmitter->sizeRange);
    sldSpread->setValue(&currentEmitter->spread);
    sldSaturation->setValue(&currentEmitter->saturation);
    sldEmissionRadius->setValue(&currentEmitter->emissionRadius);
    sldEmissionRate->setValue(&currentEmitter->emissionRate);
    sldGravity->setValue(&currentEmitter->gravity);
    sldOffsetX->setValue(&currentEmitter->position.x);
    sldOffsetY->setValue(&currentEmitter->position.y);
    sldOffsetZ->setValue(&currentEmitter->position.z);
    sldVortex->setValue(&currentEmitter->rotation.y);
    sldAlpha->setValue(&currentEmitter->alpha);
}

void initializeControls()
{
    textureList.push_back(new Texture("data/Star.tga", "Star"));
    textureList.push_back(new Texture("data/Flare.tga", "Flare"));
    textureList.push_back(new Texture("data/Flame.tga", "Flame"));
    textureList.push_back(new Texture("data/Smoke.tga", "Smoke",MODE_DARKEN));

    Emitter *emitter = new Emitter();
    emitters.push_back(emitter);
    emitter->setTexture(textureList[0]);

    sldLife = (Slider *)addControl(new Slider("Life", 0.0f, 10.0f, 0, 0, 200, 20));
    sldLifeRange = (Slider *)addControl(new Slider("Life Range", 0.0f, 3.0f, 0, 0, 200, 20));
    sldSize = (Slider *)addControl(new Slider("Size", 0.0f, 10.0f, 0, 0, 200, 20));
    sldSizeRange = (Slider *)addControl(new Slider("Size Range",0.0f, 5.0f, 0, 0, 200, 20));
    sldSpread = (Slider *)addControl(new Slider("Spread", 0.0f, 10.0f, 0, 0, 200, 20));
    sldSaturation = (Slider *)addControl(new Slider("Saturation", 0.0f, 1.0f, 0, 0, 200, 20));
    sldEmissionRadius =(Slider *)addControl(new Slider("Emission Radius", 0.0f, 10.0f, 0, 0, 200, 20));
    sldEmissionRate = (Slider *)addControl(new Slider("Emission Rate", 0.0f, 1000.0f, 0, 0, 200, 20));
    sldGravity = (Slider *)addControl(new Slider("Gravity", 0.0f, 20.0f, 0, 0, 200, 20));
    sldOffsetX = (Slider *)addControl(new Slider("Offset X", -15.0f, 15.0f, 0, 0, 200, 20));
    sldOffsetY = (Slider *)addControl(new Slider("Offset Y", 0.0f, 15.0f, 0, 0, 200, 20));
    sldOffsetZ = (Slider *)addControl(new Slider("Offset Z", -15.0f, 15.0f, 0, 0, 200, 20));
    sldVortex = (Slider *)addControl(new Slider("Vortex", -50.0f, 50.0f, 0, 0, 200, 20));
    sldAlpha = (Slider *)addControl(new Slider("Alpha", 0.0f, 1.0f, 0, 0, 200, 20));

    lstTextures = (ListBox *)addControl(new ListBox(0, 0, 200, 100));
    for (int i = 0; i < (int)textureList.size(); i++)
        lstTextures->addItem(textureList[i]->name);

    lstEmitters = (ListBox *)addControl(new ListBox(0, 0, 200, 100));
    for( int i = 0; i < (int)emitters.size(); i++)
    {
        char text[80];
        sprintf(text, "Emitter %d", i + 1);
        lstEmitters->addItem(text);
    }

    addControl(new Button("Add", 0, 0, 100, 20));
    addControl(new Button("Delete", 0, 0, 100, 20));

    updateControls();
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
    mainLight->setDiffuse(1.0, 1.0, 1.0, 1.0);
    mainLight->setPosition(0, 50, 0);

    initializeControls();
}

void handleEvent(Control *control)
{
    if(control->getType() == "button")
    {
        Button *button = (Button *)control;

        if(button->getLabel() == "Add" && (int)emitters.size() < 6)
        {
            Emitter *emitter = new Emitter();
            emitters.push_back(emitter);

            emitter->setTexture(textureList[lstTextures->getIndex()]);

            char  text[80];
            sprintf(text, "Emitter %d", (int)emitters.size());
            lstEmitters->addItem(text);
        }
        else if(button->getLabel() == "Delete" && (int)emitters.size()> 1)
        {
            int i = 0;
            for(vector<Emitter *>::iterator it = emitters.begin(); it != emitters.end(); it++)
            {
                if( i == lstEmitters->getIndex())
                {
                    Emitter *emitter = (*it);
                    emitters.erase(it);
                    delete emitter;

                    break;
                }
                i++;
            }

            lstEmitters->removeItem(lstEmitters->getIndex());
        }
    }
    else if (control == lstTextures)
    {
        Emitter *emitter = emitters[lstEmitters->getIndex()];
        emitter->setTexture(textureList[lstTextures->getIndex()]);
    }
    else if(control == lstEmitters)
    {
        int currentEmitter = lstEmitters->getIndex();

        for(int i = 0; i < (int)textureList.size(); i++)
        {
            if(emitters[currentEmitter]->texture == textureList[i])
            {
                lstTextures->setCurrent(i);
                break;
            }
        }

        updateControls();
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

    iGLEngine->drawText(5,5,"OpenGL Demo - %.2f", fps);

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

            handleEvent(control);
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

GLvoid drawGrid()
{
    const float width = 180.0f;
    const float height = 180.0f;

    const int divisions = 100;

    float incX = width / (float)divisions;
    float incY = height / (float)divisions;

    glColor3f(0, 0, 0);
    glNormal3f(0, 1, 0);
    for (float x = -width / 2; x < width / 2; x += incX)
    {
        for (float y = -height / 2; y < height / 2; y += incY)
        {
            glBegin(GL_TRIANGLE_STRIP);
            glVertex3f(x + incX, 0, y + incY);
            glVertex3f(x, 0, y + incY);
            glVertex3f(x + incX, 0, y);
            glVertex3f(x, 0, y);
            glEnd();
        }
    }
}

GLvoid drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // PASS 1
    establishProjectionMatrix(windowWidth, windowHeight);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glTranslatef(0,0,zoom);
    glRotatef(rotateX,1,0,0);
    glRotatef(rotateY,0,1,0);

    for (int i = 0; i < (int)Light::lights.size(); i ++)
        Light::lights[i]->updateLight();
    
    glEnable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    drawGrid();

    glEnable(GL_BLEND);
    for( int i = 0; i < (int)emitters.size(); i++)
        emitters[i]->Update(SDL_GetTicks());

    glColor3f(1.0f, 1.0f, 1.0f);

    glEnable(GL_BLEND);

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
const float zoom_speed = 0.1f;

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

        if(rotateX < 2.0f)
            rotateX = 1.0f;

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
