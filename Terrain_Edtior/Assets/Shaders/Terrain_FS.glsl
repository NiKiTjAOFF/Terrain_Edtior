#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in float height;

uniform sampler2D sand;
uniform sampler2D grass;
uniform sampler2D rock;
uniform sampler2D snow;

//uniform float lowerBound;
//uniform float upperBound;

uniform float sandThreshold;
uniform float grassThreshold;
uniform float rockThreshold;

void main ()
{
	/*vec4 textureColor;
	float normalizedHeight = (height + 1.0f) / 2.0f;
	if((height >= -1.0f && height < -0.1f) || (height >= 0.3 && height < 0.9f))
	{
		textureColor = texture(rock, texCoord);
	}
	else if(height >= -0.1f && height < 0.1f)
	{
		textureColor = mix(texture(sand, texCoord), texture(rock, texCoord), 0.2f);
	}
	else if (height >= 0.1f && height < 0.3f)
	{
		textureColor = mix(texture(grass, texCoord), texture(sand, texCoord), 0.3f);
	}
	else if (height >= 0.9f && height <= 1.0f)
	{
		textureColor = mix(texture(snow, texCoord), texture(snow, texCoord), 0.2f);
	}
	FragColor = vec4(normalizedHeight, normalizedHeight, normalizedHeight, 1.0f) * textureColor;*/

    /*vec4 sandColor = texture(sand, texCoord);
	vec4 grassColor = texture(grass, texCoord);
	vec4 rockColor = texture(rock, texCoord);
	vec4 snowColor = texture(snow, texCoord);
	float t = smoothstep(lowerBound, upperBound, height);
	FragColor = mix(sandColor, grassColor, t);*/

	vec4 sandColor = texture(sand, texCoord);
	vec4 grassColor = texture(grass, texCoord);
	vec4 rockColor = texture(rock, texCoord);
	vec4 snowColor = texture(snow, texCoord);

	vec4 finalColor;
	if(height >= 0.0f && height < 0.3f)
	{
		finalColor = mix(sandColor, grassColor, smoothstep(0.0f, 0.3f, height));
	}
	else if(height >= 0.3f && height < 0.6f)
	{
		finalColor = mix(grassColor, rockColor, smoothstep(0.2f, 0.6f, height));
	}
	else if(height >= 0.6f && height < 1.0f)
	{
		finalColor = mix(rockColor, snowColor, smoothstep(0.9f, 1.0f, height));
	}
    FragColor = finalColor;
}