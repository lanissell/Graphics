#include <cmath>

#include "helpers/Shader.h"
#include "helpers/Texture.h"
#include "Window.h"

static float get_channel_value(float time, float offset)
{
    return std::sin(time + offset) / 2.0f + 0.5f;
}

int main()
{
    Window window;
    window.Init(800, 600, "Graphics");

    Shader shader;
    shader.Init("resources/shaders/textured.vert", "resources/shaders/textured.frag");

    Texture texture;
    texture.Init("resources/textures/container.jpg");

    GLfloat vertices[] = {
        // position           // color            // uv
         0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
         0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    while (!window.ShouldClose())
    {
        window.PollEvents();

        float time = static_cast<float>(glfwGetTime());
        float red = get_channel_value(time, 0.0f);
        float green = get_channel_value(time, 2.0f);
        float blue = get_channel_value(time, 4.0f);

        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.Use();
        texture.Bind();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window.SwapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    return 0;
}
