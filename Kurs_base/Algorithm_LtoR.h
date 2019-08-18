#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "Alg_Elements.h"


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
		int rule_num, rule_letter;				// номер раскрываемого правила
		LtoR_Line * buf_line;

	//	buf_line = new LtoR_Line();
	//	buf_line->SetLine(string(parsing_str), "начало");
	//	parsing_log.AddRecordLine(buf_line);


		while (str_position != parsing_str.Length()) {

			while ((i != rules.size()) && (found == false)) {
				while ((j != rules[i].RightSize()) && (found == false)) {
					if (parsed_item == rules[i][j]) {
						cout << endl << "Рассматриваемая конструкция: ";
						parsed_item.PrintString();
						cout << endl;
						cout << "Совпадение: правило №" << i << ", пункт №" << j << endl;
						rules[i][j].PrintString();
						cout << endl;

						found = true;
						rule_num = i;
						rule_letter = j;
					}
					j++;

				}
				i++;
				j = 0;
			}
			if (found) {
				cout << "Замена: " << string(parsing_str[str_position]) << " на " << string(rules[rule_num].GetLeft());
				//

				buf_line = new LtoR_Line();
				buf_line->SetLine(string(parsing_str), to_string(rule_num + 1) + rules[rule_num].GetLetterOfRule(rule_letter));
				parsing_log.AddRecordLine(buf_line);

				//
				if (quantity == 1) {
					parsing_str[str_position] = rules[rule_num].GetLeft();
					str_position = 0;
				}
				else {
					parsing_str[str_position] = rules[rule_num].GetLeft();
					parsing_str.DeleteSymb(str_position + 1, quantity - 1);
					str_position = 0;
					quantity = 1;

				}

				parsed_item[0] = parsing_str[str_position];
				cout << endl << "Получено: ";
				parsing_str.PrintString();
				cout << endl;
			}
			else {
				if ((quantity == 1) && (parsed_item[0].IsTerm())) {
					cout << endl << "Ошибка, это не целое";
					return false;
				}

				if ((quantity == 1) && (str_position != parsing_str.Length() - 1)) {
					cout << endl << "увеличиваем символы поиска" << endl;
					parsed_item.AddSymb(parsing_str[str_position + 1]);
					quantity = 2;
				}
				else {
					cout << endl << "уменьшаем символы поиска" << endl;
					parsed_item.DeleteSymb(0, 1);
					str_position++;
					quantity = 1;
				}
			}
			i = j = 0;
			found = false;
		}

		cout << endl;
		parsing_str.PrintString();

		
		parsing_log.PrintLogltoR();
		//???
		return true;
	}
};

