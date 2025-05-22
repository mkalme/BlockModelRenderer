#pragma once
#include <vector>
#include <string>

class CaseConditionToken
{
public:
	std::string PropertyName;
	std::vector<std::string> PropertyValues = std::vector<std::string>();
};

