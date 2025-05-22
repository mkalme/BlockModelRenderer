#pragma once
#include <unordered_map>
#include <string>
#include "Animation.h"
#include "../../Utilities/Directory.h"

class AnimationProvider
{
public:
	std::string FullPath;
	std::unordered_map<std::string, Animation*> Cache = std::unordered_map<std::string, Animation*>();

	AnimationProvider(std::string fullPath) {
		FullPath = fullPath;
	}

	Animation* Provide(const std::string& path, UriKind uriKind, const LockedBitmap* const parentImage) {
		std::string key = path;
		if (uriKind == UriKind::Relative) key = Directory::GetAbsolutePath(key);
		else key = FullPath + "\\" + key;

		String::Replace(key, '/', '\\');
		key += ".png.mcmeta";

		Animation* output;
		if (Cache.find(key) == Cache.end()) {
			if (!Directory::FileExists(key)) {
				output = new Animation();
			}
			else {
				output = ReadAnimation(key, parentImage);
			}
			
			Cache[key] = output;
		}
		else {
			output = Cache[key];
		}

		return output;
	}

	Animation* ReadAnimation(std::string file, const LockedBitmap* const parentImage) const {
		nlohmann::json data;

		try {
			data = String::ParseFileToJson(file);
		}
		catch (...) {
			return new Animation();
		}

		Animation* output = new Animation(parentImage->GetHeight() / parentImage->GetWidth());
		if (!data.contains("animation")) return output;

		nlohmann::json obj = data["animation"];
		if (obj.contains("interpolate")) output->Interpolate = obj["interpolate"];
		if (obj.contains("frametime")) output->FrameDuration = obj["frametime"];

		if (obj.contains("frames")) {
			nlohmann::json frames = obj["frames"];

			output->Frames = new Frame[frames.size()];
			output->FrameCount = frames.size();

			for (int i = 0; i < frames.size(); i++) {
				nlohmann::json frameObj = frames[i];

				if (frameObj.type() != nlohmann::detail::value_t::object) {
					output->Frames[i].Position = frameObj;
					output->Frames[i].Duration = output->FrameDuration;
				}
				else {
					output->CustomFrameDuration = true;
					output->Frames[i].Position = frameObj["index"];
					output->Frames[i].Duration = frameObj["time"];
				}
			}
		}
		else {
			output->Frames = new Frame[output->TileCount];
			output->FrameCount = output->TileCount;

			for (int i = 0; i < output->TileCount; i++) {
				output->Frames[i].Position = i;
				output->Frames[i].Duration = output->FrameDuration;
			}
		}

		return output;
	}
};

