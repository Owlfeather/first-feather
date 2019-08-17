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

	void ChangeSymb(ItemSymb inp_symb, int position) {
		cur_string[position] = inp_symb;
	}

	int Length() {
		return cur_string.size();
	}

	void DeleteSymb(int first, int quantity) {
		cur_string.erase(cur_string.begin() + first, cur_string.begin() + first + quantity);
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

	ItemString &operator[] (int i) { return right[i]; }

	int RightSize() {
		return right.size();
	}

	ItemSymb GetLeft() {
		return left;
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

	void SetParsingStr(ItemString inp_str) {
		parsing_str = inp_str;
	}

	virtual void SetRulesOfAlg() = 0;
	virtual bool DoParse() = 0;
};

class LtoR_MethodAlg : public ParseAlgorithm {
public:
	void SetRulesOfAlg() override {

		//__________________________________________Создание нетерминалов

		ItemSymb c_integer("<Целое>", false);
		ItemSymb c_unsigned_int("<ЦБЗ>", false);
		ItemSymb c_number("<Цифра>", false);
		ItemSymb c_sign("<Знак>", false);

		vector<ItemSymb> buf_symb;					// переменная-шаблон для варианта раскрытия правила
		vector<ItemString> buf_str;					// переменная-шаблон для хранения всех вариантов раскрытия правила (правая часть правила)
		
		buf_symb = { c_sign, c_unsigned_int };		// <знак><цбз>
		buf_str.push_back(ItemString(buf_symb));	// записали вариант 1

		buf_symb = { c_unsigned_int };				// <цбз>
		buf_str.push_back(ItemString(buf_symb));	// записали вариант 2

		ItemRule rule(c_integer, buf_str);			// ПРАВИЛО
		buf_str.clear();							// чистка вариантов
		rules.push_back(rule);						// добавление нового правила
		//----------------------------------------------------

		buf_symb.push_back(c_number);				// <цбз><цифра>
		buf_str.push_back(ItemString(buf_symb));	// записали вариант 1

		buf_symb = { c_number };					// <цифра>
		buf_str.push_back(ItemString(buf_symb));	// записали вариант 2

		rule.SetRule(c_unsigned_int, buf_str);		// ПРАВИЛО
		buf_str.clear();							// чистка вариантов
		rules.push_back(rule);						// добавление нового правила

		//__________________________________________Создание терминалов

		for (int i = 0; i < 10; i++) {
			buf_symb = { ItemSymb(to_string(i)) };
			buf_str.push_back(ItemString(buf_symb));   // новый вариант
		}

		rule.SetRule(c_number, buf_str);			// ПРАВИЛО
		buf_str.clear();							// чистка вариантов
		rules.push_back(rule);						// добавление нового правила
		//----------------------------------------------------

		buf_symb = { ItemSymb("+") };
		buf_str.push_back(ItemString(buf_symb));	// +

		buf_symb = { ItemSymb("-") };
		buf_str.push_back(ItemString(buf_symb));	// -

		rule.SetRule(c_sign, buf_str);				// ПРАВИЛО
		rules.push_back(rule);						// добавление нового правила
		//----------------------------------------------------

		cout << endl << "Правила для разбора слева направо сформированы:" << endl << endl;
		for (unsigned i = 0; i < rules.size(); i++) {
			rules[i].PrintRule();
			cout << endl;
		}
	}

	bool DoParse() override {

		int str_position = 0;										// разбор осуществляется с начала строки
		int quantity = 1;											// количество рассматриваемых символов (пока 1)
		ItemString parsed_item({ parsing_str[str_position] });

		//parsed_item.AddSymb(parsing_str[1]);
		//cout << endl << "Рассматриваемая конструкция: ";
		//parsed_item.PrintString();
		//cout << endl;
		//cout << endl;
		//rules[3][1].PrintString();

		int i = 0, j = 0;
		bool found = false;			// найден ли элемент
		int rule_num;				// номер раскрываемого правила

		while (str_position != parsing_str.Length()) {

			while ((i != rules.size()) && (found == false)) {
				while ((j != rules[i].RightSize()) && (found == false)) {
					if (parsed_item == rules[i][j]) {
						cout << endl << "Рассматриваемая конструкция: ";
						parsed_item.PrintString();
						cout << endl;
						cout << "Совпадение: правило №" << i <<", пункт №" << j << endl;
						rules[i][j].PrintString();
						cout << endl;
						found = true;
						rule_num = i;
					}
					j++;

				}
				i++;
				j = 0;
			}
			if (found) {
				cout << "Замена: " << string(parsing_str[str_position]) << " на " << string(rules[rule_num].GetLeft());
				if (quantity == 1) {
					parsing_str[str_position] = rules[rule_num].GetLeft();
					str_position = 0;
				} 
				else {
					parsing_str[str_position] = rules[rule_num].GetLeft();
					parsing_str.DeleteSymb(str_position + 1, quantity-1);
					str_position = 0;
					quantity = 1;

				}
				parsed_item.ChangeSymb(parsing_str[str_position], 0);
				cout << endl << "Получено: ";
				parsing_str.PrintString();
				cout << endl;
			}
			else {
				if ((quantity == 1)&&(str_position != parsing_str.Length()-1)) {
					cout << endl << "увеличиваем символы поиска"<< endl;
					parsed_item.AddSymb(parsing_str[str_position + 1]);
					quantity = 2;
				}
				else {
					cout << endl << "уменьшаем символы поиска" << endl;
					parsed_item.DeleteSymb(0, 1);
					str_position ++;
					quantity = 1;
				}
			}
			i = j = 0;
			found = false;
		}

		cout << endl;
		parsing_str.PrintString();
		//???
		return true;
	}
};


