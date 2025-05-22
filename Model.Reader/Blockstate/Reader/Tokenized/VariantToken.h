#pragma once
#include <unordered_map>
#include <string>
#include "MultiModelToken.h"

class VariantToken
{
public:
	std::unordered_map<std::string, MultiModelToken> Variants = std::unordered_map<std::string, MultiModelToken>();
};