#pragma once
#include <string>

class BlockstateModelToken
{
public:
	std::string Name;
	float Y = 0;
	float X = 0;
	bool UvLock = false;
	int Weight = 1;
};

