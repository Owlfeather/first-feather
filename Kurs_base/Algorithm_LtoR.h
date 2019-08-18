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

		//__________________________________________�������� ������������

		ItemSymb c_integer("<�����>", false);
		ItemSymb c_unsigned_int("<���>", false);
		ItemSymb c_number("<�����>", false);
		ItemSymb c_sign("<����>", false);

		vector<ItemSymb> buf_symb;					// ����������-������ ��� �������� ��������� �������
		vector<ItemString> buf_str;					// ����������-������ ��� �������� ���� ��������� ��������� ������� (������ ����� �������)

		buf_symb = { c_sign, c_unsigned_int };		// <����><���>
		buf_str.push_back(ItemString(buf_symb));	// �������� ������� 1

		buf_symb = { c_unsigned_int };				// <���>
		buf_str.push_back(ItemString(buf_symb));	// �������� ������� 2

		ItemRule rule(c_integer, buf_str);			// �������
		buf_str.clear();							// ������ ���������
		rules.push_back(rule);						// ���������� ������ �������
		//----------------------------------------------------

		buf_symb.push_back(c_number);				// <���><�����>
		buf_str.push_back(ItemString(buf_symb));	// �������� ������� 1

		buf_symb = { c_number };					// <�����>
		buf_str.push_back(ItemString(buf_symb));	// �������� ������� 2

		rule.SetRule(c_unsigned_int, buf_str);		// �������
		buf_str.clear();							// ������ ���������
		rules.push_back(rule);						// ���������� ������ �������

		//__________________________________________�������� ����������

		for (int i = 0; i < 10; i++) {
			buf_symb = { ItemSymb(to_string(i)) };
			buf_str.push_back(ItemString(buf_symb));   // ����� �������
		}

		rule.SetRule(c_number, buf_str);			// �������
		buf_str.clear();							// ������ ���������
		rules.push_back(rule);						// ���������� ������ �������
		//----------------------------------------------------

		buf_symb = { ItemSymb("+") };
		buf_str.push_back(ItemString(buf_symb));	// +

		buf_symb = { ItemSymb("-") };
		buf_str.push_back(ItemString(buf_symb));	// -

		rule.SetRule(c_sign, buf_str);				// �������
		rules.push_back(rule);						// ���������� ������ �������
		//----------------------------------------------------

		cout << endl << "������� ��� ������� ����� ������� ������������:" << endl << endl;
		for (unsigned i = 0; i < rules.size(); i++) {
			rules[i].PrintRule();
			cout << endl;
		}
	}

	bool DoParse() override {

		int str_position = 0;										// ������ �������������� � ������ ������
		int quantity = 1;											// ���������� ��������������� �������� (���� 1)
		ItemString parsed_item({ parsing_str[str_position] });

		//parsed_item.AddSymb(parsing_str[1]);
		//cout << endl << "��������������� �����������: ";
		//parsed_item.PrintString();
		//cout << endl;
		//cout << endl;
		//rules[3][1].PrintString();

		int i = 0, j = 0;
		bool found = false;			// ������ �� �������
		int rule_num, rule_letter;				// ����� ������������� �������
		LtoR_Line * buf_line;

	//	buf_line = new LtoR_Line();
	//	buf_line->SetLine(string(parsing_str), "������");
	//	parsing_log.AddRecordLine(buf_line);


		while (str_position != parsing_str.Length()) {

			while ((i != rules.size()) && (found == false)) {
				while ((j != rules[i].RightSize()) && (found == false)) {
					if (parsed_item == rules[i][j]) {
						cout << endl << "��������������� �����������: ";
						parsed_item.PrintString();
						cout << endl;
						cout << "����������: ������� �" << i << ", ����� �" << j << endl;
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
				cout << "������: " << string(parsing_str[str_position]) << " �� " << string(rules[rule_num].GetLeft());
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
				cout << endl << "��������: ";
				parsing_str.PrintString();
				cout << endl;
			}
			else {
				if ((quantity == 1) && (parsed_item[0].IsTerm())) {
					cout << endl << "������, ��� �� �����";
					return false;
				}

				if ((quantity == 1) && (str_position != parsing_str.Length() - 1)) {
					cout << endl << "����������� ������� ������" << endl;
					parsed_item.AddSymb(parsing_str[str_position + 1]);
					quantity = 2;
				}
				else {
					cout << endl << "��������� ������� ������" << endl;
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

