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
	/*
	void SetString(vector<ItemSymb> inp_str) {
		cur_string = inp_str;
	} */

	void AddSymb(ItemSymb inp_symb) {
		cur_string.push_back(inp_symb);
	}

	int Length() {
		return cur_string.size();
	}

	void DeleteSymb(int first, int quantity) {
		cur_string.erase(cur_string.begin() + first, cur_string.begin() + first + quantity);
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




