#pragma once
#include "../SampleResult.h"
#include <Render/Scene/Object/Model.h>
#include "SampleParameters.h"
#include <Render/Render/ModelRendererRayCaster.h>

class SampleTemplate
{
public:
	SampleTemplate();

	void Initialize(const SampleParameters& sampleParameters);
	SampleResult Sample(const Model* const model) const;
private:
	SampleParameters m_parameters;
	
	Vec2f m_areaFrom;
	Vec2f m_areaTo;
	Ray* m_rayBuffer;
	uint8_t* m_hitPixels;

	ModelRendererRayCaster m_rayCaster = ModelRendererRayCaster();

	void ShootInitialRays();
};

