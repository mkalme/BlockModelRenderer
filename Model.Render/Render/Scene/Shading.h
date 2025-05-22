#pragma once
#include <cstdint>
#include "../../Geometry/Vector/Vec3f.h"
class Shading
{
public:
	uint8_t ShadingEnabled;
	Vec3f AmbientLight;
	Vec3f DirectionalLightDirection;
	Vec3f DirectionalLight;

	Shading();
};