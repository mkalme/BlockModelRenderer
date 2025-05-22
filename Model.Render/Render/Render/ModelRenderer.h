#pragma once
#include "../Scene/Environment.h"
#include "../Scene/Object/Model.h"
#include "../Paint/Image/WriteonlyImage.h"
#include "ModelRendererRayCaster.h"

class ModelRenderer
{
public:
	void Render(const Environment& environment, const Model* const model, int width, int height, WriteonlyImage* const image);

	~ModelRenderer();
private:
	Ray* m_rayBuffer = 0;
	int m_rayBufferSize = 0;
	ModelRendererRayCaster m_rayCaster = ModelRendererRayCaster();

	Rgba CastRay(const Ray& ray, const Environment& environment, const Model* const model) const;
	Ray* ProvideRayBuffer(int minimumSize);

	inline Vec3f Shade(Vec3f normal, const Environment& environment) const noexcept;
	void BinaryInsert(CastResult* const value, CastResult** stack, int stackSize) const;
};