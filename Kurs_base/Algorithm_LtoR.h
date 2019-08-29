#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Alg_Elements.h"


class LtoR_MethodAlg : public ParseAlgorithm {

	ItemString parsing_item;

public:

	void SetRulesOfAlg() override;
	bool DoParse() override;
	void SetParsingStr(ItemString inp_str) override;

private:

	unsigned FindMaxQuantity();

	RuleNum FindRuleNum(const RuleNum & rulenum);
	RuleNum GetNextRule();

	void TransformAccordingRule(const ItemSymb & substr, const unsigned start, const unsigned num_of_cleaned);
	void Rollback();

	void WriteToLog(const RuleNum cur_rule_num);
	ItemString RestoreStringFromLog(const string & log_str);
	
};

