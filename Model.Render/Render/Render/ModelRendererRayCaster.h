#pragma once
#include "../Paint/Rgba.h"
#include "../../Geometry/Ray.h"
#include "../Scene/Environment.h"
#include "../Scene/Object/Model.h"
#include "../Scene/HitResult.h"

class ModelRendererRayCaster
{
public:
	HitResult CastRay(const Ray& ray, const Environment& environment, const Model* const model) const;
private:
	void BinaryInsert(CastResult* const value, CastResult** stack, int stackSize) const;
	inline void Shade(const CastResult& castResult, const Environment& environment, Vec3f& color) const noexcept;
};