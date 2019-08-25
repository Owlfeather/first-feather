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

	ItemSymb(): term(true), content("") {}		// по умолчанию создаётся терминал (символы)
	ItemSymb(const string inp_cont, bool inp_term = true) : content(inp_cont), term(inp_term) {}

	bool IsTerm() const { return term; }
	void MakeNonterminal() { term = false; }

	void SetSymb(const string & inp_cont, bool inp_term = true);
	void SetSymb(const char & inp_cont, bool inp_term = true);

	operator string() const { return content; }
	bool operator == (const ItemSymb& c2) const { return content == c2.content; }
	bool operator != (const ItemSymb& c2) const { return content != c2.content; }
};

//-----------------------------------------------------------------

class ItemString {

	vector<ItemSymb> cur_string;

public:

	ItemString() {}
	ItemString(const vector<ItemSymb> & inp_str) : cur_string(inp_str) {}
	ItemString(const string & orig_str);	// первоначальная установка строки
	
	void SetString(const vector<ItemSymb> & inp_str) { cur_string = inp_str; }
	int Length() { return cur_string.size(); }
	void PrintString();

	void AddSymb(const ItemSymb & inp_symb) { cur_string.push_back(inp_symb); }
	void DeleteSymb(const int & first, const int & quantity);
	
	operator string() const; 
	ItemSymb &operator[] (int i) { return cur_string[i]; }
	bool operator==(const ItemString& another_str) const { return (cur_string == another_str.cur_string); }
	bool operator!=(const ItemString& another_str) const { return (cur_string != another_str.cur_string); }
};




