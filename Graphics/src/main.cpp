#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    shader.BindUniformBlock("Constants", 0);

    Texture texture;
    texture.Init("resources/textures/container.jpg");

    GLfloat vertices[] = {
        // position            // uv         // face
        // front
        -0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   0.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   0.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   0.0f,
        -0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   0.0f,
        // back
         0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   1.0f,
        -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   1.0f,
        -0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   1.0f,
         0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   1.0f,
        // left
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   2.0f,
        -0.5f, -0.5f,  0.5f,   1.0f, 0.0f,   2.0f,
        -0.5f,  0.5f,  0.5f,   1.0f, 1.0f,   2.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   2.0f,
        // right
         0.5f, -0.5f,  0.5f,   0.0f, 0.0f,   3.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   3.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   3.0f,
         0.5f,  0.5f,  0.5f,   0.0f, 1.0f,   3.0f,
        // top
        -0.5f,  0.5f,  0.5f,   0.0f, 0.0f,   4.0f,
         0.5f,  0.5f,  0.5f,   1.0f, 0.0f,   4.0f,
         0.5f,  0.5f, -0.5f,   1.0f, 1.0f,   4.0f,
        -0.5f,  0.5f, -0.5f,   0.0f, 1.0f,   4.0f,
        // bottom
        -0.5f, -0.5f, -0.5f,   0.0f, 0.0f,   5.0f,
         0.5f, -0.5f, -0.5f,   1.0f, 0.0f,   5.0f,
         0.5f, -0.5f,  0.5f,   1.0f, 1.0f,   5.0f,
        -0.5f, -0.5f,  0.5f,   0.0f, 1.0f,   5.0f
    };

    GLuint indices[] = {
         0,  1,  2,    2,  3,  0,
         4,  5,  6,    6,  7,  4,
         8,  9, 10,   10, 11,  8,
        12, 13, 14,   14, 15, 12,
        16, 17, 18,   18, 19, 16,
        20, 21, 22,   22, 23, 20
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    GLuint UBO;
    glGenBuffers(1, &UBO);
    glBindBuffer(GL_UNIFORM_BUFFER, UBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) + sizeof(glm::vec4), nullptr, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    while (!window.ShouldClose())
    {
        window.PollEvents();

        float time = static_cast<float>(glfwGetTime());
        float red = get_channel_value(time, 0.0f);
        float green = get_channel_value(time, 2.0f);
        float blue = get_channel_value(time, 4.0f);

        glClearColor(red, green, blue, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 model = glm::rotate(glm::mat4(1.0f), time * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        glm::mat4 mvp = projection * view * model;

        glBindBuffer(GL_UNIFORM_BUFFER, UBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(mvp));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(float), &time);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);

        shader.Use();
        texture.Bind();
        glBindBufferBase(GL_UNIFORM_BUFFER, 0, UBO);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        window.SwapBuffers();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteBuffers(1, &UBO);
    return 0;
}
