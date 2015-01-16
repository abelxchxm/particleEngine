#ifndef GL_ENGINE_H
#define GL_ENGINE_H

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

#include "Light.h"
#include "Texture.h"
#include <cstdarg>
#include <iostream>
#include <cstring>

#define iGLEngine GLEngine::getEngine()

class GLEngine
{
public:
    GLEngine();
    ~GLEngine();

    static GLvoid Uninitialize();
    static GLEngine *getEngine();

    GLvoid Initialize(GLint width, GLint height);

    GLvoid buildTextureFont();
    GLvoid drawText(GLint x, GLint y, const char *text, ...);

    GLuint getTextWidth(const char *text);
    GLuint getTextHeight(const char *text);
private:
    GLuint fontBase;

    Texture *fontTexture;
};

#endif
