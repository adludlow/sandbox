#version 410 core
layout (location = 0) in vec4 aPos;

uniform mat4 transform;

void main() {
  gl_Position = transform * vec4(aPos);
}
