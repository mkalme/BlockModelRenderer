#pragma once
#include "../../Geometry/Vector/Vec3f.h"
#include "../Paint/Rgba.h"

struct CastResult
{
	float T;
	Vec3f Normal;
	Rgba Color;
	bool Shade;

	CastResult() {
		T = -1;
		Normal = Vec3f();
		Color = Rgba();
		Shade = true;
	}

	CastResult(float t, Vec3f normal, Rgba color) {
		T = t;
		Normal = normal;
		Color = color;
		Shade = true;
	}

	bool Intersected() const {
		return T > 0;
	}
};