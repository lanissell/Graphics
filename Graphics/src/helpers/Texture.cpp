#include "Texture.h"

#include <stb_image.h>

Texture::~Texture()
{
    glDeleteTextures(1, &texture);
}

void Texture::Init(const char* path)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, channels;
    unsigned char* pixels = stbi_load(path, &width, &height, &channels, 4);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(pixels);
}

void Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}
