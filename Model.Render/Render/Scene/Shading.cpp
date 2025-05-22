#include "Shading.h"

Shading::Shading() {
	ShadingEnabled = true;
	AmbientLight = Vec3f();
	DirectionalLightDirection = Vec3f(0, 1, 0);
	DirectionalLight = Vec3f();
}