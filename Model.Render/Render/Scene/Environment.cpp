#pragma once
#include "Environment.h"

Environment::Environment() {
	EnvironmentCamera = 0;
	EnvironmentShading = Shading();
	SkyColor = Rgba();
}

Environment::Environment(Camera* environmentCamera, Shading environmentShading, Rgba skyColor) {
	EnvironmentCamera = environmentCamera;
	EnvironmentShading = environmentShading;
	SkyColor = skyColor;
}