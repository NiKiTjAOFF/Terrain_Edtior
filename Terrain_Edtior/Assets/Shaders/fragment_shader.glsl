#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D Wall;
uniform sampler2D Smiley_Face;
uniform float mixVal;

void main ()
{
	//FragColor = texture(Wall, TexCoord);
	FragColor = mix(texture(Wall, TexCoord), texture(Smiley_Face, TexCoord), mixVal);
}