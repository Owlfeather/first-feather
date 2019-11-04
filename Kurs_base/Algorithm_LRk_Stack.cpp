#include"Algorithm_LRk_Stack.h"
#include <cctype>
#include<conio.h>
#include<stdio.h>
#include<set>

void Stack_LRk_MethodAlg::SetRulesOfAlg()
{
	//__________________________________________Создание нетерминалов

	ItemSymb c_calculation("<Выражение>", false);
	ItemSymb c_term("<Терм>", false);
	ItemSymb c_factor("<Множ>", false);
	ItemSymb c_id("<Ид>");

	vector<ItemSymb> buf_symb;					// переменная-шаблон для варианта раскрытия правила
	vector<ItemString> buf_str;					// переменная-шаблон для хранения всех вариантов 
												// раскрытия правила (правая часть правила)

	buf_symb = { c_term };									// <Терм>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { c_term, ItemSymb("+"), c_calculation };	// <Терм>+<Выражение>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { c_term, ItemSymb("-"), c_calculation };	// <Терм>-<Выражение>
	buf_str.push_back(ItemString(buf_symb));

	ItemRule rule(c_calculation, buf_str);					// ПРАВИЛО
	buf_str.clear();										// чистка вариантов
	rules.push_back(rule);									// добавление нового правила
	//----------------------------------------------------

	buf_symb = { c_factor };								// <Множ>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { c_factor, ItemSymb("*"), c_term };			// <Множ>*<Терм>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { c_factor, ItemSymb("/"), c_term };			// <Множ>/<Терм>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_term, buf_str);							// ПРАВИЛО
	buf_str.clear();										// чистка вариантов
	rules.push_back(rule);									// добавление нового правила
	//----------------------------------------------------

	buf_symb = { c_id };									// <Ид>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("("), c_calculation, ItemSymb(")") };	//(<Выражение>)								// <Ид>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_factor, buf_str);							// ПРАВИЛО
	buf_str.clear();											// чистка вариантов
	rules.push_back(rule);										// добавление нового правила
	//----------------------------------------------------


	cout << endl << "Правила для разбора стековым методом для LRk сформированы:" << endl << endl;
	for (unsigned i = 0; i < rules.size(); i++) {
		rules[i].PrintRule();
		cout << endl;
	}


	relation_table.push_back({ CARRY, CARRY, CARRY, ERROR, EXIT });
	relation_table.push_back({ CONV, CARRY, CARRY, CONV, CONV });
	relation_table.push_back({ CONV, CONV, CARRY, CONV, CONV });
	relation_table.push_back({ CARRY, CARRY, CARRY, CONV_BR, ERROR });
	relation_table.push_back({ CONV, CONV, ERROR, CONV, CONV });

	cout << endl << "Таблица отношений для разбора стековым методом для LRk сформирована:" << endl << endl;

	for (unsigned i = 0; i < relation_table.size(); i++) {
		for (unsigned j = 0; j < relation_table[i].size(); j++) {
			switch (relation_table[i][j])
			{
			case CARRY:
			{
				cout << "< ";
				break;
			}
			case CONV:
			{
				cout << "> ";
				break;
			}
			case CONV_BR:
			{
				cout << "= ";
				break;
			}
			case ERROR:
			{
				cout << "? ";
				break;
			}
			case EXIT:
			{
				cout << "В ";
				break;
			}
			
			}
			
		}
		cout << "\n";
	}
	cout << "\n";
}

