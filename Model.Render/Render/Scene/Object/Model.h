#pragma once
#include <vector>
#include "Cube.h"
#include "BoundingBox.h"

class Model
{
public:
	std::vector<Cube> Elements;
	Rotation XRotation;
	Rotation YRotation;
	Vec3f Origin;
	bool AllowsSemiTransparency = false;

	BoundingBox Box;

	Model();

	void UpdateBoundingBox();
};