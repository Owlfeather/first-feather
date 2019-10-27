#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Basic_Elements.h"

using namespace std;

//-----------------------------------------------------------------

class ItemRule {

	ItemSymb left;
	vector<ItemString> right;

public:

	ItemRule() {}
	ItemRule(const ItemSymb & inp_l, const vector <ItemString> & inp_r) : left(inp_l), right(inp_r) {}

	void SetRule(const ItemSymb & inp_l, const vector <ItemString> & inp_r);
	int RightSize() { return right.size(); }
	ItemSymb GetLeft() { return left; }
	void PrintRule();

	ItemString &operator[] (const int i) { return right[i]; }
};

//-----------------------------------------------------------------

struct RuleNum { int fir_num; int sec_num; };

//-----------------------------------------------------------------


class RecordLine {

protected:

	string cur_string;
	RuleNum rule_num;

public:

	virtual void PrintLine() = 0;

	string GetCurString() { return cur_string; }
	RuleNum GetRuleNum() { return rule_num; }
};


//-----------------------------------------------------------------

class LtoR_Line : public RecordLine {

public:

	void SetLine(const string & inp_str, const RuleNum & inp_rnum);
	void PrintLine() override;
};

//-----------------------------------------------------------------

class TtoD_Line : public RecordLine {

	string recognized;
	string target;
	unsigned type;
	TypeOfTtoDLine type_of_line;

public:

	void SetLine(const string & rec_str, const string & pars_str, 
		const string & targ_str, unsigned type_of_l, 
		TypeOfTtoDLine line_type, const RuleNum & inp_rnum);
	void PrintLine() override;

	void MarkRollback() { rule_num.sec_num++; }
	void TypeMarkRollback() { type_of_line = TypeOfTtoDLine::ROLLB_IMPOSS; }
	string MakePrintable(string & str_with_seps);
	string GetRecString() { return recognized; }
	string GetTargString() { return target; }
	TypeOfTtoDLine& GetTypeOfLine() { return type_of_line; }
};

//-----------------------------------------------------------------

class LLk_TtoD_Line : public RecordLine {

	string stack_str;
	//unsigned type;
	//TypeOfTtoDLine type_of_line;

public:

	void SetLine(const string& pars_str,
		const string& st_str, const RuleNum& inp_rnum);
	void PrintLine() override;

	string GetStackString() { return stack_str; }

	//void MarkRollback() { rule_num.sec_num++; }
	//void TypeMarkRollback() { type_of_line = TypeOfTtoDLine::ROLLB_IMPOSS; }
	//string MakePrintable(string& str_with_seps);
	//string GetRecString() { return recognized; }
	
	//TypeOfTtoDLine& GetTypeOfLine() { return type_of_line; }
};

//-----------------------------------------------------------------


class ParseLog {
	vector<RecordLine *> records;
public:

	ParseLog() {}

	void AddRecordLine(RecordLine *inp_rec) { records.push_back(inp_rec); }
	int Size() { return records.size(); }
	void PrintLogltoR();
	void PrintLogTtoD();
	void PrintLogLLk();

	RecordLine * &operator[] (int i) { return records[i]; }
};

//-----------------------------------------------------------------

class ParseAlgorithm {

protected:

	vector<ItemRule> rules;
	ItemString parsing_str;
	ParseLog parsing_log;

public:

	ParseAlgorithm() {}
	ParseAlgorithm(ItemString & inp_str) : parsing_str(inp_str) {}

	//void SetParsingStr(ItemString inp_str) { parsing_str = inp_str; }

	virtual void SetParsingStr(ItemString inp_str) = 0;
	virtual void SetRulesOfAlg() = 0;
	virtual bool DoParse() = 0;
};

//-----------------------------------------------------------------
