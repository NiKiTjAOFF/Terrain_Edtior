#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
	: width(512), height(512), seed(0), nChannels(4), scale(20.0f),
	octaves(1), persistence(0.5f), lacunarity(1.0f), offset(glm::vec2(0.0f, 0.0f))
{}

void PerlinNoise::generateNoise()
{
	image.resize(width * height * nChannels);
	heightMap.resize(width * height);
	
	std::mt19937 prng(seed);
	std::uniform_int_distribution<int> dist(-100000, 100000);
	std::vector<glm::vec2> octaveOffsets(octaves);
	for (int i = 0; i < octaves; i++)
	{
		float offsetX = dist(prng) + offset.x;
		float offsetY = dist(prng) + offset.y;
		octaveOffsets[i] = glm::vec2(offsetX, offsetY);
	}

	float halfWidth = width / 2.0f;
	float halfHeight = height / 2.0f;

	float min = std::numeric_limits<float>::max();
	float max = std::numeric_limits<float>::lowest();

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{

			float frequency = 1.0f;
			float amplitude = 1.0f;
			float noiseHeight = 0;

			for (int i = 0; i < octaves; i++)
			{
				float sampleX = (x - halfWidth) / scale * frequency + octaveOffsets[i].x;
				float sampleY = (y - halfHeight) / scale * frequency + octaveOffsets[i].y;

				float perlinValue = stb_perlin_noise3_seed(sampleX, sampleY, 0, 0, 0, 0, seed);
				noiseHeight += perlinValue * amplitude;

				amplitude *= persistence;
				frequency *= lacunarity;
			}

			if (noiseHeight > max) {
				max = noiseHeight;
			}
			if (noiseHeight < min)
			{
				min = noiseHeight;
			}
			int index = y * width + x;
			heightMap[index] = noiseHeight;
		}
	}

	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			int index = y * width + x;
			float heightValue = (heightMap[index] - min) / (max - min);
			heightMap[index] = heightValue * 2 - 1;
			
			index *= nChannels;
			unsigned char color = (unsigned char)(heightValue * 255.0f);
			image[index] = color;
			image[index + 1] = color;
			image[index + 2] = color;
			image[index + 3] = 255;
		}
	}
}

void PerlinNoise::saveImage(const char* filename)
{
	stbi_write_png(filename, width, height, nChannels, image.data(), width * nChannels);
}

void PerlinNoise::deleteImage()
{
	image.clear();
}

void PerlinNoise::deleteHeightMap()
{
	heightMap.clear();
}