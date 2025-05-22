#include "ModelRendererRayCaster.h"
#include "../../Utilities/Utilities.h"
#include "../Scene/HitResult.h"

HitResult ModelRendererRayCaster::CastRay(const Ray& ray, const Environment& environment, const Model* const model) const {
	if (model == NULL || model->Elements.empty()) return HitResult(false, environment.SkyColor);

	Ray rotatedRay = ray;
	model->YRotation.RotateWithAxis(rotatedRay, Axis::Y, model->Origin);
	model->XRotation.RotateWithAxis(rotatedRay, Axis::X, model->Origin);
	if (!model->Box.Intersect(rotatedRay)) return HitResult(false, environment.SkyColor);

	if (model->Elements.size() == 1) {
		CastResult result = model->Elements[0].Intersect(rotatedRay);
		if (!result.Intersected()) return HitResult(false, environment.SkyColor);

		Shade(result, environment, result.Color.Rgb);
		
		if (Utilities::InBounds(result.Color.A, 0, 1)) result.Color.CombineTowardsBehind(environment.SkyColor);
		return HitResult(result.Color);
	}

	if (model->AllowsSemiTransparency) {
		CastResult* memory = (CastResult*) _alloca(sizeof(CastResult) * model->Elements.size());
		CastResult** stack = (CastResult**) _alloca(sizeof(CastResult*) * model->Elements.size());
		int stackSize = 0;

		for (int i = 0; i < model->Elements.size(); i++) {
			CastResult result = model->Elements[i].Intersect(rotatedRay);
			if (!result.Intersected()) continue;

			memory[stackSize] = result;
			BinaryInsert(&memory[stackSize], stack, stackSize);
			stackSize++;
		}
		if (stackSize == 0) return HitResult(false, environment.SkyColor);

		Rgba output = stack[0]->Color;
		Shade(*stack[0], environment, output.Rgb);

		for (int i = 1; i < stackSize; i++) {
			if (output.A == 1) break;

			Rgba current = stack[i]->Color;
			if (current.A == 0) continue;

			if (current.A < 0 && output.A == 0) {
				output = current;
				continue;
			}

			if (current.A > 0 && output.A < 0) output = Rgba(0, 0);

			// Normal needs to be rotated back
			Shade(*stack[i], environment, current.Rgb);

			current.CombineTowardsTop(output);
			output = current;
		}

		if (Utilities::InBounds(output.A, 0, 1)) output.CombineTowardsBehind(environment.SkyColor);
		return HitResult(output);
	}
	else {
		CastResult output = model->Elements[0].Intersect(rotatedRay);
		bool intersected = output.Intersected();

		for (int i = 1; i < model->Elements.size(); i++) {
			CastResult result = model->Elements[i].Intersect(rotatedRay);
			intersected |= result.Intersected();
			
			if (!result.Intersected() || result.Color.A == 0) continue;
			if (result.Color.A < 0 && output.Color.A > 0) continue;

			if (!output.Intersected() || result.T <= output.T || output.Color.A <= 0) output = result;
		}

		if (output.Color.A == 0) return HitResult(intersected, environment.SkyColor);

		// Normal needs to be rotated back
		Shade(output, environment, output.Color.Rgb);
		return HitResult(intersected, output.Color);
	}
}

inline void ModelRendererRayCaster::Shade(const CastResult& castResult, const Environment& environment, Vec3f& color) const noexcept {
	if (!environment.EnvironmentShading.ShadingEnabled || !castResult.Shade || castResult.Color.A <= 0) return;
	
	Vec3f directionalLight = environment.EnvironmentShading.DirectionalLight * std::fabs(environment.EnvironmentShading.DirectionalLightDirection.Dot(castResult.Normal));
	color *= (directionalLight + environment.EnvironmentShading.AmbientLight).Clamp(0, 1);
}

void ModelRendererRayCaster::BinaryInsert(CastResult* const value, CastResult** stack, int stackSize) const {
	int left = 0;
	int right = stackSize - 1;
	int mid = 0;

	while (left <= right) {
		mid = left + (right - left) / 2;
		if (stack[mid]->T < value->T) {
			left = mid + 1;
		}
		else {
			right = mid - 1;
		}
	}

	int insertPos = left;

	for (int i = stackSize; i > insertPos; --i) {
		stack[i] = stack[i - 1];
	}

	stack[insertPos] = value;
}