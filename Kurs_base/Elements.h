﻿#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>

using namespace std;

//----------------------------Классы-------------------------------

class ItemSymb {

	bool term;
	string content;

public:

	ItemSymb() : term(true), content("") {}		// по умолчанию создаётся терминал (символы)
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

	void SetOutputString(string & orig_str) {			// первоначальная установка строки

		ItemSymb buffer;
		for (unsigned i = 0; i < orig_str.length(); i++) {
			buffer.SetSymb(string(&orig_str[i]));
			cur_string.push_back(buffer);

			cout << "Добавлено в вектор: " << orig_str[i] << endl;
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

	//TODO: создание и синхронизация с логом разбора

public:

	ParseAlgorithm() {}
	ParseAlgorithm(ItemString inp_str) : parsing_str(inp_str) {}

	virtual void SetRulesOfAlg() = 0;
	virtual bool DoParse() = 0;
};

class LtoR_MethodAlg : public ParseAlgorithm {
public:
	void SetRulesOfAlg() override {

		//___________________________________Создание нетерминалов

		ItemSymb c_integer("<Целое>", false);
		ItemSymb c_unsigned_int("<ЦБЗ>", false);
		ItemSymb c_number("<Цифра>", false);
		ItemSymb c_sign("<Знак>", false);

		vector<ItemSymb> buf_symb;
		vector<ItemString> buf_str;
		ItemString one_str;

		/*
		buf_symb.push_back(c_sign);				// <знак><цбз>
		buf_symb.push_back(c_unsigned_int);
		*/
		buf_symb = { c_sign, c_unsigned_int };
		
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// записали вар1
		buf_symb.clear();

		buf_symb.push_back(c_unsigned_int);		// <цбз>
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// записали вар2

		ItemRule rule(c_integer, buf_str);		// ПРАВИЛО
		buf_str.clear();						// чистка вариантов
		rules.push_back(rule);					// запушили
		//----------------------------------------------------

		buf_symb.push_back(c_number);			// <цбз><цифра>
		one_str.SetString(buf_symb); 
		buf_str.push_back(one_str);				// вар1
		buf_symb.clear();

		buf_symb.push_back(c_number);			// <цифра>
		one_str.SetString(buf_symb);
		buf_str.push_back(one_str);				// вар2
		buf_symb.clear();

		rule.SetRule(c_unsigned_int, buf_str);	// ПРАВИЛО
		buf_str.clear();						// чистка вариантов
		rules.push_back(rule);					// запушили
		//----------------------------------------------------

		ItemSymb symb;
		for (int i = 0; i < 10; i++) {
			symb.SetSymb(to_string(i));
			buf_symb.push_back(symb);
			one_str.SetString(buf_symb);
			buf_str.push_back(one_str);			// новый вариант
			buf_symb.clear();
		}

		rule.SetRule(c_number, buf_str);		// ПРАВИЛО
		buf_str.clear();						// чистка вариантов
		rules.push_back(rule);					// запушили
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

		rule.SetRule(c_sign, buf_str);			// ПРАВИЛО
		buf_str.clear();						// чистка вариантов
		rules.push_back(rule);					// запушили
		//----------------------------------------------------

		cout << endl << "Правила для разбора слева направо сформированы:" << endl << endl;
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


