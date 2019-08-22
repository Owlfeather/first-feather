#pragma once
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

	void SetSymb(char inp_cont, bool inp_term = true) {
		content = { inp_cont };
		term = inp_term;
	}

	bool IsTerm() {
		return term;
	}

	void MakeNonterminal() {
		term = false;
	}

	string PureContent() {
		string result;
		for (int i = 0; i < content.size(); i++) {
			if ((i != 0) && (i != content.size() - 1))
				result += content[i];
		}
		return result;
	}

	operator string() const {
		return content;
	}

	bool operator == (const ItemSymb& c2) const
	{
		return content == c2.content;
	}

	bool operator != (const ItemSymb& c2) const
	{
		return content != c2.content;
	}
};

//-----------------------------------------------------------------

class ItemString {

	vector<ItemSymb> cur_string;

public:

	ItemString() {}
	ItemString(vector<ItemSymb> inp_str) : cur_string(inp_str) {}

	ItemString(string & orig_str) {			// первоначальная установка строки
		ItemSymb buffer;
		for (unsigned i = 0; i < orig_str.length(); i++) {
			buffer.SetSymb(orig_str[i]);
			cur_string.push_back(buffer);

			//cout << "Добавлено в вектор: " << orig_str[i] << endl;
			cout << "Добавлено в вектор: " << string(cur_string[i]) << endl;
			
		}
		cout << "Длина вектора: " << cur_string.size() << endl;
	} 
	
	void SetString(vector<ItemSymb> inp_str) {
		cur_string = inp_str;
	} 

	void AddSymb(ItemSymb inp_symb) {
		cur_string.push_back(inp_symb);
	}

	int Length() {
		return cur_string.size();
	}

	void DeleteSymb(int first, int quantity) {
		cur_string.erase(cur_string.begin() + first, cur_string.begin() + first + quantity);
	}

	void TransformAccordingRule(ItemSymb substr, unsigned start, unsigned num_of_cleaned) {
		cur_string[start] = substr;
		if (num_of_cleaned > 1) {
			DeleteSymb(start + 1, num_of_cleaned - 1);
			//cur_string.erase(cur_string.begin() + start + 1, cur_string.begin() + num_of_cleaned - 1);
		}
	}

	void SetStringFromLog(string log_str) {
		cur_string.clear();
		char * writable = new char[log_str.size() + 1];
		copy(log_str.begin(), log_str.end(), writable);
		writable[log_str.size()] = '\0'; 
		char * context;
		//bool added = false;

		char * pch;
		pch = strtok_s(writable, "<>", &context);
		while (pch != NULL)
		{ 
			/* for (int i = 0; i < rules.size(); i++) {
				if (string(pch) == string(rules[i].GetLeft())) {
					cur_string.push_back(ItemSymb(pch));
					added = true;
				}
			} 

			if (!added) {
				for (int i = 0; i < string(pch).length(); i++) {
					cur_string.push_back(ItemSymb(&pch[i]));
				}
			} */

			//if (string(pch).length())
			cur_string.push_back(ItemSymb(pch));
			pch = strtok_s(NULL, "<>", &context);
			//added = false;
		}

		for (int i = 0; i < cur_string.size(); i++) {
			if (string(cur_string[i]).size() > 1) 
				cur_string[i].SetSymb("<" + string(cur_string[i]) + ">", false);
		}

		delete[] writable;
	}

	operator string() const {
		string result;
		for (int i = 0; i < cur_string.size(); i++) {
			result += string(cur_string[i]);
		}
		return result;
	}

	ItemSymb &operator[] (int i) { return cur_string[i]; }
	
	 bool operator==(const ItemString& another_str) const {
			return (cur_string == another_str.cur_string);
	}

	 bool operator!=(const ItemString& another_str) const {
			return (cur_string != another_str.cur_string);
	}

	void PrintString() {
		for (unsigned i = 0; i < cur_string.size(); i++) {
			cout << string(cur_string[i]);
		}
	}
};




