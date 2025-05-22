#pragma once
#include <string>
#include "Uv.h"

class FaceToken
{
public:
	std::string* Texture;
	Uv* Uv;
	float Rotation = 0;
	int TintIndex = -1;
};