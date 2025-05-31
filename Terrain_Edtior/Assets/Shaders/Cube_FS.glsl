#version 330 core

struct Material{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light{
	vec3 position;
	vec4 color;
};

out vec4 FragColor;

in vec3 fragPos;
in vec3 normal;
in vec2 texCoord;

uniform Material material;
uniform Light light;

uniform vec3 viewPos;

void main ()
{
	//Ambient
	vec3 ambient = light.color.rgb * material.ambient * light.color.a;

	//Diffuse
	vec3 norm = normalize(normal);
	vec3 lightDir = normalize(light.position - fragPos);
	//If it's perpendicular to a vector or goes under the surface, there's no lighting
	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = light.color.rgb * (diff * material.diffuse);

	//Specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), material.shininess * 128);
	vec3 specular = light.color.rgb * (spec * material.specular);

	FragColor = vec4(vec3(ambient + diffuse + specular), 1.0f);
}