bool Stack_LRk_MethodAlg::DoParse()
{
	bool okey = true;
	RuleNum rulenum;
	ItemSymb beg("beg");
	ItemSymb end("end");
	point_of_entry = 0;
	TypeOfRelation relation;
	bool conv_happened = false;
	bool conv_done = true;
	bool symb_found = true;


	stack_str.SetString({ beg }); // beg

	while (okey) {

		if (!conv_happened) {
			symb_found = SelectNextSymb();

			if (!symb_found) {
				cout << "\nОшибка, дальнейший разбор невозможен\n";
				okey = false;
				return true;
			}
		}
		else {
			conv_happened = false;
		}
		
		rulenum = { FindLeftNum(), FindRightNum() };

		if (rulenum.sec_num == -1) {
			cout << "\nОшибка, дальнейший разбор невозможен\n";
			okey = false;
			return true;
		}
		
		cout << "\nПолученное правило: [" << rulenum.fir_num <<", " << rulenum.sec_num << "]\n";
		relation = relation_table[rulenum.fir_num][rulenum.sec_num];
		cout << "Отношение: " << relation <<endl;

		switch (relation)
		{
		case CARRY:
			DoCarry();
			break;
		case CONV:
			conv_done = DoConvolution(true);
			conv_happened = true;
			if (!conv_done) {
				cout << "\n\nОшибка, дальнейший разбор невозможен";
				okey = false;
			}
			break;
		case CONV_BR:
			conv_done = DoConvolution(false);
			conv_happened = true;
			if (!conv_done) {
				cout << "\n\nОшибка, дальнейший разбор невозможен";
				okey = false;
			}
			break;
		case ERROR:
			cout << "\n\nОшибка, дальнейший разбор невозможен";
			okey = false;
			break;
		case EXIT:
			cout << "\n\nРазбор завершён успешно";
			okey = false;
			break;
		default:
			break;
		}



		//okey = false;

	}


	return true;
}

void Stack_LRk_MethodAlg::SetParsingStr(ItemString inp_str)
{
	unsigned inp_size = inp_str.Length();
	int symb_code;
	bool prev_is_letter = false;

	for (unsigned i = 0; i < inp_size; i++) {

		symb_code = int(string(inp_str).c_str()[i]);

		if ((isalpha(symb_code))
			|| ((symb_code <0 && symb_code > -65)
				|| (symb_code > 64 && symb_code < 91)
				|| (symb_code > 96 && symb_code < 123)))
		{
			///это буква
			if (!prev_is_letter) { // буква встретилась первой
				prev_is_letter = true;
				parsing_str.AddSymb(ItemSymb("<Ид>"));
			}
			// иначе - просто следующая буква идентификатора	
		}
		else { // встретилась не буква
			prev_is_letter = false;
			parsing_str.AddSymb(inp_str[i]);
		}
	}
	parsing_str.AddSymb(ItemSymb("end"));

	cout << endl << "Входная строка: ";
	parsing_str.PrintString();
	cout << endl << endl;
}



bool Stack_LRk_MethodAlg::SelectNextSymb()
{
	ItemSymb next_symb;
	const vector<ItemSymb> operators({ ItemSymb("+"), ItemSymb("-"), ItemSymb("*"), ItemSymb("/"), ItemSymb("("), ItemSymb(")"),ItemSymb("end") });
	bool operator_found = false;

//	if (parsing_item.Length() != 0) {
//		parsing_item.DeleteSymb(0, parsing_item.Length());
//	}

	if (point_of_entry < parsing_str.Length()) {

		while (!operator_found) {
			next_symb = parsing_str[point_of_entry];
			parsing_item.AddSymb(next_symb, parsing_item.Length());
			point_of_entry++;

			if (find(operators.begin(), operators.end(), next_symb) != operators.end()) {
				// если символ - оператор
				operator_found = true;
			}
		}
		
		cout << "\nРассматриваемая конструкция:\n";
		parsing_item.PrintString();

		if ((parsing_item.Length() == 1) && (parsing_item[0] != ItemSymb("("))) {
			cout << "\nНет идентификатора для свертки тройки\n";
			return false;
		}

		

	}
	return true;
}

