#include"Algorithm_LLk_TtoD.h"
#include <cctype>
#include<conio.h>
#include<stdio.h>


void TtoD_LLk_MethodAlg::SetRulesOfAlg()
{
	//__________________________________________Создание нетерминалов

	ItemSymb c_string("<Строка>", false);
	ItemSymb c_calculation("<Выражение>", false);
	ItemSymb c_sum("<Слож>", false);
	ItemSymb c_term("<Терм>", false);
	ItemSymb c_mul("<Умнож>", false);
	ItemSymb c_factor("<Множ>", false);
	ItemSymb c_id("<Ид>");
	ItemSymb empty("e");
	ItemSymb end("end");

	vector<ItemSymb> buf_symb;					// переменная-шаблон для варианта раскрытия правила
	vector<ItemString> buf_str;					// переменная-шаблон для хранения всех вариантов 
												// раскрытия правила (правая часть правила)

	buf_symb = { c_calculation, end };			// <Выражение>end
	buf_str.push_back(ItemString(buf_symb));

	ItemRule rule(c_string, buf_str);			// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	buf_symb = { c_term, c_sum };				// <Терм><Слож>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_calculation, buf_str);		// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	buf_symb = { empty };						// e
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("+"), c_term, c_sum }; // +<Терм><Слож>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("-"), c_term, c_sum }; // +<Терм><Слож>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_sum, buf_str);				// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	buf_symb = { c_factor, c_mul };				// <Множ><Умнож>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_term, buf_str);				// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	buf_symb = { empty };						// e
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("*"), c_factor, c_mul }; // *<Множ><Умнож>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("/"), c_factor, c_mul }; // /<Множ><Умнож>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_mul, buf_str);				// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	buf_symb = { c_id };						// <Ид>
	buf_str.push_back(ItemString(buf_symb));

	buf_symb = { ItemSymb("("), c_calculation, ItemSymb(")") }; // *<Множ><Умнож>
	buf_str.push_back(ItemString(buf_symb));

	rule.SetRule(c_factor, buf_str);				// ПРАВИЛО
	buf_str.clear();							// чистка вариантов
	rules.push_back(rule);						// добавление нового правила
	//----------------------------------------------------

	cout << endl << "Правила для разбора сверху вниз для LLk сформированы:" << endl << endl;
	for (unsigned i = 0; i < rules.size(); i++) {
		rules[i].PrintRule();
		cout << endl;
	}
}


