#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform sampler2D Wall;
uniform sampler2D Smiley_Face;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main ()
{
	//Ambient
	vec3 ambient = light.ambient * material.ambient;

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	//If it's perpendicular to a vector or goes under the surface, there's no lighting
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.diffuse * (diff * material.diffuse);

	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128);
	vec3 specular = light.specular * (spec * material.specular);

	//FragColor = mix(texture(Wall, texCoord), texture(Smiley_Face, texCoord), 0.5f) * vec4(vec3(0.5f), 1.0f);
	FragColor = vec4(vec3(ambient + diffuse + specular), 1.0f);
}