#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 fragPos;
out vec3 normal;
out vec2 texCoord;

uniform mat3 normalMatrix;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	fragPos = vec3(model * vec4(aPos, 1.0f));
	//Translates normal to World Space and changes it to point to a proper direction
	normal = normalMatrix * aNormal;
	texCoord = aTexCoord;

	gl_Position = projection * view * vec4(fragPos, 1.0f);
}