#pragma once
#include "Geometry/Vector/Vec3f.h"
#include "Geometry/Rotation/AxisRotation.h"

class RotationToken
{
public:
	Vec3f Origin;
	AxisRotation Rotation;
	bool Rescale;
	float Angle;
};

