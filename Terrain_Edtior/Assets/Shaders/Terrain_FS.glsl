#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Grass;

void main ()
{
	FragColor = texture(Grass, TexCoord);
}