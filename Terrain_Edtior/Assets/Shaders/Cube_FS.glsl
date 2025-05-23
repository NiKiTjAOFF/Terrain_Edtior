#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 normals;

uniform sampler2D Wall;
uniform sampler2D Smiley_Face;

void main ()
{
	FragColor = mix(texture(Wall, TexCoord), texture(Smiley_Face, TexCoord), 0.5f) * vec4(vec3(0.5f), 1.0f);
}