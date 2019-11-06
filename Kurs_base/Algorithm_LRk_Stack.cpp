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


	relation_table.push_back({ CARRY, CARRY, CARRY, ERROR, EXIT, ID_ERROR, OP_ERROR });
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
	//RuleNum rulenum;
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
				cout << "\nОшибка, дальнейший разбор невозможен0\n";
				okey = false;
				parsing_log.PrintLogLRk();
				return true;
			}
		}
		else {
			conv_happened = false;
		}
		
		rulenum = { FindLeftNum(), FindRightNum() };

		///

		if ((rulenum.fir_num == -1)||(rulenum.sec_num == -1)) {
			cout << "\nОшибка, дальнейший разбор невозможен\n";
			okey = false;
			WriteToLog({ 0, 6 }, string(stack_str), string(parsing_item));
			parsing_log.PrintLogLRk();
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
				//cout << "\n\nОшибка, дальнейший разбор невозможен1";
				cout << "\nНет идентификатора для свертки тройки\n";
				WriteToLog({ 0, 5 }, string(stack_str), string(parsing_item));
				okey = false;
			}
			break;
		case CONV_BR:
			conv_done = DoConvolution(false);
			conv_happened = true;
			if (!conv_done) {
				cout << "\n\nОшибка, дальнейший разбор невозможен2";
				okey = false;
			}
			break;
		case ERROR:
			WriteToLog(rulenum, string(stack_str), string(parsing_item));
			cout << "\n\nОшибка, дальнейший разбор невозможен3";
			okey = false;
			break;
		case EXIT:
			WriteToLog(rulenum, string(stack_str), string(parsing_item));
			cout << "\n\nРазбор завершён успешно";
			okey = false;
			break;
		default:
			break;
		}



		//okey = false;

	}

	parsing_log.PrintLogLRk();
	return true;
}

void Stack_LRk_MethodAlg::SetParsingStr(ItemString inp_str)
{
	unsigned inp_size = inp_str.Length();
	int symb_code;
	bool prev_is_letter = false;
	unsigned num_of_id = 0;

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
				num_of_id++;
				parsing_str.AddSymb(ItemSymb("<Ид" + to_string(num_of_id) + ">"));
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
		/////////////////////////////////////////
		if ((parsing_item.Length() == 1) && ((parsing_item[0] == ItemSymb("+") )
											|| (parsing_item[0] == ItemSymb("-"))
											|| (parsing_item[0] == ItemSymb("*"))
											|| (parsing_item[0] == ItemSymb("/") )) ) {
			cout << "\nНет идентификатора для свертки тройки\n";
			WriteToLog({ 0, 5 }, string(stack_str), string(parsing_item));
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
	else result = -1;

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
	WriteToLog(rulenum, string(stack_str), string(parsing_item));

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
	static int trio_num = 1;
	
	int stack_len = stack_str.Length();
	RuleNum cur_rule;
	ItemString trio;

	string stack_for_line = string(stack_str);
	string parse_for_line = string(parsing_item);

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

			trio.AddSymb(ItemSymb("R" + to_string(trio_num)));
			trio.AddSymb(ItemSymb("="));
				
			//	= { ItemSymb("R" + to_string(trio_num)), ItemSymb("=") };
			trio.AddSymb(stack_str[stack_len - 2]);
			trio.AddSymb(stack_str[stack_len - 1]);

			ItemSymb oper = trio[trio.Length() - 1];
			
			if (oper == ItemSymb("+")) {
				//0,1
				cur_rule = { 0, 1 };
			}
			else if (oper == ItemSymb("-")) {
				//0,2
				cur_rule = { 0, 2 };
			}
			else if (oper == ItemSymb("*")) {
				//1,1
				cur_rule = { 1, 1 };
			}
			else if (oper == ItemSymb("/")) {
				//1,2
				cur_rule = { 1, 2 };
			}

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

			cout << "\n Результат: ";
			if (cur_rule.fir_num == 0) {
				cout << "Выражение";
			}
			else if (cur_rule.fir_num == 1) {
				cout << "Терм";
			}



		}
		else {

			if (stack_str[stack_len - 1] == ItemSymb("(")) {

				trio_num--;
				//ItemString trio({ ItemSymb("R" + to_string(trio_num)), ItemSymb("=") });
				trio.AddSymb(ItemSymb("R" + to_string(trio_num)));
				trio.AddSymb(ItemSymb("="));

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
					cout << "\n Результат: Множ";
					//2,1
				}
			}
			else {
				cout << "\nТройку сформировать нельзя\n";
			}
		}
		cout << "\n";

		WriteToLog(rulenum, stack_for_line, parse_for_line, string(trio), cur_rule);

		trio_num++;
		return true;
	}

	


}

void Stack_LRk_MethodAlg::WriteToLog(const RuleNum& rel_rule_num, const string& stack_s, 
									 const string& parse_s, const string& trio_str,
									 const RuleNum& res_rule_num)
{
	LRk_Stack_Line* buf_line;
	cout << "Осуществляется запись в лог" << endl;
	buf_line = new LRk_Stack_Line();


	buf_line->SetLine(parse_s, rel_rule_num, stack_s, relation_table[rel_rule_num.fir_num][rel_rule_num.sec_num], trio_str, res_rule_num);
	parsing_log.AddRecordLine(buf_line);
}