int Stack_LRk_MethodAlg::FindLeftNum()
{
	ItemSymb stack_s;
	stack_s = stack_str[stack_str.Length() - 1];
	unsigned result;

	if (stack_s == ItemSymb("beg")) {
		result = 0;
	}
	else if((stack_s == ItemSymb("+"))
		|| (stack_s == ItemSymb("-"))) {
		result = 1;
	}
	else if ((stack_s == ItemSymb("*"))
		|| (stack_s == ItemSymb("/"))) {
		result = 2;
	}
	else if (stack_s == ItemSymb("(")) {
		result = 3;
	}
	else if (stack_s == ItemSymb(")")) {
		result = 4;
	}

	return result;
}

int Stack_LRk_MethodAlg::FindRightNum()
{
	ItemSymb parse_s;
	parse_s = parsing_item[parsing_item.Length() - 1];
	unsigned result;

	if ((parse_s == ItemSymb("+"))
		|| (parse_s == ItemSymb("-"))) {
		result = 0;
	}
	else if ((parse_s == ItemSymb("*"))
		|| (parse_s == ItemSymb("/"))) {
		result = 1;
	}
	else if (parse_s == ItemSymb("(")) {
		result = 2;
	}
	else if (parse_s == ItemSymb(")")) {
		result = 3;
	}
	else if (parse_s == ItemSymb("end")) {
		result = 4;
	}
	else {
		result = -1;
	}
	return result;
}

void Stack_LRk_MethodAlg::DoCarry()
{
	for (int i = 0; i < parsing_item.Length(); i++) {
		stack_str.AddSymb(parsing_item[i]);
	}

	parsing_item.DeleteSymb(0, parsing_item.Length());

	cout << "\nВыполнен перенос, стек после переноса: \n";
	stack_str.PrintString();
	cout << "\n";
}

bool Stack_LRk_MethodAlg::DoConvolution(bool full)
{
	static int trio_num = 0;
	
	int stack_len = stack_str.Length();

	cout << "\nДо свёртки скобок:\n Стек: ";
	stack_str.PrintString();
	cout << "\n Рассматриваемый элемент: ";
	parsing_item.PrintString();

	if ((parsing_item.Length() == 1) && (parsing_item[0] == ItemSymb(")"))) {

		cout << "\nТройку сформировать нельзя\n";
		return false;
	}
	else {
		if (full) {

			ItemString trio({ ItemSymb("R" + to_string(trio_num)), ItemSymb("=") });
			trio.AddSymb(stack_str[stack_len - 2]);
			trio.AddSymb(stack_str[stack_len - 1]);

			stack_str.DeleteSymb(stack_len - 2, 2);

			trio.AddSymb(parsing_item[0]);
			parsing_item.DeleteSymb(0, 1);
			parsing_item.AddSymb(trio[0], 0);

			cout << "\nПосле свёртки:\n Стек: ";
			stack_str.PrintString();
			cout << "\n Рассматриваемый элемент: ";
			parsing_item.PrintString();
			cout << "\n Тройка: ";
			trio.PrintString();
		}
		else {

			if (stack_str[stack_len - 1] == ItemSymb("(")) {

				trio_num--;
				ItemString trio({ ItemSymb("R" + to_string(trio_num)), ItemSymb("=") });
				trio.AddSymb(stack_str[stack_len - 1]);
				trio.AddSymb(parsing_item[0]);
				trio.AddSymb(parsing_item[1]);

				stack_str.DeleteSymb(stack_len - 1, 1);
				parsing_item.DeleteSymb(1, 1);

				bool symb_found = SelectNextSymb();
				if (!symb_found) {
					cout << "\nТройку сформировать нельзя\n";
				}
				else {
					cout << "\nПосле свёртки скобок:\n Стек: ";
					stack_str.PrintString();
					cout << "\n Рассматриваемый элемент: ";
					parsing_item.PrintString();
					cout << "\n Тройка: ";
					trio.PrintString();
				}
			}
			else {
				cout << "\nТройку сформировать нельзя\n";
			}
		}
		cout << "\n";

		trio_num++;
		return true;
	}

	


}
