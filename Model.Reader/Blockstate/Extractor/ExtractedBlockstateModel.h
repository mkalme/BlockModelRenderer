#pragma once
#include "../Reader/Tokenized/MultiModelToken.h"
#include "PropertyBlockstate.h"

class ExtractedBlockstateModel
{
public:
	std::vector<int> Models = std::vector<int>();
	std::vector<PropertyBlockstate> Blockstates = std::vector<PropertyBlockstate>();
};

