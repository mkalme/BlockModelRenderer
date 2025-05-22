#pragma once
#include "../../../Geometry/Vector/Vec3f.h"
#include "../../../Geometry/Rotation/AxisRotation.h"
#include "../../../Geometry/Ray.h"
#include "../CastResult.h"
#include "Face.h"
#include "BoundingBox.h"

class Cube
{
public:
	Vec3f Min;
	Vec3f Max;
	
	AxisRotation Rotation;
	bool Shade;

	Face Up;
	Face Down;
	Face North;
	Face South;
	Face West;
	Face East;

	Cube(Vec3f from, Vec3f to);

	CastResult Intersect(const Ray& ray) const;
private:
	const Vec3f X_NORMAL = Vec3f(1, 0, 0), Y_NORMAL = Vec3f(0, 1, 0), Z_NORMAL = Vec3f(0, 0, 1);
	const CastResult EMPTY_CAST = CastResult();

	CastResult IntersectRotated(const Ray& ray) const;

	CastResult IntersectXAxis(const Ray& ray, float t, const Face& face) const;
	CastResult IntersectYAxis(const Ray& ray, float t, const Face& face) const;
	CastResult IntersectZAxis(const Ray& ray, float t, const Face& face) const;
};

