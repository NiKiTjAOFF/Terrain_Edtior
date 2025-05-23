#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
	: width(512), height(512), seed(0), nChannels(4), scale(20.0f),
	octaves(1), persistence(0.5f), lacunarity(1.0f), offset(glm::vec2(0.0f, 0.0f))
{}

void PerlinNoise::generateNoise()
{
	image.resize(width * height * nChannels);
	
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

			int index = (y * width + x) * nChannels;
			unsigned char color = (unsigned char)((noiseHeight + 1.0f) * 127.5f);
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