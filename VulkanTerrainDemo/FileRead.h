#pragma once

#include "Shared.h"

#include <fstream>
#include <vector>

/**
* Function used to read gives file and return it as a verctor of chars 
* @param filename file path to read file from
* @returns vector of chars containing the read file
*/
std::vector<char> readFile(const std::string& filename);