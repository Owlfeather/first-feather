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

class RecordLine {
protected:
	string cur_string;
	string next_rule;
public:
	virtual void PrintLine() = 0;
};


//-----------------------------------------------------------------

class LtoR_Line : public RecordLine {
public:
	void SetLine(string inp_str, string inp_rl) {
		cur_string = inp_str;
		next_rule = inp_rl;
	}
	void PrintLine() override {
		cout << "Строка: " << cur_string << " Правило: " << next_rule << endl;
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
