#include "SampleTemplate.h"
#include <limits>
#include <iostream>

SampleTemplate::SampleTemplate() {
	m_hitPixels = 0;
	m_rayBuffer = 0;
}

void SampleTemplate::Initialize(const SampleParameters& sampleParameters) {
	m_parameters = sampleParameters;
	if (m_rayBuffer != NULL) delete m_rayBuffer;
	if (m_hitPixels != NULL) delete m_hitPixels;

	m_rayBuffer = new Ray[sampleParameters.Width * sampleParameters.Height];

	int length = (int)std::ceilf(sampleParameters.Width * sampleParameters.Height / 8.0);
	m_hitPixels = new uint8_t[length];
	for (int i = 0; i < length; i++) {
		m_hitPixels[i] = 0;
	}

	m_areaFrom = std::numeric_limits<float>::max();
	m_areaTo = std::numeric_limits<float>::min();

	ShootInitialRays();
}

void SampleTemplate::ShootInitialRays() {
	m_parameters.SampleEnvironment.EnvironmentCamera->Cast(m_rayBuffer, m_parameters.Width, m_parameters.Height);

	for (int y = 0; y < m_parameters.Height; y++) {
		for (int x = 0; x < m_parameters.Width; x++) {
			int index = (y * m_parameters.Width + x);
			
			bool hit = m_parameters.CuboidVolume.Intersect(m_rayBuffer[index]);
			if (hit) {
				m_hitPixels[index / 8] |= (1 << (index % 8));

				m_areaFrom = m_areaFrom.MinVector(m_areaFrom, Vec2f(x, y));
				m_areaTo = m_areaTo.MaxVector(m_areaTo, Vec2f(x, y));
			}
			else {
				m_hitPixels[index / 8] &= ~(1 << (index % 8));
			}
		}
	}
}

SampleResult SampleTemplate::Sample(const Model* const model) const {
	int x0 = (int)m_areaFrom.X, y0 = (int)m_areaFrom.Y;
	int x1 = (int)m_areaTo.X, y1 = (int)m_areaTo.Y;

	SampleResult output = SampleResult();

	Vec3f averageRgb = Vec3f();
	float averageA = 0, averageTint = 0;

	for (int y = y0; y <= y1; y++) {
		for (int x = x0; x <= x1; x++) {
			int index = (y * m_parameters.Width + x);
			if ((m_hitPixels[index / 8] & (1 << (index % 8))) == 0) continue;
			output.Volume.TotalCuboidArea++;

			HitResult hitResult = m_rayCaster.CastRay(m_rayBuffer[index], m_parameters.SampleEnvironment, model);
			if (!hitResult.Hit) continue;
			
			Rgba pixel = hitResult.Color;
			if (pixel.A < 0) output.Volume.InvisibleFaceRays++;
			else if (pixel.A == 0) output.Volume.VisibleFaceTransparentRays++;
			else {
				output.Volume.VisibleFaceRays++;

				averageRgb += pixel.Rgb * pixel.A;
				averageA += pixel.A;
				averageTint += pixel.Tint * pixel.A;
			}
		}
	}

	if (output.Volume.VisibleFaceRays > 0) {
		output.Color.AverageColorOfVisibleFace = Rgba(averageRgb / averageA, averageA / output.Volume.VisibleFaceRays, averageTint / averageA);
	}

	return output;
}