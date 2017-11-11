#include "Image.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

Image::Image(std::string path, ImageType type)
{
	if (type == RGB) {
		pixels = stbi_load(path.c_str(), &w, &h, &channels, STBI_rgb);
	}
	else 
	{
		pixels = stbi_load(path.c_str(), &w, &h, &channels, STBI_rgb_alpha);
	}

	if (!pixels) {
		throw std::runtime_error("Failed to load texture image!");
	}
}

Image::~Image()
{
	stbi_image_free(pixels);
}

std::vector<unsigned char> Image::toGreyScale()
{
	monochrome.clear();
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w*3; j += 3) {
			monochrome.push_back(
				(unsigned char)
				(
					0.30 * pixels[i * w * 3 + j] +
					0.59 * pixels[i * w * 3 + j + 1] +
					0.11 * pixels[i * w * 3 + j + 2]
					)
			);
		}
	}
	return monochrome;
}

unsigned char * Image::getArray()
{
	return pixels;
}
