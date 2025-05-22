#pragma once
#include "Render/Paint/Rgba.h"

struct VolumetricResult {
	int TotalCuboidArea = 0;
	int VisibleFaceRays = 0;
	int VisibleFaceTransparentRays = 0;
	int InvisibleFaceRays = 0;
};

struct ColorResult {
	Rgba AverageColorOfVisibleFace = Rgba();
};

struct SampleResult
{
	VolumetricResult Volume = VolumetricResult();
	ColorResult Color = ColorResult();
};