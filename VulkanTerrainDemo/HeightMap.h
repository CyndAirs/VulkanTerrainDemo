#pragma once
#include "Shared.h"
#include "Image.h"
#include "Vertex.h"


#include <vector>
#include <algorithm>

/**
* Class holding the height map for the model and generating the model from it
*/
class HeightMap
{
public:
	
	/** 
	* Constructor, sets filename, size of patch, ratio of patch and z-scale
	* @param filename file path
	* @param patchsize size of generated model in vertices
	* @param ratio ratio of model lenght and width, 0 no default based on image
	* @param zScale scale for model height 1 (flat model) to 255 (up to max height)
	*/
	HeightMap::HeightMap(std::string filename, uint32_t patchsize, float ratio, uint16_t zScale);
	
	~HeightMap();

	/** 
	* Generates height based image on coordinates x and y normalised to the size of model,
	* @param x x coordinate
	* @param y y coordinate
	* @returns height value in given point 
	*/
	float getHeight(uint32_t x, uint32_t y);

	/** 
	* Generates vertices with default values 
	* @returns vector of vertex objects 
	*/
	std::vector<Vertex> generateVertices();

	/**
	* Generates indexes for polygon in order to save memory
	* returns vector of integers containing indexes 
	*/
	std::vector<uint32_t> generateIndices();


private:
	/**
	* Vector containg height map image
	*/
	std::vector<unsigned char> heightMap;
	/**
	* Scale of vertices in row to pixels on height map in width
	*/
	float scaleW;
	/**
	* Width of map image
	*/
	uint32_t w;
	/**
	* Number of vertices in width on the model
	*/
	uint32_t patch_w;
	/**
	* Scale of vertices in row to pixels on height map in height
	*/
	float scaleH;
	/**
	* Height of map image
	*/
	uint32_t h;
	/**
	* Number of vertices in lenght on the model
	*/
	uint32_t patch_h;
	/**
	* Width to lenght ratio of the model
	*/
	float ratio;
	/**
	* Model height scale
	*/
	uint16_t zScale;
};

