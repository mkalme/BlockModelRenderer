#include "ModelRenderer.h"
#include "../Scene/Environment.h"
#include "../Paint/Image/WriteonlyImage.h"
#include "../Scene/Object/Model.h"
#include <execution>

static int m_prevHeight = 0;
static std::vector<int> m_prevRange;

void ModelRenderer::Render(const Environment& environment, const Model* const model, int width, int height, WriteonlyImage* const image) {
	Ray* rays = ProvideRayBuffer(width * height);
	environment.EnvironmentCamera->Cast(rays, width, height);

	if (m_prevHeight != height) {
		m_prevHeight = height;
		m_prevRange = std::vector<int>(m_prevHeight);
		std::iota(m_prevRange.begin(), m_prevRange.end(), 0);
	}

	int tile = 4;

	std::for_each(std::execution::par, m_prevRange.begin(), m_prevRange.end(),
	[&, rays](int& y) {
		for (int x = 0; x < width; x++) {
			Rgba pixel = m_rayCaster.CastRay(rays[y * width + x], environment, model).Color;
			//if (pixel.A < 0) pixel = ((x / tile) % 2 == 0) ^ ((y / tile) % 2 == 0) ? Rgba(0, 0) : Rgba(Vec3f(0, 0.5, 0.5), 1);
			if (pixel.A < 0) pixel.A = 0;

			image->WritePixel(pixel, x, y);
		}
	});
}

Ray* ModelRenderer::ProvideRayBuffer(int minimumSize) {
	if (m_rayBuffer != NULL && minimumSize <= m_rayBufferSize) return m_rayBuffer;

	if (m_rayBuffer != NULL) delete m_rayBuffer;
	m_rayBuffer = new Ray[minimumSize];
	m_rayBufferSize = minimumSize;

	return m_rayBuffer;
}

ModelRenderer::~ModelRenderer() {
	if (m_rayBuffer != NULL) delete m_rayBuffer;
}