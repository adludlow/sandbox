#version 420 core
layout (location = 0) in vec4 vertex;
layout (location = 1) in vec2 textCoords;
out vec2 TexCoords;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(vertex);
    TexCoords = textCoords;
}
