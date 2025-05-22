#pragma once
#include "../../../Geometry/Vector/Vec3f.h"
#include "../../../Geometry/Ray.h"
#include "../../../Geometry/Rotation/Axis.h"
#include "../../../Geometry/Rotation/AxisRotation.h"

class BoundingBox
{
public:
	BoundingBox();
	BoundingBox(Vec3f from, Vec3f to);

	bool Intersect(const Ray& ray) const;
	void Rotate(const AxisRotation& axisRotation);

	void SetMin(Vec3f min);
	Vec3f GetMin() const;

	void SetMax(Vec3f max);
	Vec3f GetMax() const;
private:
	Vec3f m_min, m_max;
};