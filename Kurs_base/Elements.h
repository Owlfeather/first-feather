#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

//----------------------------������-------------------------------

class ItemSymb {

	bool term;
	string content;

public:

	ItemSymb() : term(true), content("") {}		// �� ��������� �������� �������� (�������)
	ItemSymb(string inp_cont, bool inp_term = true) : content(inp_cont), term(inp_term) {}

	void SetSymb(string inp_cont, bool inp_term = true) {
		content = inp_cont;
		term = inp_term;
	}

	operator string() const {
		return content;
	}
};

//-----------------------------------------------------------------

class ItemString {

	vector<ItemSymb> cur_string;

public:

	ItemString() {}
	ItemString(vector<ItemSymb> inp_str) : cur_string(inp_str) {}

	void SetOutputString(string & orig_str) {			// �������������� ��������� ������

		ItemSymb buffer;
		for (unsigned i = 0; i < orig_str.length(); i++) {
			buffer.SetSymb(string(&orig_str[i]));
			cur_string.push_back(buffer);

			cout << "��������� � ������: " << orig_str[i] << endl;
		}
	}

	void SetString(vector<ItemSymb> inp_str) {
		cur_string = inp_str;
	}

	void PrintString() {
		for (unsigned i = 0; i < cur_string.size(); i++) {
			cout << string(cur_string[i]);
		}
	}
};

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

};

//-----------------------------------------------------------------

class ParseAlgorithm {

protected:

	vector<ItemRule> rules;
	ItemString parsing_str;

	//TODO: �������� � ������������� � ����� �������

public:

	ParseAlgorithm() {}
	ParseAlgorithm(ItemString inp_str) : parsing_str(inp_str) {}

	virtual void SetRulesOfAlg() = 0;
	virtual bool DoParse() = 0;
};

class LtoR_MethodAlg : public ParseAlgorithm {
public:
	void SetRulesOfAlg() override {

		//___________________________________�������� ������������

		ItemSymb c_integer("<�����>", false);
		ItemSymb c_unsigned_int("<���>", false);
		ItemSymb c_number("<�����>", false);
		ItemSymb c_sign("<����>", false);

		vector<ItemSymb> buf_symb;
		vector<ItemString> buf_str;
		ItemString one_str;

		/*
		buf_symb.push_back(c_sign);				// <����><���>
		buf_symb.push_back(c_unsigned_int);
		*/
		buf_symb = { c_sign, c_unsigned_int };
		
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// �������� ���1
		buf_symb.clear();

		buf_symb.push_back(c_unsigned_int);		// <���>
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// �������� ���2

		ItemRule rule(c_integer, buf_str);		// �������
		buf_str.clear();						// ������ ���������
		rules.push_back(rule);					// ��������
		//----------------------------------------------------

		buf_symb.push_back(c_number);			// <���><�����>
		one_str.SetString(buf_symb); 
		buf_str.push_back(one_str);				// ���1
		buf_symb.clear();

		buf_symb.push_back(c_number);			// <�����>
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// ���2
		buf_symb.clear();

		rule.SetRule(c_unsigned_int, buf_str);	// �������
		buf_str.clear();						// ������ ���������
		rules.push_back(rule);					// ��������
		//----------------------------------------------------

		ItemSymb symb;
		for (int i = 0; i < 10; i++) {
			symb.SetSymb(to_string(i));
			buf_symb.push_back(symb);
			one_str.SetString(buf_symb);
			buf_str.push_back(one_str);			// ����� �������
			buf_symb.clear();
		}

		rule.SetRule(c_number, buf_str);		// �������
		buf_str.clear();						// ������ ���������
		rules.push_back(rule);					// ��������
		//----------------------------------------------------

		symb.SetSymb("+");
		buf_symb.push_back(symb);
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// +
		buf_symb.clear();

		symb.SetSymb("-");
		buf_symb.push_back(symb);
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// -
		buf_symb.clear();

		rule.SetRule(c_sign, buf_str);			// �������
		buf_str.clear();						// ������ ���������
		rules.push_back(rule);					// ��������
		//----------------------------------------------------

		cout << endl << "������� ��� ������� ����� ������� ������������:" << endl << endl;
		for (unsigned i = 0; i < rules.size(); i++) {
			rules[i].PrintRule();
			cout << endl;
		}
	}

	bool DoParse() override {

		//???
		return true;
	}
};


