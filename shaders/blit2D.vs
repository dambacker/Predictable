#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texcoordIn;

out vec2 texcoords;

uniform mat4 modelViewProjection;

void main()
{
    gl_Position = modelViewProjection * vec4(position, 0, 1);

	texcoords = texcoordAttributes;
}

