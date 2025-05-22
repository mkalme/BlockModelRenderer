#pragma once
#include "MultiModelToken.h"
#include "ConditionType.h"
#include "CaseConditionBlockToken.h"

class CaseToken
{
public:
	MultiModelToken Apply;

	ConditionType Type;
	std::vector<CaseConditionBlockToken> Blocks = std::vector<CaseConditionBlockToken>();
};