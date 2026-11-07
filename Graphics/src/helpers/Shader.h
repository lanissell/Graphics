#pragma once

#include <GL/glew.h>

class Shader
{
public:
    ~Shader();

    void Init(const char* vertexPath, const char* fragmentPath);
    void Use();
    void BindUniformBlock(const char* name, GLuint binding);

private:
    GLuint program = 0;
};
