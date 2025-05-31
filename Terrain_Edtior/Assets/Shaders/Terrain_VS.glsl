#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 normal;
out vec2 texCoord;
out float height;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float numberOfTiles;
uniform float maxHeight;
uniform float textureRepeat;
uniform float size;

void main()
{
	fragPos = vec3(model * vec4(aPos.x * size, aPos.y * maxHeight, aPos.z * size, 1.0));
	gl_Position = projection * view * vec4(fragPos, 1.0f);
	texCoord = aTexCoord * textureRepeat;
	normal = aNormal;
	height = (aPos.y + 1.0f) / 2.0f;//[0, 1]
}