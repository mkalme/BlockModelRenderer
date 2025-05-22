#pragma once
#include <cstdint>
#include "Geometry/Vector/Vec3f.h"

struct Vec3fInterop
{
public:
	float X;
	float Y;
	float Z;

	Vec3f ToVec3f() const {
		return Vec3f(X, Y, Z);
	}
};

enum class CameraTypeInterop : uint32_t {
	Perspective,
	Projection
};

struct CameraInterop {
	Vec3fInterop Location;
	Vec3fInterop Direction;
	float Rotation;
	float PlaneWidth;
	float FocalLenght;
	CameraTypeInterop CameraType;
};

struct ShadingInterop {
	uint8_t ShadingEnabled;
	Vec3fInterop AmbientLight;
	Vec3fInterop DirectionalLightDirection;
	Vec3fInterop DirectionalLight;
};

struct RgbaInterop {
	Vec3fInterop Rgb;
	float A;
};

struct EnvironmentInterop {
	RgbaInterop SkyColor;
	ShadingInterop Shading;
	CameraInterop Camera;
};

struct ModelRenderRequestArgs {
	const char* Name;
	EnvironmentInterop Environment;
	int Width;
	int Height;
};

struct ModelInfo {
	Vec3fInterop From;
	Vec3fInterop To;
};

extern "C" __declspec(dllexport) int initialize();
extern "C" __declspec(dllexport) void loadDirectory(const char* directory);
extern "C" __declspec(dllexport) void provideModelNames(const char**& arr, int& size);
extern "C" __declspec(dllexport) ModelInfo requestModelInfo(const char* name);
extern "C" __declspec(dllexport) uint8_t* requestModelRender(ModelRenderRequestArgs args);
extern "C" __declspec(dllexport) void dispose();