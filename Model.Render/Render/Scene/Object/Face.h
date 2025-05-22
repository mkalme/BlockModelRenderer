#pragma once
#include "../../../Geometry/Vector/Vec2f.h"
#include "../../Paint/Image/ReadOnlyImage.h"

class Face
{
public:
	Vec2f From, To;
	bool Visible = true;
	ReadonlyImage* Surface = 0;

	Vec2f Size;

	Face();
	Face(Vec2f from, Vec2f to);
};