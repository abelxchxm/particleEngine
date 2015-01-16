#ifndef SHADER_H
#define SHADER_H

#define GLEW_STATIC
#include <GL/glew.h>

#include <string.h>
#include <iostream>
#include "Texture.h"

class Shader
{
public:
    Shader(string vertexFile, string fragFile);

    void useShader();

    void setAttribute1i(string name, int x);
    void setAttribute1f(string name, float x);
    void setAttribute2f(string name, float x, float y);
    void setAttribute3f(string name, float x, float y, float z);
    void setAttribute4f(string name, float x, float y, float z, float w);

    static void disableShaders();

private:
    char *readFile(const char *fn);

    
private:
    GLhandleARB vertexShader;
    GLhandleARB fragmentShader;

    GLhandleARB program;
};

#endif
