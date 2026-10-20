#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Window
{
public:
    ~Window();

    void Init(int width, int height, const char* title);
    bool ShouldClose();
    void PollEvents();
    void SwapBuffers();

private:
    GLFWwindow* window = nullptr;
};
