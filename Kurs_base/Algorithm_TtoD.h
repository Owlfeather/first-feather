
#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Alg_Elements.h"


class TtoD_MethodAlg : public ParseAlgorithm {

	ItemString target_str;
	ItemString recognized_str;

public:

	void SetRulesOfAlg() override;
	bool DoParse() override;
	void SetParsingStr(ItemString inp_str) override;

private:

	//unsigned FindMaxQuantity();

	RuleNum FindRuleNum();
	bool FindCorrectTerm(const RuleNum & rulenum);
	void RemoveMatchingSymbs();
	//RuleNum GetNextRule();

	void TransformAccordingRule(const RuleNum & rulenum);
	RuleNum Rollback();

	void WriteToLog(const unsigned & type, const TypeOfTtoDLine & line_type,  const RuleNum & cur_rule_num = {-1, 0});
	ItemString RestoreStringFromLog(const string & log_str);

	string MakeStrForLog(ItemString & orig_str);

};
