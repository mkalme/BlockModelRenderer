#pragma once
#include "../Model.Render/Render/Paint/Image/ReadonlyImage.h"

class SolidReadonlyImage : public ReadonlyImage
{
public:
	Rgba Color;

	SolidReadonlyImage(Rgba color) : Color(color) {}

	Rgba ReadPixel(Vec2f pixel) override {
		return Color;
	}
};

