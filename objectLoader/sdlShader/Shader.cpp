#include "Shader.h"

Shader::Shader(string vertexFile, string fragFile)
{
    vertexShader = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    fragmentShader = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    const char *vv = readFile(&vertexFile[0]);
    const char *ff = readFile(&fragFile[0]);

    if(vv != NULL && ff != NULL)
    {
        glShaderSourceARB(vertexShader, 1, &vv, NULL);
        glShaderSourceARB(fragmentShader, 1, &ff, NULL);

        delete [] vv;
        delete [] ff;

        glCompileShaderARB(vertexShader);
        glCompileShaderARB(fragmentShader);

        program = glCreateProgramObjectARB();
        glAttachObjectARB(program, vertexShader);
        glAttachObjectARB(program, fragmentShader);

        glLinkProgramARB(program);
    }
    else
    {
        std::cout << "Problem opening vertex and fragment files "<< std::endl;
    }
}

void Shader::useShader()
{
    glUseProgramObjectARB(program);
}

void Shader::setAttribute1i(string name, int x)
{
    glUniform1iARB(glGetUniformLocationARB(program, name.data()), x);
}

void Shader::setAttribute1f(string name, float x)
{
    glUniform1fARB(glGetUniformLocationARB(program, name.data()), x);
}

void Shader::setAttribute2f(string name, float x, float y)
{
    glUniform2fARB(glGetUniformLocationARB(program, name.data()), x, y);
}

void Shader::setAttribute3f(string name, float x, float y, float z)
{
    glUniform3fARB(glGetUniformLocationARB(program, name.data()), x, y, z);
}

void Shader::setAttribute4f(string name, float x, float y, float z, float w)
{
    glUniform4fARB(glGetUniformLocationARB(program, name.data()), x, y, z, w);
}

void Shader::disableShaders()
{
    glUseProgramObjectARB(0); // it was NULL
}

char *Shader::readFile(const char *fn)
{
    int length;
    char *buffer;

    ifstream file;

    file.open(fn, std::ios::binary);

    if( !file.is_open())
        return NULL;

    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);

    buffer = new char[length];

    file.read(buffer, length);

    file.close();

    return buffer;
}
