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
	ItemRule(ItemSymb inp_l, vector <ItemString> inp_r) : left(inp_l), right(inp_r) {}

	void SetRule(ItemSymb inp_l, vector <ItemString> inp_r) {
		left = inp_l;
		right = inp_r;
	}

	void PrintRule() {
		cout << string(left) << " ::= ";
		for (unsigned i = 0; i < right.size(); i++) {
			right[i].PrintString();
			if (right.size() - i != 1) cout << "|";

		}
	}

	char GetLetterOfRule(int i) {
		if (i > right.size()) return ' ';
		else {
			return char(i + 224);
		}
	}

	ItemString &operator[] (int i) { return right[i]; }

	int RightSize() {
		return right.size();
	}

	ItemSymb GetLeft() {
		return left;
	}

};

//-----------------------------------------------------------------
struct RuleNum { int fir_num; int sec_num; };

class RecordLine {
protected:
	string cur_string;
	RuleNum rule_num;
	//int next_rule_num;
	//int next_rule_letter;
public:
	virtual void PrintLine() = 0;

	string GetCurString() {
		return cur_string;
	}

	RuleNum GetRuleNum() {
		//return next_rule_num;
		return rule_num;
	}
/*
	unsigned GetRuleLetter() {
		//return next_rule_letter;
		return rule_num.sec_num;
	}
	*/
};


//-----------------------------------------------------------------

class LtoR_Line : public RecordLine {
public:
	void SetLine(string inp_str, RuleNum inp_rnum) {
		cur_string = inp_str;
		rule_num = inp_rnum;
		//next_rule_num = inp_rl_num;
		//next_rule_letter = inp_rl_let;
	}
	void PrintLine() override {
		cout << "Строка: " << cur_string;
		if (rule_num.fir_num > -1) {
			cout << " Правило: " << rule_num.fir_num + 1 << char(rule_num.sec_num + 224) << endl;
		}
		else
			if (rule_num.sec_num == -3) {
				cout << "       конец!" << endl;
			}
			else if (rule_num.sec_num == -4){
			cout << "       разбор дальше невозможен!" << endl;
			}
			else {
				cout << "       тупик!" << endl;
			}
	}
};
//-----------------------------------------------------------------

class ParseLog {
	vector<RecordLine *> records;
public:

	ParseLog() {}

	void AddRecordLine(RecordLine *inp_rec) {
		records.push_back(inp_rec);
	}

	void PrintLogltoR() {
		cout << endl << endl << "Лог:" << endl;
		for (int i = 0; i < records.size(); i++) {
			dynamic_cast<LtoR_Line *>(records[i])->PrintLine();
		}
	}

	RecordLine * &operator[] (int i) { return records[i]; }

	int Size() {
		return records.size();
	}
};

//-----------------------------------------------------------------



class ParseAlgorithm {

protected:

	vector<ItemRule> rules;
	ItemString parsing_str;
	ParseLog parsing_log;

public:

	ParseAlgorithm() {}
	ParseAlgorithm(ItemString inp_str) : parsing_str(inp_str) {}

	void SetParsingStr(ItemString inp_str) {
		parsing_str = inp_str;
	}

	virtual void SetRulesOfAlg() = 0;
	virtual bool DoParse() = 0;
};

//-----------------------------------------------------------------
