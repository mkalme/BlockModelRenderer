#pragma once
#include <unordered_set>
#include <functional>
#include "Reader/Tokenized/BlockstateToken.h"

struct Property {
	std::string Name;
	std::string Value;

	Property(const std::string& s1, const std::string& s2) : Name(s1), Value(s2) {}

	bool operator==(const Property& other) const {
		return Name == other.Name && Value == other.Value;
	}
};

namespace std {
	template <>
	struct hash<Property> {
		std::size_t operator()(const Property& ms) const {
			std::size_t h1 = std::hash<std::string>{}(ms.Name);
			std::size_t h2 = std::hash<std::string>{}(ms.Value);

			return h1 ^ (h2 << 1);
		}
	};
}

class Evaluator
{
public:
	void Evaluate(const BlockstateToken& blockState, const std::unordered_set<Property>& blockProperties, std::vector<int>& output) const {
		if (blockState.Type == BlockstateTypeToken::Variant) {
			EvaluateVariant(*blockState.Variant, blockProperties, output);
		}
		else {
			EvaluateMultipart(*blockState.Multipart, blockProperties, output);
		}
	}

	void EvaluateVariant(const VariantToken& variant, const std::unordered_set<Property>& blockProperties, std::vector<int>& output) const {

	}

	void EvaluateMultipart(const MultipartToken& multipart, const std::unordered_set<Property>& blockProperties, std::vector<int>& output) const {
		for (int i = 0; i < multipart.Cases.size(); i++) {
			if (EvaluateCase(multipart.Cases[i], blockProperties)) {
				output.push_back(i);
			}
		}
	}

	bool EvaluateCase(const CaseToken& caseToken, const std::unordered_set<Property>& blockProperties) const {
		if (caseToken.Blocks.size() == 0) {
			return true;
		}
		
		bool applies = false;
		for (int i = 0; i < caseToken.Blocks.size(); i++) {
			bool evaluated = EvaluateBlock(caseToken.Blocks[i], blockProperties);

			if (caseToken.Type == ConditionType::OR && evaluated) {
				applies = true;
				break;
			}
			else if (caseToken.Type == ConditionType::AND) {
				applies = evaluated;
				if (!applies) break;
			}
		}

		return applies;
	}

	bool EvaluateBlock(const CaseConditionBlockToken& block, const std::unordered_set<Property>& blockProperties) const {
		for (int i = 0; i < block.Conditions.size(); i++) {
			const CaseConditionToken* condition = &block.Conditions[i];

			for (int j = 0; j < condition->PropertyValues.size(); j++) {
				Property property = Property(condition->PropertyName, condition->PropertyValues[j]);
				if (!blockProperties.contains(property)) return false;
			}
		}
		
		return true;
	}
};

