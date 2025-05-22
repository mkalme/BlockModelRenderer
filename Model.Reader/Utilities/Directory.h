#pragma once
#include <vector>
#include <string>
#include "String.h"

#include <iostream>
#include <filesystem>

class Directory
{
public:
    static void GetAllFiles(const std::filesystem::path& path, std::vector<std::string>& output) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.is_regular_file()) {
                output.push_back(entry.path().string());
            }
            else if (entry.is_directory()) {
                GetAllFiles(entry.path(), output);
            }
        }
    }

	static std::vector<std::string> GetAllFiles(const std::string& directory) {
        std::vector<std::string> output;
        GetAllFiles(directory, output);
        return output;
	}

    inline static bool FileExists(const std::string& file) {
        struct stat buffer;
        return (stat(file.c_str(), &buffer) == 0);
    }

    inline static std::string GetAbsolutePath(const std::string& relativePath) {
        char absPath[_MAX_PATH];
        if (_fullpath(absPath, relativePath.c_str(), _MAX_PATH) == nullptr) {
            perror("_fullpath");
            return "";
        }
        return std::string(absPath);
    }
};