#version 330 core

in vec2 texCoord;
in float faceIndex;

layout (std140) uniform Constants
{
    mat4 mvp;
    float time;
};

out vec4 color;

uniform sampler2D ourTexture;

void main()
{
    vec3 faceColor = 0.5 + 0.5 * sin(time + faceIndex + vec3(0.0, 2.0, 4.0));
    color = texture(ourTexture, texCoord) * vec4(faceColor, 1.0);
}
