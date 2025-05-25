#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormals;
layout (location = 2) in vec2 aTexCoord;

out vec3 normals;
out vec2 texCoord;
out float height;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float numberOfTiles;
uniform float maxHeight;
uniform float textureRepeat;
uniform float size;

void main()
{
	gl_Position = projection * view * model * vec4(aPos.x * size, aPos.y * maxHeight, aPos.z * size, 1.0);
	texCoord = aTexCoord * textureRepeat;
	normals = aNormals;
	height = (aPos.y + 1.0f) / 2.0f;
	//height = aPos.y;
}