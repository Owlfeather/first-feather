#pragma once

#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Alg_Elements.h"


class Stack_LRk_MethodAlg : public ParseAlgorithm {

	//ItemString target_str;
	//ItemString recognized_str;

public:

	void SetRulesOfAlg() override;
	bool DoParse() override;
	void SetParsingStr(ItemString inp_str) override;

private:
	
	vector<vector<TypeOfRelation>> relation_table;
	ItemString stack_str;
	ItemString parsing_item;
	RuleNum rulenum;
	//RuleNum rulenum;
	//ItemString trio;
	unsigned point_of_entry;

	bool SelectNextSymb();
	int FindLeftNum();
	int FindRightNum();

	void DoCarry();
	bool DoConvolution(bool full);

//	RuleNum FindRuleNum();
//	bool FindCorrectTerm(const RuleNum& rulenum);
//	void RemoveMatchingSymbs();

//	void TransformAccordingRule(const RuleNum& rulenum);

	void WriteToLog(const RuleNum& rel_rule_num, const string& stack_s, const string& parse_s,
		const string& trio_str = "", const RuleNum& res_rule_num = { -1, 0 });


};
