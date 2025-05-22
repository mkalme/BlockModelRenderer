#pragma once
#include <map>
#include <string>
#include <vector>
#include "../../Utilities/Directory.h"
#include "Tokenized/BlockstateToken.h"
#include "Tokenized/CaseConditionToken.h"

class BlockstateReader
{
public:
	std::map<std::string, BlockstateToken*>* Read(std::string directory) const {
		std::vector<std::string> files = Directory::GetAllFiles(directory);

		std::map<std::string, BlockstateToken*>* output = new std::map<std::string, BlockstateToken*>();
		for (int i = 0; i < files.size(); i++) {
			BlockstateToken* model = ParseModelFromJson(directory, files[i]);
			if (model != NULL) (*output)[model->Name] = model;
		}

		return output;
	}
private:
	BlockstateToken* ParseModelFromJson(std::string directory, std::string file) const {
		nlohmann::json data;

		try {
			data = String::ParseFileToJson(file);
		}
		catch (...) {
			return NULL;
		}

		BlockstateToken* output = new BlockstateToken();
		output->Name = String::RemoveFileExtension(file);

		//if (output->Name != "chiseled_bookshelf") return NULL;

		if (data.contains("variants")) {
			output->Type = BlockstateTypeToken::Variant;
			ReadVariantFromJson(data["variants"], *output->Variant);
		}

		if (data.contains("multipart")) {
			output->Type = BlockstateTypeToken::Multipart;
			ReadMultipartFromJson(data["multipart"], *output->Multipart);
		}

		return output;
	}

	void ReadVariantFromJson(nlohmann::json data, VariantToken& output) const {
		for (auto it = data.begin(); it != data.end(); ++it) {
			output.Variants[it.key()] = ReadMultiModelToken(*it);
		}
	}

	void ReadMultipartFromJson(nlohmann::json data, MultipartToken& output) const {
		for (int i = 0; i < data.size(); i++) {
			nlohmann::json caseObj = data[i];

			CaseToken caseToken = CaseToken();
			caseToken.Apply = ReadMultiModelToken(caseObj["apply"]);

			if (caseObj.contains("when")) {
				nlohmann::json whenData = caseObj["when"];
				if (whenData.contains("AND")) {
					caseToken.Type = ConditionType::AND;
					ReadWhenCaseTokens(whenData["AND"], caseToken);
				}
				else if (whenData.contains("OR")) {
					caseToken.Type = ConditionType::OR;
					ReadWhenCaseTokens(whenData["OR"], caseToken);
				}
				else {
					caseToken.Type = ConditionType::AND;

					CaseConditionBlockToken block = CaseConditionBlockToken();
					CaseConditionToken state = CaseConditionToken();

					for (auto it = whenData.begin(); it != whenData.end(); ++it) {
						state.PropertyName = it.key();
						std::string value = it.value();
						String::Split(value, '|', state.PropertyValues);

						break;
					}

					block.Conditions.push_back(state);
					caseToken.Blocks.push_back(block);
				}
			}

			output.Cases.push_back(caseToken);
		}
	}

	void ReadWhenCaseTokens(nlohmann::json data, CaseToken& output) const {
		for (int i = 0; i < data.size(); i++) {
			CaseConditionBlockToken block = ReadCaseConditionBlockToken(data[i]);
			output.Blocks.push_back(block);
		}
	}

	CaseConditionBlockToken ReadCaseConditionBlockToken(nlohmann::json data) const {
		CaseConditionBlockToken output = CaseConditionBlockToken();

		for (auto it = data.begin(); it != data.end(); ++it) {
			CaseConditionToken state = CaseConditionToken();

			state.PropertyName = it.key();
			std::string propertyValue = it.value();
			String::Split(propertyValue, '|', state.PropertyValues);

			output.Conditions.push_back(state);
		}

		return output;
	}

	CaseConditionToken ReadCaseConditionState(nlohmann::json data) const {
		CaseConditionToken state = CaseConditionToken();
	}

	MultiModelToken ReadMultiModelToken(nlohmann::json data) const {
		MultiModelToken output = MultiModelToken();

		if (data.type() == nlohmann::detail::value_t::array) {
			for (int i = 0; i < data.size(); i++) {
				output.Models.push_back(ReadBlockstateModelToken(data[i]));
			}
		}
		else {
			output.Models.push_back(ReadBlockstateModelToken(data));
		}

		return output;
	}

	BlockstateModelToken ReadBlockstateModelToken(nlohmann::json data) const {
		BlockstateModelToken output = BlockstateModelToken();
		output.Name = data["model"];
		if (data.contains("y")) output.Y = data["y"];
		if (data.contains("x")) output.X = data["x"];
		if (data.contains("uvlock")) output.UvLock = data["uvlock"];
		if (data.contains("weight")) output.Weight = data["weight"];

		return output;
	}
};