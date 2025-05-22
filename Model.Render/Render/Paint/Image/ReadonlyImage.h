#pragma once
#include "../Rgba.h"
#include "../../../Geometry/Vector/Vec2f.h"

class ReadonlyImage
{
public:
	virtual Rgba ReadPixel(Vec2f pixel) = 0;

	virtual ~ReadonlyImage() {}
};

