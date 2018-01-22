#include "HeightMap.h"


HeightMap::HeightMap(std::string filename, uint32_t patchsize, float ratio, uint16_t zScale)
{
	Image image(filename, RGB);
	heightMap = image.toGreyScale();
	if (image.h * image.w != heightMap.size()) {
		throw std::runtime_error("Error while loading file.");
		std::exit(-1);
	}

	patch_w = patchsize;
	w = image.w;
	scaleW = w;
	scaleW /= patch_w;

	patch_h = patchsize;
	h = image.h;
	scaleH = h;
	scaleH /= patch_h;

	this->ratio = ratio;

	if (ratio == 0) {
		this->ratio = (float)w / (float)h;
	}

	this->zScale = zScale;
};


HeightMap::~HeightMap()
{
}


float HeightMap::getHeight(uint32_t x, uint32_t y)
{
	glm::fvec2 rpos = glm::fvec2((float)x, (float)y) * glm::fvec2(scaleW, scaleH);
	rpos.x = std::max(0, std::min((int)rpos.x, (int)w - 1));
	rpos.y = std::max(0, std::min((int)rpos.y, (int)h - 1));
	if (rpos.x >15 || rpos.y >15) {
		int a = 0;
	}
	return ((float)heightMap[rpos.y * w + w - 1 - rpos.x] / 255.0f);
}

std::vector<Vertex> HeightMap::generateVertices()
{
	std::vector<Vertex> vertices;

	

	float wx = ZOOM;
	float wy = ZOOM;

	if (ratio > 1) {
		wy = (1/ratio)*ZOOM;
	}
	else {
		wx = ratio*ZOOM;
	}


	int index = 0;
	for (auto x = 0; x < patch_w; x++)
	{

		for (auto y = 0; y < patch_h; y++)
		{
			Vertex vertex;
			vertex.pos[0] = (x * wx + wx / 2.0f) / (float)patch_w - wx / 2.0f;
			vertex.pos[1] = (y * wy + wy / 2.0f) / (float)patch_h - wy / 2.0f;
			vertex.pos[2] = -0.5f + getHeight(x, y) * (float)zScale / 255.0f;
			vertex.color[0] = 1.0f;
			vertex.color[1] = 1.0f;
			vertex.color[2] = 1.0f;
			vertex.texPos[0] = (x * wx + wx / 2.0f) / (float)patch_w - wx / 2.0f;
			vertex.texPos[1] = (y * wy + wy / 2.0f) / (float)patch_h - wy / 2.0f;

			vertex.normal.x = getHeight(x-1, y) - getHeight(x + 1, y);

			vertex.normal.y = getHeight(x, y-1) - getHeight(x + 1, y+1);

			vertex.normal.z = 2.0;

			vertex.normal = glm::normalize(vertex.normal);

			vertices.push_back(vertex);
			index++;
		}
	}

	return vertices;
}

std::vector<uint32_t> HeightMap::generateIndices()
{
	const uint32_t wInd = (patch_w - 1);
	const uint32_t hInd = (patch_h - 1);

	uint32_t *indicesArr = new uint32_t[wInd * hInd * 6];
	for (auto x = 0; x < wInd; x++)
	{
		for (auto y = 0; y < hInd; y++)
		{
			uint32_t index = (x + y * wInd) * 6;
			if (((x*y) % 2) == 0) {
				indicesArr[index] = (x + y * patch_w);
				indicesArr[index + 1] = indicesArr[index] + patch_w;
				indicesArr[index + 2] = indicesArr[index + 1] + 1;
				indicesArr[index + 3] = indicesArr[index + 2];
				indicesArr[index + 4] = indicesArr[index] + 1;
				indicesArr[index + 5] = indicesArr[index];
			}
			else {
				indicesArr[index] = (x + y * patch_w) + patch_w;
				indicesArr[index + 1] = indicesArr[index] + 1;
				indicesArr[index + 2] = indicesArr[index + 1] - patch_w;
				indicesArr[index + 3] = indicesArr[index + 2];
				indicesArr[index + 4] = indicesArr[index] - patch_w;
				indicesArr[index + 5] = indicesArr[index];
			}
		}
	}
	std::vector<uint32_t> indices;
	for (auto i = 0; i < (wInd * hInd * 6); i++)
	{
		indices.push_back(indicesArr[i]);
	}
	return indices;
}

void HeightMap::setVertices(std::vector<Vertex> vertices)
{
	heightMap.clear();
	for (auto x = 0; x < patch_w; x++)
	{
		for (auto y = 0; y < patch_h; y++)
		{
			heightMap.push_back(int((0.5f + vertices[x + patch_w * y].pos[2]) * 255.0f * 255.0f / (float) zScale));
		}
	}
}

void HeightMap::saveAsImage(std::string path)
{
	Image image(heightMap, patch_w, patch_h);

	image.saveToFile(path);	
}

