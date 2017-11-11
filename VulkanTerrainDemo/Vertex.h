#pragma once
#include "Shared.h"
#include <array>

/**
* Struct for containg model vertices
*/
struct Vertex
{
public:
	/**
	* Position of the vertex
	*/
	glm::vec3 pos;
	/**
	* Colour at given vertex
	*/
	glm::vec3 color;
	/**
	* Position of the vertex on texture
	*/
	glm::vec2 texPos;
	/**
	* Vertex's normal
	*/
	glm::vec3 normal;
	
	/**
	* Binding description of the imput
	* @returns binding description of the imput
	*/
	static VkVertexInputBindingDescription getInputBindingDescription();

	/** 
	* Returns input attributes
	* @returns input attributes
	*/
	static std::array<VkVertexInputAttributeDescription, 4> getInputAttributeDescriptions();
};

