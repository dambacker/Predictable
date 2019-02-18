#version 330 core

layout(location = 0) in vec4 position;

out vec2 texcoords;

uniform mat4 modelViewProjection;

void main()
{
    gl_Position = modelViewProjection * vec4(position.x, position.y, 0, 1);

	texcoords = vec2(position.z, position.w);
}

