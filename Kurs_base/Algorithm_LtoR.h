#pragma once
#include <locale.h>
#include <stdio.h>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <tuple>
#include "Alg_Elements.h"


class LtoR_MethodAlg : public ParseAlgorithm {
	ItemString parsed_item;
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

	unsigned FindMaxQuantity() {
		unsigned max_quantity = 0;
		unsigned cur_length = 0;
		for (int i = 0; i < rules.size(); i++) {
			for (int j = 0; j < rules[i].RightSize(); j++) {
				cur_length = rules[i][j].Length();
				if (cur_length > max_quantity) {
					max_quantity = cur_length;
				}
			}
		}
		return max_quantity;
	}

	RuleNum FindRuleNum(RuleNum rulenum = {0, 0}) {

		cout << endl << "Производится разбор строки: ";
		parsing_str.PrintString();

		int rules_number = rules.size();		// суммарное число правил
		int subrules_number;					// число подпунктов (вариантов расшифровки) одного правила

		for (int i = rulenum.fir_num; i < rules_number; i++) {
			subrules_number = rules[i].RightSize();
			for (int j = rulenum.sec_num; j < subrules_number; j++) {
				if (parsed_item == rules[i][j]) {

					cout << endl << endl << "СОВПАДЕНИЕ";
					cout << endl << "Рассматриваемая конструкция: ";
					parsed_item.PrintString();
					cout << endl;
					cout << "Совпадение: правило №" << i << ", пункт №" << j <<" : ";
					rules[i][j].PrintString();
					cout << endl;
					return RuleNum{ i, j };
				}
			}
		}
		return RuleNum{ -1, -1 };
	};

	void WriteToLog(RuleNum cur_rule_num) {
		LtoR_Line * buf_line;
		cout << endl << "Осуществляется запись в лог" << endl;
		buf_line = new LtoR_Line();
		//
		string str_with_separators;
		for (int i = 0; i < parsing_str.Length(); i++) {
			str_with_separators += string(parsing_str[i]) + '\n';
		}
		//

		buf_line->SetLine(str_with_separators, cur_rule_num);
		parsing_log.AddRecordLine(buf_line);
	}

	ItemString RestoreStringFromLog(string log_str) {////////////////////////////////////////////// символы одиночные не восстанавливает

		ItemString restored_str;
		char * arr_of_char = new char[log_str.size() + 1];
		copy(log_str.begin(), log_str.end(), arr_of_char);
		arr_of_char[log_str.size()] = '\0';
		char * context;
		bool added = false;

		//const char * separator = new char('\n');
		char * item;
		item = strtok_s(arr_of_char, "\n", &context);
		while (item != NULL)
		{
			for (int i = 0; i < rules.size(); i++) {
				if (string(item) == string(rules[i].GetLeft())) {
					added = true;
					restored_str.AddSymb(rules[i].GetLeft());
				}
			}
			if (!added) {
				restored_str.AddSymb(ItemSymb(item));
			}
			item = strtok_s(NULL, "\n", &context);
			added = false;
		}

		return restored_str;
	}

	void Rollback() {

		cout << endl << "Выполняется откат назад" << endl;
		//parsing_str.SetStringFromLog((*(parsing_log[parsing_log.Size() - 2])).GetCurString());
		parsing_str = RestoreStringFromLog((*(parsing_log[parsing_log.Size() - 2])).GetCurString());
		RuleNum prev_rule = (*(parsing_log[parsing_log.Size() - 2])).GetRuleNum();

		//if (prev_rule.sec_num == rules[prev_rule.fir_num].RightSize() - 1) prev_rule.fir_num++;
		//else prev_rule.sec_num++;

		parsed_item = rules[prev_rule.fir_num][prev_rule.sec_num];

		cout << endl << "Разбираемая строка: ";
		parsing_str.PrintString();
			//cout << endl;
			//cout << endl << "Начнём поиск с правила: " << prev_rule.fir_num << ", " << prev_rule.sec_num << endl;
		cout << endl << "Будет выполняться поиск конструкции: ";
		parsed_item.PrintString();
		cout << endl;
		
	}

	RuleNum GetNextRule() {
		RuleNum prev_rule = (*(parsing_log[parsing_log.Size() - 2])).GetRuleNum();
		if (prev_rule.sec_num == rules[prev_rule.fir_num].RightSize() - 1) prev_rule.fir_num++;
		else prev_rule.sec_num++;
		cout << endl << "Начнём поиск с правила: " << prev_rule.fir_num << ", " << prev_rule.sec_num << endl;
		return prev_rule;
	}

