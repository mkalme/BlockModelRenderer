#pragma once
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include "../External/json.hpp"

class String
{
public:
    static std::string RemoveFileExtension(const std::string& filename) {        
        std::string withoutSlashes = std::filesystem::path(filename).filename().string();
        size_t lastDotPos = withoutSlashes.find_last_of('.');

        if (lastDotPos != std::string::npos && lastDotPos != 0) {
            return withoutSlashes.substr(0, lastDotPos);
        }

        return withoutSlashes;
    }

    static std::string CutToLast(const std::string& value, char separator) {
        size_t lastDotPos = value.find_last_of(separator) + 1;

        if (lastDotPos != std::string::npos && lastDotPos != 0) {
            return value.substr(lastDotPos);
        }

        return value;
    }

    static nlohmann::json ParseFileToJson(std::string file) {
        std::ifstream i(file);
        return nlohmann::json::parse(i);
    }

    static bool StartsWith(const std::string& fullString, const std::string& starting) {
        if (fullString.length() >= starting.length()) {
            return (0 == fullString.compare(0, starting.length(), starting));
        }
        else {
            return false;
        }
    }

    static void Replace(std::string& value, char oldValue, char newValue) {
        std::replace(value.begin(), value.end(), oldValue, newValue);
    }

    static void Split(const std::string& value, char separator, std::vector<std::string>& output) {
        std::string token;

        for (char ch : value) {
            if (ch == separator) {
                if (!token.empty()) {
                    output.push_back(token);
                    token.clear();
                }
            }
            else {
                token += ch;
            }
        }

        if (!token.empty()) {
            output.push_back(token);
        }
    }
};