void TtoD_LLk_MethodAlg::SetParsingStr(ItemString inp_str)
{

	unsigned inp_size = inp_str.Length();
	int symb_code;
	bool prev_is_letter = false;

	for (unsigned i = 0; i < inp_size; i++) {

		symb_code = int(string(inp_str).c_str()[i]);

		if ((isalpha(symb_code))
			||  ((symb_code <0 && symb_code > -65) 
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

RuleNum TtoD_LLk_MethodAlg::FindRuleNum()
{
	int rules_size = rules.size();
	for (int i = 1; i < rules_size; i++) { /////? по идее можно смотреть с 1
		if (rules[i].GetLeft() == target_str[0]) { // нашли правило
			cout << endl << "Найдено подходящее правило: " << i + 1 << ", a" << endl;
			return RuleNum({ i, 0 });	// берём первое возможное раскрытие
		}
	}

	ItemSymb bracket(")");
	if ((parsing_str[0] == bracket)
		&& (target_str[0] == bracket)) {
		return RuleNum({ -4,-4 });	// скобки, которые необходимо убрать
	}
	///
	return RuleNum({ -5,-5 }); // возврат с ошибкой
	///
}

bool TtoD_LLk_MethodAlg::FindCorrectTerm(const RuleNum& rulenum)
{

	ItemSymb empty("e");
	bool may_be_deleted = false;
	unsigned del_num;
	// символ уберётся гарантированно
	//RuleNum cur_rulenum = rulenum

	unsigned size = rules[rulenum.fir_num].RightSize();
	for (int i = 0; i < size; i++){
		//rulenum.sec_num = rulenum.sec_num + i;
		cout << endl << "Хотим убрать: " << string(parsing_str[0]);
		
		cout << endl << "Пробуем:  " << string(rules[rulenum.fir_num][rulenum.sec_num + i][0]);

		if (rules[rulenum.fir_num][rulenum.sec_num + i][0] == empty) {
			may_be_deleted = true;
			del_num = rulenum.fir_num;
		}

		if (parsing_str[0] == rules[rulenum.fir_num][rulenum.sec_num + i][0]) {

			//WriteToLog({ rulenum.fir_num,rulenum.sec_num + i });

			TransformAccordingRule({ rulenum.fir_num , rulenum.sec_num + i });

			/// запись в лог с не убранными
			
			cout << endl;
			cout << "СОВПАДЕНИЕ ";
			cout << endl;
			cout << "Разбираемая строка: ";
			parsing_str.PrintString();
			cout << endl;
			cout << "Выбранная строка-цель: ";
			target_str.PrintString();
			cout << endl;

			cout << "Замена по правилу: " << rulenum.fir_num + 1 << char(rulenum.sec_num + i + 224) ;

			RemoveMatchingSymbs();
		//	recognized_str.AddSymb(parsing_str[0]);
		//	parsing_str.DeleteSymb(0, 1);
		//	target_str.DeleteSymb(0, 1);

			cout << endl;
			cout << "Разбираемая строка: ";
			parsing_str.PrintString();
			cout << endl;
			cout << "Выбранная строка-цель: ";
			target_str.PrintString();
			return true;
		}
	}
	// если не нашлось, но можно тереть - трём e
	
	if (may_be_deleted) {
		// запись в лог с е

		WriteToLog({ rulenum.fir_num, 5 });

		cout << endl;
		cout << "Совпадений не найдено, но можно удалить, удаляем первый символ из target_str: ";
		cout << endl << "Строка-цель до удаления: ";
		target_str.PrintString();
		target_str.DeleteSymb(0, 1);
		cout << endl << "Строка-цель после удаления: ";
		target_str.PrintString();
		return true;
	}
	/*
	cout << endl;
	cout << "Совпадений не найдено, удаляем первый символ из target_str: ";
	cout << endl << "Строка-цель до удаления: ";
	target_str.PrintString();
	target_str.DeleteSymb(0, 1);
	cout << endl << "Строка-цель после удаления: ";
	target_str.PrintString();
	*/
	cout << endl << "Ошибка подбора терминального символа" << endl;
	return false;
}

void TtoD_LLk_MethodAlg::RemoveMatchingSymbs()
{
	//recognized_str.AddSymb(parsing_str[0]);
	WriteToLog({ -3, 0 });
	parsing_str.DeleteSymb(0, 1);
	target_str.DeleteSymb(0, 1);

	cout << endl << "Разбираемая строка после удаления: ";
	parsing_str.PrintString();
	cout << endl << "Строка-цель после удаления: ";
	target_str.PrintString();

}

void TtoD_LLk_MethodAlg::TransformAccordingRule(const RuleNum& rulenum)
{
	ItemString substr = rules[rulenum.fir_num][rulenum.sec_num];

	cout << endl << "Меняем строки по правилу " << rulenum.fir_num + 1 << ", " << rulenum.sec_num + 1 << endl;
	cout << "Меняем " << string(target_str[0]) << " на ";
	substr.PrintString();
	cout << endl;

	WriteToLog(rulenum);

	target_str[0] = substr[0];
	if (substr.Length() > 1) {
		for (int i = 1; i < substr.Length(); i++) {
			target_str.AddSymb(substr[i], i);
		}
	}
	cout << "Полученная строка-цель: ";
	target_str.PrintString();
	cout << endl;
	
}

void TtoD_LLk_MethodAlg::WriteToLog(const RuleNum& cur_rule_num)
{
	LLk_TtoD_Line* buf_line;
	cout << "Осуществляется запись в лог" << endl;
	buf_line = new LLk_TtoD_Line();
	

	buf_line->SetLine(parsing_str, target_str, cur_rule_num);
	parsing_log.AddRecordLine(buf_line);
}

bool TtoD_LLk_MethodAlg::DoParse()
{	
	bool okey = true;
	bool term_was_found;
	RuleNum next_rule;
	RuleNum new_rule;
	ItemSymb end("end");
	//recognized_str.AddSymb(ItemSymb(""));

	target_str.SetString({ rules[1].GetLeft(), ItemSymb("end") }); // <выражение>end
	// запись в лог

	cout << "Строка-цель: ";
	target_str.PrintString();
	cout << endl;

	while (okey) {

		cout << endl;
		cout << "-------------------------------------------";
		cout << endl;
		cout << "Разбираемая строка: ";
		parsing_str.PrintString();
		cout << endl;
		cout << "Строка-цель: ";
		target_str.PrintString();
		cout << endl;

		next_rule = FindRuleNum(); // всегда выдаст первое раскрытие


		if (next_rule.fir_num < 0) {

			if (next_rule.fir_num == -5) {
				cout << "Ошибка, разбор дальше невозможен" << endl;
				okey = false;
				return true;
			}
			else if (next_rule.fir_num == -4) { // необходимо убрать скобки
				RemoveMatchingSymbs();
				if ((target_str.Length() == 1)
					&& (parsing_str.Length() == 1)
					&& (target_str[0] == end)) {
					cout << endl << "Разбор завершён";
					WriteToLog({ -10, 0 });
					okey = false;
				}
			}
		}
		else {

			if (rules[next_rule.fir_num][next_rule.sec_num][0].IsTerm()) {
				term_was_found = FindCorrectTerm(next_rule);

				if (!term_was_found) {
					// встречен некорректный символ, введённая строка - не выражение

					// запись в лог о некорректном символе
					cout << endl << "Некорректный символ, дальнейший разбор невозможен" << endl;

					WriteToLog();
					okey = false;
				}

				if ((target_str.Length() == 1)
					&& (parsing_str.Length() == 1)
					&& (target_str[0] == end)) {
					cout << endl << "Разбор завершён"<< endl;
					WriteToLog({ -10, 0 });
					okey = false;
				}
			}
			else {
				//WriteToLog(next_rule);
				TransformAccordingRule(next_rule); // корректируем target_str
			}
		}
		
		// Иначе - начинаем сначала
	}

	parsing_log.PrintLogLLk();

	return true;
}