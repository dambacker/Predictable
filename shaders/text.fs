#version 330 core

in vec2 texcoords;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
	color.a = 1 - texture( textureSampler, texcoords ).r;
	color.rgb = vec3(1);
}