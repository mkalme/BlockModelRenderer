#pragma once
#include "../Reader/Tokenized/BlockstateToken.h"
#include "ExtractedBlockstateModel.h"
#include <iostream>

struct ProperyAllValues {
	std::string Name;
	std::vector<std::string> AllValues = std::vector<std::string>();

	ProperyAllValues(std::string name) {
		Name = name;
	}
};

class ModelExtractor
{
public:
	void Extract(const MultipartToken& multipart, std::vector<ExtractedBlockstateModel>& output) const {
		auto properties = std::unordered_map<std::string, std::unordered_set<std::string>*>();
		GetPropertiesFromBlockstate(multipart, properties);

		if (properties.size() == 0) return;

		auto propertySet = std::vector<ProperyAllValues>();
		int combinationCount = 1;

		for (const auto& pair : properties) {
			ProperyAllValues property = ProperyAllValues(pair.first);
			for (const auto& value : *pair.second) {
				property.AllValues.push_back(value);
			}

			property.AllValues.push_back("-");

			propertySet.push_back(property);
			combinationCount *= property.AllValues.size();
		}

		int* combination = (int*) _alloca(sizeof(int) * combinationCount * propertySet.size());
		CreateAllCombinations(propertySet, combination);

		for (int i = 0; i < combinationCount; i++) {
			int* thisCombination = combination + propertySet.size() * i;

			for (int j = 0; j < propertySet.size(); j++) {
				//std::cout << propertySet[j].Name << " = " << propertySet[j].AllValues[thisCombination[j]];

				//if (j < propertySet.size() - 1) std::cout << ", ";
			}

			//std::cout << "\n";
		}

		for (int i = 0; i < combinationCount; i++) {
			int* thisCombination = combination + propertySet.size() * i;

			std::unordered_set<Property> set = std::unordered_set<Property>();
			for (int j = 0; j < propertySet.size(); j++) {
				set.insert(Property(propertySet[j].Name, propertySet[j].AllValues[thisCombination[j]]));
			}

			std::vector<int> modelSet = std::vector<int>();
			m_evaluator.EvaluateMultipart(multipart, set, modelSet);

			for (int j = 0; j < modelSet.size(); j++) {
				//std::cout << modelSet[j] << ", ";
			}

			//std::cout << "\n";

			ExtractedBlockstateModel* model = GetOrCreateExtractedBlockstateModel(modelSet, output);
			//model->Blockstates

			PropertyBlockstate blockstate = PropertyBlockstate();
			for (int j = 0; j < propertySet.size(); j++) {
				blockstate.Properties.push_back(Property(propertySet[j].Name, propertySet[j].AllValues[thisCombination[j]]));
			}

			model->Blockstates.push_back(blockstate);
		}

		int bits = (int)ceilf(log(4096) / log(2));
		for (int i = 0; i < propertySet.size(); i++) {
			bits += (int) ceilf(log(propertySet[i].AllValues.size()) / log(2));
		}

		std::cout << "Bits: " << bits << "\n";
		//std::cout << "Combinations: " << combinationCount << "\n";
		//std::cout << "Unique models: " << output.size() << "\n";
		//std::cout << "==================\n";
	}

private:
	Evaluator m_evaluator = Evaluator();

	ExtractedBlockstateModel* GetOrCreateExtractedBlockstateModel(const std::vector<int>& modelSet, std::vector<ExtractedBlockstateModel>& output) const {
		for (int i = 0; i < output.size(); i++) {
			if (VectorsMatch(modelSet, output[i].Models)) return &output[i];
		}

		output.push_back(ExtractedBlockstateModel());
		ExtractedBlockstateModel* model = &output[output.size() - 1];
	
		model->Models = modelSet;

		return model;
	}

	bool VectorsMatch(const std::vector<int>& a, const std::vector<int>& b) const {
		if (a.size() != b.size()) return false;

		for (int i = 0; i < a.size(); i++) {
			if (a[i] != b[i]) return false;
		}

		return true;
	}

	void CreateAllCombinations(const std::vector<ProperyAllValues>& propertySet, int* output) const {
		int k = 0;
		int* stack = (int*) _alloca(sizeof(int) * propertySet.size());

		CreateCombinations(propertySet, stack, output, 0, k);
	}

	void CreateCombinations(const std::vector<ProperyAllValues>& propertySet, int* stack, int* output, int index, int& k) const {
		if (propertySet.size() == 0) return;
		
		if (index == propertySet.size() - 1) {
			for (int i = 0; i < propertySet[index].AllValues.size(); i++) {
				stack[index] = i;
				
				for (int j = 0; j < propertySet.size(); j++) {
					output[propertySet.size() * k + j] = stack[j];
				}
				
				k++;
			}
		}
		else {
			for (int i = 0; i < propertySet[index].AllValues.size(); i++) {
				stack[index] = i;
				CreateCombinations(propertySet, stack, output, index + 1, k);
			}
		}
	}

	void GetPropertiesFromBlockstate(const MultipartToken& multipart, std::unordered_map<std::string, std::unordered_set<std::string>*>& output) const {
		for (int i = 0; i < multipart.Cases.size(); i++) {
			CaseToken caseToken = multipart.Cases[i];
			GetPropertiesFromCase(caseToken, output);
		}
	}

	void GetPropertiesFromCase(const CaseToken& caseToken, std::unordered_map<std::string, std::unordered_set<std::string>*>& output) const {
		for (int i = 0; i < caseToken.Blocks.size(); i++) {
			CaseConditionBlockToken block = caseToken.Blocks[i];
			GetPropertiesFromBlockToken(block, output);
		}
	}

	void GetPropertiesFromBlockToken(const CaseConditionBlockToken& block, std::unordered_map<std::string, std::unordered_set<std::string>*>& output) const {
		for (int i = 0; i < block.Conditions.size(); i++) {
			CaseConditionToken condition = block.Conditions[i];

			if (!output.contains(condition.PropertyName)) {
				output[condition.PropertyName] = new std::unordered_set<std::string>();
			}

			std::unordered_set<std::string>* set = output[condition.PropertyName];

			for (int j = 0; j < condition.PropertyValues.size(); j++) {
				set->insert(condition.PropertyValues[j]);
			}
		}
	}
};