#pragma once

#include <GL/glew.h>

class Texture
{
public:
    ~Texture();

    void Init(const char* path);
    void Bind();

private:
    GLuint texture = 0;
};
