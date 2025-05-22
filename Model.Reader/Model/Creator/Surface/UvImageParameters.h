#pragma once
#include "../../Reader/Tokenized/Uv.h"

struct UvImageParameters
{
	Uv TextureUv = Uv();
	int TextureRotation = 0;
	int UvRotation = 0;
	bool AllowSemiTransparency = false;
	int TintIndex = -1;
};