#pragma once
#include "BlockstateTypeToken.h"
#include "VariantToken.h"
#include "MultipartToken.h"
#include <string>

class BlockstateToken
{
public:
	std::string Name;

	BlockstateTypeToken Type;
	VariantToken* Variant = new VariantToken();
	MultipartToken* Multipart = new MultipartToken();
};