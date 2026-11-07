#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in float face;

layout (std140) uniform Constants
{
    mat4 mvp;
    float time;
};

out vec2 texCoord;
out float faceIndex;

void main()
{
    gl_Position = mvp * vec4(position, 1.0);
    texCoord = uv;
    faceIndex = face;
}
