#version 330 core
out vec4 FragColor;

in vec2 texCoord;
in float height;

#define TEXTURE 0
#define COLOR 1
#define TEXTURE_COLOR 2

#define MIX 0
#define SMOOTHSTEP 1

//Sand
uniform sampler2D sand;
uniform vec4 sandColor;
uniform float sandHeight;
//Grass
uniform sampler2D grass;
uniform vec4 grassColor;
uniform float grassHeight;
//Rock
uniform sampler2D rock;
uniform vec4 rockColor;
uniform float rockHeight;
//Ice
uniform sampler2D ice;
uniform vec4 iceColor;
uniform float iceHeight;


uniform int colorType;
uniform int factorCalculationMethod;
uniform float colorBlending;

vec4 mixTextures(sampler2D tex1, sampler2D tex2, float tex1Height, float tex2Height)
{
	vec4 tex1Color = texture(tex1, texCoord);
	vec4 tex2Color = texture(tex2, texCoord);
	float factor;

	if(factorCalculationMethod == MIX)
	{
		float delta = tex2Height - tex1Height;
		factor = (height - tex1Height) / delta;
	}
	else if (factorCalculationMethod == SMOOTHSTEP)
	{
		factor = smoothstep(tex1Height, tex2Height, height);
	}
	
	return mix(tex1Color, tex2Color, factor);
}

vec4 mixColors(vec4 color1, vec4 color2, float color1Height, float color2Height)
{
	float factor;

	if(factorCalculationMethod == MIX)
	{
		float delta = color2Height - color1Height;
		factor = (height - color1Height) / delta;
	}
	else if (factorCalculationMethod == SMOOTHSTEP)
	{
		factor = smoothstep(color1Height, color2Height, height);
	}
	
	return mix(color1, color2, factor);
}

void main ()
{
	vec4 finalColor;

	//Sand
	if(height < sandHeight)
	{
		if(colorType == TEXTURE)
		{
			finalColor = texture(sand, texCoord);
		}
		else if (colorType == COLOR)
		{
			finalColor = sandColor;
		}
		else if (colorType == TEXTURE_COLOR)
		{
			finalColor = mix(texture(sand, texCoord), sandColor, colorBlending);
		}
	}
	//Sand - Grass
	else if (height < grassHeight)
	{
		if(colorType == TEXTURE)
		{
			finalColor = mixTextures(sand, grass, sandHeight, grassHeight);
		}
		else if (colorType == COLOR)
		{
			finalColor = mixColors(sandColor, grassColor, sandHeight, grassHeight);
		}
		else if (colorType == TEXTURE_COLOR)
		{
			vec4 textureColor = mixTextures(sand, grass, sandHeight, grassHeight);
			vec4 baseColor = mixColors(sandColor, grassColor, sandHeight, grassHeight);
			finalColor = mix(textureColor, baseColor, colorBlending);
		}
	}
	//Grass - Rock
	else if (height < rockHeight)
	{
		if(colorType == TEXTURE)
		{
			finalColor = mixTextures(grass, rock, grassHeight, rockHeight);
		}
		else if (colorType == COLOR)
		{
			finalColor = mixColors(grassColor, rockColor, grassHeight, rockHeight);
		}
		else if (colorType == TEXTURE_COLOR)
		{
			vec4 textureColor = mixTextures(grass, rock, grassHeight, rockHeight);
			vec4 baseColor = mixColors(grassColor, rockColor, grassHeight, rockHeight);
			finalColor = mix(textureColor, baseColor, colorBlending);
		}
	}
	//Rock - Ice
	else if (height < iceHeight)
	{
		if(colorType == TEXTURE)
		{
			finalColor = mixTextures(rock, ice, rockHeight, iceHeight);
		}
		else if (colorType == COLOR)
		{
			finalColor = mixColors(rockColor, iceColor, rockHeight, iceHeight);
		}
		else if (colorType == TEXTURE_COLOR)
		{
			vec4 textureColor = mixTextures(rock, ice, rockHeight, iceHeight);
			vec4 baseColor = mixColors(rockColor, iceColor, rockHeight, iceHeight);
			finalColor = mix(textureColor, baseColor, colorBlending);
		}
	}
	//Ice
	else
	{
		if(colorType == TEXTURE)
		{
			finalColor = texture(ice, texCoord);
		}
		else if (colorType == COLOR)
		{
			finalColor = iceColor;
		}
		else if (colorType == TEXTURE_COLOR)
		{
			finalColor = mix(texture(ice, texCoord), iceColor, colorBlending);
		}
	}
	FragColor = finalColor;
}