#pragma once
#include "Shared.h"
#include <vector>

enum ImageType { RGB, RGBA };

/**
* Class for loading image from file and turning it to grey scale
*/
class Image
{
public:
	/** 
	* Constructor, initialises the image with data from the file 
	* @param path path to image
	* @param type wheteher or not the pixels should include alpha
	*/
	Image(std::string path, ImageType type);

	/** 
	* Destructor, frees memory taken by image array 
	*/
	~Image();

	/** 
	* Converts image to grey scale 
	* @returns vector containing image in grey scale
	*/
	std::vector<unsigned char> toGreyScale();

	/** 
	* Returns array of the image 
	* @returns array containing the image
	*/
	unsigned char * getArray();

	/**
	* Image width
	*/
	int w;
	
	/**
	* Image height
	*/
	int h;
	
	/**
	* Image channels
	*/
	int channels;

protected:	

	/**
	* Array of image pixels
	*/
	unsigned char * pixels;

	/**
	* Vector of image in grey scale
	*/
	std::vector<unsigned char> monochrome;

};

