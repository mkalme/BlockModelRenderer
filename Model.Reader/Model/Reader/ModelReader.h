#pragma once
#include <string>
#include <map>
#include <sstream>
#include <fstream>
#include "Tokenized/ModelToken.h"
#include "../../Utilities/Directory.h"
#include "../../External/json.hpp"

class ModelReader
{
public:
	std::map<std::string, ModelToken*>* Read(std::string directory) const {
		std::vector<std::string> files = Directory::GetAllFiles(directory + "\\block");

		std::map<std::string, ModelToken*>* output = new std::map<std::string, ModelToken*>();
		for (int i = 0; i < files.size(); i++) {
			ParseModelFromJson(directory, files[i], output);
		}

		return output;
	}
private:
	ModelToken* ParseModelFromJson(std::string directory, std::string file, std::map<std::string, ModelToken*>* output) const {
		nlohmann::json data;
		
		try {
			data = String::ParseFileToJson(file);
		}
		catch (...) {
			return NULL;
		}

		ModelToken* model = new ModelToken();
		model->Name = String::RemoveFileExtension(file);

		nlohmann::json texturesObj = data["textures"];
		for (auto it = texturesObj.begin(); it != texturesObj.end(); ++it) {
			nlohmann::json texture = *it;
			model->Textures[it.key()] = (std::string) texture;
		}

		nlohmann::json elementsArray = data["elements"];
		for (auto it = elementsArray.begin(); it != elementsArray.end(); ++it) {
			nlohmann::json element = *it;
			model->Elements.push_back(ParseElement(element));
		}

		if (data.contains("parent")) {
			std::string parentName = data["parent"];

			const std::string SKIP = "minecraft:";
			if (String::StartsWith(parentName, SKIP)) parentName = parentName.substr(SKIP.size());
			String::Replace(parentName, '/', '\\');

			ModelToken* parent = ParseModelFromJson(directory, directory + "\\" + parentName + ".json", output);
			ApplyParentProperties(model, parent);
		}

		(*output)[model->Name] = model;
		return model;
	}

	void ApplyParentProperties(ModelToken* child, ModelToken* parent) const {
		if (parent == NULL) return;

		for (const auto& pair : parent->Textures) {
			if (child->Textures.find(pair.first) == child->Textures.end()) {
				child->Textures.insert(pair);
			}
		}

		if (child->Elements.size() == 0) {
			for (int i = 0; i < parent->Elements.size(); i++) {
				child->Elements.push_back(parent->Elements[i]);
			}
		}
	}

	ElementToken ParseElement(nlohmann::json data) const {
		ElementToken output = ElementToken();
		output.From = ParseVec3f(data["from"]);
		output.To = ParseVec3f(data["to"]);
		
		if (data.contains("rotation")) output.Rotation = ParseRotation(data["rotation"]);
		if (data.contains("shade")) output.Shade = data["shade"];

		nlohmann::json faces = data["faces"];
		if (faces.contains("down")) output.Down = ParseFace(faces["down"]);
		if (faces.contains("up")) output.Up = ParseFace(faces["up"]);
		if (faces.contains("north")) output.North = ParseFace(faces["north"]);
		if (faces.contains("south")) output.South = ParseFace(faces["south"]);
		if (faces.contains("west")) output.West = ParseFace(faces["west"]);
		if (faces.contains("east")) output.East = ParseFace(faces["east"]);

		return output;
	}

	FaceToken* ParseFace(nlohmann::json data) const {
		FaceToken* output = new FaceToken();
		if(data.contains("texture")) output->Texture = new std::string(data["texture"]);
		if(data.contains("uv")) output->Uv = ParseUv(data["uv"]);
		if(data.contains("rotation")) output->Rotation = data["rotation"];
		if(data.contains("tintindex")) output->TintIndex = data["tintindex"];

		return output;
	}

	Uv* ParseUv(nlohmann::json data) const {
		Vec2f from = Vec2f(data[0], data[1]);
		Vec2f to = Vec2f(data[2], data[3]);

		Uv* uv = new Uv();
		uv->From = from;
		uv->To = to;

		return uv;
	}

	Vec3f ParseVec3f(nlohmann::json data) const {
		return Vec3f(data[0], data[1], data[2]);
	}

	RotationToken* ParseRotation(nlohmann::json data) const {
		RotationToken* output = new RotationToken();
		if(data.contains("origin")) output->Origin = ParseVec3f(data["origin"]);
		if(data.contains("rescale")) output->Rescale = data["rescale"];
		if(data.contains("angle")) output->Angle = data["angle"];

		Axis axis;
		std::string axisValue = data["axis"];

		if (axisValue == "x") axis = Axis::X;
		else if (axisValue == "y") axis = Axis::Y;
		else axis = Axis::Z;

		output->Rotation = AxisRotation(axis, Rotation::FromDegrees(output->Angle), output->Origin);

		return output;
	}
};