	bool DoParse() override {

		int okey = 0;
		unsigned entry_point = 0;		// указывает на следующий для добавления в parsing_item символ
		//ItemString parsed_item({ parsing_str[entry_point] });		// разбираемый участок строки
		parsed_item.SetString({ parsing_str[entry_point] });	// разбираемый участок строки
//		LtoR_Line * buf_line;									// буфер для записи в лог разбора
		RuleNum next_rule = { 0, 0 };

		unsigned quantity = FindMaxQuantity();

		while (okey == 0) {
		RuleNum rule_num = FindRuleNum(next_rule);

		if (rule_num.fir_num != -1) {		// Если правило нашлось

			cout <<endl << "Замена: ";
			parsed_item.PrintString();
			cout << " на ";
			cout << string(rules[rule_num.fir_num].GetLeft());

			// запись в лог
			WriteToLog(rule_num);
			//

			// замена строк
			parsing_str.TransformAccordingRule(rules[rule_num.fir_num].GetLeft(), entry_point, parsed_item.Length());
			parsed_item.SetString({ parsing_str[0] });
			//

			if (rule_num.fir_num == 0) {
				// поменяли на аксиому
				if (parsing_str.Length() == 1) {
					// выявлено целое, конец разбора

					WriteToLog({-3, -3}); // КОД -3 - конец разбора
					okey = 1;
					//return true;
				}
				else {
					// откат назад
					WriteToLog({ -2, -2 }); // КОД -2 - тупик, откат назад
					Rollback();
					next_rule = GetNextRule();
				}
			}
			else {
				entry_point = 0;
				// в начало
			}
		}
		else { // если правило не нашлось
			if ((parsed_item.Length() == 1) && (parsed_item[0].IsTerm())) {

				//запись в лог
				WriteToLog({ -4, -4 });

				cout << endl << "Ошибка, неопознанный символ : ";
				parsed_item.PrintString();
				cout << endl;
				okey = -1;
				//return false;
			}
			if (entry_point == parsing_str.Length() - 1) {
				//запись в лог
				WriteToLog({ -4, -4 });

				cout << endl << "Все возможные преобразования выполнены, но строка не является целым числом";
				cout << endl;
				okey = -1;
				//return false;
			}
			if ((parsed_item.Length() < quantity) && (entry_point != parsing_str.Length() - 1)) {
				parsed_item.AddSymb(parsing_str[entry_point + parsed_item.Length()]);
			}
			else {
				parsed_item.DeleteSymb(0, 1);
				entry_point++;
			}
			next_rule = { 0, 0 };
			// в начало
		}
		}
		parsing_log.PrintLogltoR();
		return true;

		//cout << "Замена: " << string(parsing_str[str_position]) << " на " << string(rules[rule_num].GetLeft());
	}

	/*
	bool DoParse() override {

		int str_position = 0;										// разбор осуществляется с начала строки
		int quantity = 1;											// количество рассматриваемых символов (пока 1)
		ItemString parsed_item({ parsing_str[str_position] });

		int i = 0, j = 0;
		bool found = false;			// найден ли элемент
		int rule_num, rule_letter;	// номер раскрываемого правила
		LtoR_Line * buf_line;
		int prev_position;
		int prev_quantity;


		while (str_position != parsing_str.Length()) {
			cout << "ищем: ";
			parsed_item.PrintString();
			cout <<endl << "начинаем поиск: правило №" << i << ", пункт №" << j << endl;

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
				prev_position = str_position;
				prev_quantity = quantity;
				cout << "Замена: " << string(parsing_str[str_position]) << " на " << string(rules[rule_num].GetLeft());

				//запись в лог
				cout << endl << "запись в лог" << endl;
				buf_line = new LtoR_Line();
				buf_line->SetLine(string(parsing_str), rule_num, rule_letter);
				parsing_log.AddRecordLine(buf_line);

				//

				parsing_str[str_position] = rules[rule_num].GetLeft();

				if (quantity != 1) {
					parsing_str.DeleteSymb(str_position + 1, quantity - 1);
					quantity = 1;
					prev_quantity = 2;

				}
				else
					prev_quantity = 1;
				str_position = 0;

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
			if ((quantity == 1) && (parsed_item[0] == rules[0].GetLeft())) {
			//if (rule_num == 0)  {
				//str_position = prev_position;
				cout << "появилась аксиома!";
				if (parsing_str.Length() == 1) return true;
				else {
					//
					cout << endl << "запись в лог" << endl;
					buf_line = new LtoR_Line();
					buf_line->SetLine(string(parsing_str), -1, -1);
					parsing_log.AddRecordLine(buf_line);
					//
					cout << "откат назад";
					parsing_str.SetStringFromLog((*(parsing_log[parsing_log.Size() - 2])).GetCurString());
					i = (*(parsing_log[parsing_log.Size() - 2])).GetRuleNum();
					j = (*(parsing_log[parsing_log.Size() - 2])).GetRuleLetter();
					if (j == rules[i].RightSize() - 2) i++;
					else j++;

					str_position = prev_position;
					quantity = prev_quantity;
					for (int i = 0; i < quantity; i++) {
						parsed_item[i] = parsing_str[str_position + i];
					};
					cout << endl << "Получено: ";
					parsing_str.PrintString();
					cout << endl;
					cout << endl << "Начнём поиск с правила: " << i <<", " << j << endl;
					cout << endl << "поиск: ";
					parsed_item.PrintString();
					cout << endl << "количество символов: " << quantity;

				}
			}
			else {
				i = j = 0;
			}
			//i = j = 0;
			found = false;
		}

		cout << endl;
		parsing_str.PrintString();

		
		parsing_log.PrintLogltoR();
		return true;
	}
	*/
};

