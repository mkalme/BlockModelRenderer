#pragma once
#include <unordered_map>
#include <string>
#include <algorithm>
#include "UriKind.h"
#include "../../Utilities/Directory.h"
#include "LockedBitmap.h"

class ImageProvider
{
public:
	std::string FullPath;
	std::unordered_map<std::string, LockedBitmap*> Cache = std::unordered_map<std::string, LockedBitmap*>();

	ImageProvider(std::string fullPath) {
		FullPath = fullPath;
	}

    LockedBitmap* Provide(const std::string& path, UriKind uriKind) {
        std::string imageKey = path;
		if (uriKind == UriKind::Relative) imageKey = Directory::GetAbsolutePath(imageKey);
		else imageKey = FullPath + "\\" + imageKey;

		String::Replace(imageKey, '/', '\\');
		imageKey += ".png";

		LockedBitmap* output;
        if (Cache.find(imageKey) == Cache.end()) {
            if (!Directory::FileExists(imageKey)) return NULL;

            output = new LockedBitmap(imageKey.c_str());
            Cache[imageKey] = output;
		}
		else {
			output = Cache[imageKey];
		}

        return output;
    }
};