#pragma once
#include "Reader/Tokenized/BlockstateToken.h"
#include <set>
#include <algorithm>
#include <iterator>
#include "Extractor/ModelExtractor.h"

class SizeCalculator
{
public:
	ModelExtractor m_extractor = ModelExtractor();

	int Calculate(const BlockstateToken& blockstate) const {
		if (blockstate.Type == BlockstateTypeToken::Variant) return blockstate.Variant->Variants.size();

		auto output = std::vector<ExtractedBlockstateModel>();
		m_extractor.Extract(*blockstate.Multipart, output);

		int total = 0;

		for (int i = 0; i < output.size(); i++) {
			total += output[i].Models.size();
		}

		return total;
	}
};

