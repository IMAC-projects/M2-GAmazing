#version 330 core
layout (location = 0) in vec3 aPos;

uniform float zoom;

void main()
{
    gl_Position = vec4(aPos.x * zoom, aPos.y * zoom, aPos.z, 1.0);
}