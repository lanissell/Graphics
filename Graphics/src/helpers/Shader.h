#pragma once

#include <GL/glew.h>

class Shader
{
public:
    ~Shader();

    void Init(const char* vertexPath, const char* fragmentPath);
    void Use();

private:
    GLuint program = 0;
};
