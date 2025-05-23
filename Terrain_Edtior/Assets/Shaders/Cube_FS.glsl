#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 normals;

uniform sampler2D Wall;
uniform sampler2D Smiley_Face;
uniform float mixVal;

void main ()
{
	FragColor = mix(texture(Wall, TexCoord), texture(Smiley_Face, TexCoord), mixVal) * vec4(vec3(0.5f), 1.0f);
}