#pragma once
#include "Geometry/Vector/Vec2f.h"

class Uv
{
public:
	Vec2f From, To;

	Uv() {
		From = Vec2f();
		To = Vec2f();
	}

	Uv(Vec2f from, Vec2f to) {
		From = from;
		To = to;
	}
};

