#include "Model.h"
#include <iostream>

Model::Model() {
	Elements = std::vector<Cube>();
	XRotation = Rotation::FromRadians(0);
	YRotation = Rotation::FromRadians(0);
	Origin = 8;
	Box = BoundingBox();
}

void Model::UpdateBoundingBox() {
	Vec3f min = std::numeric_limits<float>::max();
	Vec3f max = std::numeric_limits<float>::min();

	for (int i = 0; i < Elements.size(); i++) {
		BoundingBox box = BoundingBox(Elements[i].Min, Elements[i].Max);
		box.Rotate(Elements[i].Rotation);

		min = Vec3f::MinVector(min, box.GetMin());
		max = Vec3f::MaxVector(max, box.GetMax());
	}

	Box = BoundingBox(min, max);
}