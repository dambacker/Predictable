#version 330 core

in vec2 texcoords;

out vec4 color;

uniform sampler2D textureSampler;

void main()
{
	color.rgb = texture( textureSampler, texcoords ).rgb;
	color.a = 0.5;
}