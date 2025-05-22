#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "ElementToken.h"

class ModelToken
{
public:
	std::string Name;
	std::unordered_map<std::string, std::string> Textures;
	std::vector<ElementToken> Elements;
};

