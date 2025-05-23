#ifndef PERLIN_NOISE_H
#define PERLIN_NOISE_H

#include <stb/stb_perlin.h>
#include <stb/stb_image_write.h>

#include <glm/glm.hpp>

#include <vector>
#include <iostream>
#include <random>

class PerlinNoise {

public:
    int width;
    int height;
    int seed;
    int nChannels;
    float scale;
    int octaves;
    float persistence;
    float lacunarity;
    glm::vec2 offset;
    std::vector<unsigned char> image;
    std::vector<float> heightMap;

    PerlinNoise();

    void generateNoise();

    void saveImage(const char* filename);
    void deleteImage();
    void deleteHeightMap();
};

#endif