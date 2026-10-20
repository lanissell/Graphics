#include "Shader.h"

#include <fstream>
#include <sstream>
#include <string>

static std::string read_file(const char* path)
{
    std::ifstream file(path);
    std::stringstream stream;
    stream << file.rdbuf();
    return stream.str();
}

static GLuint compile_shader(GLenum type, const char* path)
{
    std::string code = read_file(path);
    const GLchar* source = code.c_str();

    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    return shader;
}

Shader::~Shader()
{
    glDeleteProgram(program);
}

void Shader::Init(const char* vertexPath, const char* fragmentPath)
{
    GLuint vertexShader = compile_shader(GL_VERTEX_SHADER, vertexPath);
    GLuint fragmentShader = compile_shader(GL_FRAGMENT_SHADER, fragmentPath);

    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Use()
{
    glUseProgram(program);
}
