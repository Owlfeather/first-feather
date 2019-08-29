#include "Algorithm_TtoD.h"

//---------------------TtoD_MethodAlg---------------------

void TtoD_MethodAlg::SetRulesOfAlg()
{

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

	buf_symb = { c_number, c_unsigned_int };	// <цифра><цбз>
	buf_str.push_back(ItemString(buf_symb));	// записали вариант 1

	buf_symb.pop_back();						// <цифра>
	//buf_symb = { c_number };					// <цифра>
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

	cout << endl << "Правила для разбора сверху вниз сформированы:" << endl << endl;
	for (unsigned i = 0; i < rules.size(); i++) {
		rules[i].PrintRule();
		cout << endl;
	}
}

bool TtoD_MethodAlg::DoParse() 
{
	bool okey = true;
	RuleNum next_rule;
	RuleNum new_rule;
	recognized_str.AddSymb(ItemSymb(""));
	bool rollback_happened = false;
	
	target_str.SetString({ rules[0].GetLeft(), ItemSymb("end") }); // <целое>end
	WriteToLog(1);

	cout << "Строка-цель: ";
	target_str.PrintString();
	cout << endl;
	
	while (okey) {
		//if (rollback_happened) {
			//next_rule = new_rule;
		//	rollback_happened = false;
		//}
		//else {
			next_rule = FindRuleNum();
		//}
		TransformAccordingRule(next_rule);
		if (target_str[0].IsTerm()) {

			if (target_str[0] == parsing_str[0]) {
				//убираем по символу и проверяем, не дошли ли до конца
			}
			else {
				if (!FindCorrectTerm(next_rule)) {
					// нужен откат
					//new_rule = Rollback();
					//rollback_happened = true;
					//if (new_rule.fir_num == -1) { // если больше откат выполнить не удастся
						okey = false;
					//}
				}
			}
		}
		else WriteToLog(2, next_rule);
		//WriteToLog(2, next_rule);
		cout << endl << "Разбираемая строка: ";
		parsing_str.PrintString();
		cout << endl << "Строка-цель: ";
		target_str.PrintString();

	}

	parsing_log.PrintLogTtoD();
	return true;
}

void TtoD_MethodAlg::SetParsingStr(ItemString inp_str)
{
	parsing_str = inp_str;
	parsing_str.AddSymb(ItemSymb("end"));
	cout << "Добавлен символ конца строки" << endl;

}


RuleNum TtoD_MethodAlg::FindRuleNum() // используется для поиска первого подходящего правила
									  // для взятия просто следующего правила сделать проц GetNextRule
{
	int rules_number = rules.size();
	for (int i = 0; i < rules_number; i++) {
		if (rules[i].GetLeft() == target_str[0]) { // нашли правило для этого нетерминала
			cout << endl << "Найдено подходящее правило: " << i+1 << ", a" << endl;
			return RuleNum({ i, 0 });	// берём первое возможное раскрытие
		}
	}
	// эта функция корректна только тогда, когда в target_str[0] - нетерминал
}

bool TtoD_MethodAlg::FindCorrectTerm(const RuleNum & rulenum)
{
	unsigned size_of_rule = rules[rulenum.fir_num].RightSize();
	ItemSymb correct_term;

	for (int i = 0; i < size_of_rule; i++) {
		correct_term = rules[rulenum.fir_num][i][0];
		if (correct_term == parsing_str[0]) {
			cout << endl << "Правило " << rulenum.fir_num + 1 << ", " << i + 1 << " подошло" << endl;
			if (i > 0) {
				WriteToLog(3, { rulenum.fir_num , i - 1 });
			}
			//запись в лог о несовпадениях (если они есть) типа 3
			target_str.DeleteSymb(0, 1); // трём первый символ
			target_str.AddSymb(correct_term, 0); // вставляем в начало нужный
			WriteToLog(4, { rulenum.fir_num , i });
			//запись в лог типа 4
			RemoveMatchingSymbs();
			WriteToLog(1);
			// запись в лог типа 1
			return true;	// найти символ удалось
		}
		cout << endl << "Правило " << rulenum.fir_num + 1 << ", " << i + 1 << " не подошло" << endl;
	}
	WriteToLog(3, { rulenum.fir_num , int(size_of_rule) });	// не нашлось, запись в лог типа 4
	//если не удалось - нужен откат
	return false;
}

void TtoD_MethodAlg::RemoveMatchingSymbs()
{
	// parsing_str[0] == target_str[0] - факт
	recognized_str.AddSymb(parsing_str[0]);
	parsing_str.DeleteSymb(0, 1);
	target_str.DeleteSymb(0, 1);
}

void TtoD_MethodAlg::TransformAccordingRule(const RuleNum & rulenum)
{
	ItemString substr = rules[rulenum.fir_num][rulenum.sec_num];

	cout << endl << "Меняем строки по правилу " << rulenum.fir_num+1 << ", " << rulenum.sec_num+1 << endl;
	cout << "Меняем " << string(target_str[0]) << " на ";
	substr.PrintString();
	cout << endl;

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

RuleNum TtoD_MethodAlg::Rollback()
{
	cout << endl << "Выполняется откат назад" << endl;

	bool found = false;
	//unsigned log_size = parsing_log.Size();
	int i = parsing_log.Size() - 1;
	RuleNum rollback_info;
	unsigned required_ind;

	while ((!found) && (i != -1)) {
		rollback_info = parsing_log[i]->GetRuleNum();
		if ((rollback_info.fir_num = -1) && (rollback_info.sec_num < 1)) {
			// откат возможен
			found = true;
			required_ind = i;
		}
		i--;
	}

	if (found) {
		parsing_str = RestoreStringFromLog((*(parsing_log[required_ind-1])).GetCurString());

		recognized_str = RestoreStringFromLog(dynamic_cast<TtoD_Line *>(parsing_log[required_ind-1])->GetRecString());
		target_str = RestoreStringFromLog(dynamic_cast<TtoD_Line *>(parsing_log[required_ind-1])->GetTargString());

		cout << endl << "восстановленная строка распознанного: ";
		recognized_str.PrintString();
		cout << endl << "восстановленная строка разбора: ";
		parsing_str.PrintString();
		cout << endl << "восстановленная строка цели: ";
		target_str.PrintString();



		RuleNum next_rule = (*(parsing_log[required_ind + 1])).GetRuleNum();
		next_rule.sec_num += rollback_info.sec_num + 1;
			return next_rule;
	}
	return RuleNum({-1, -1});
	//parsing_str = RestoreStringFromLog((*(parsing_log)))
}

void TtoD_MethodAlg::WriteToLog(const unsigned & type, const RuleNum & cur_rule_num)
{
	TtoD_Line * buf_line;
	cout << "Осуществляется запись в лог" << endl;
	buf_line = new TtoD_Line();

	string rec_str = MakeStrForLog(recognized_str);
	string pars_str = MakeStrForLog(parsing_str);
	string targ_str = MakeStrForLog(target_str);

	buf_line->SetLine(rec_str, pars_str, targ_str, type, cur_rule_num);
	parsing_log.AddRecordLine(buf_line);



}

string TtoD_MethodAlg::MakeStrForLog(ItemString & orig_str)
{
	string str_with_separators;
	for (int i = 0; i < orig_str.Length(); i++) {
		str_with_separators += string(orig_str[i]) + '\n';
	}
	return str_with_separators;
}

ItemString TtoD_MethodAlg::RestoreStringFromLog(const string & log_str)
{

	ItemString restored_str;
	char * arr_of_char = new char[log_str.size() + 1];
	copy(log_str.begin(), log_str.end(), arr_of_char);
	arr_of_char[log_str.size()] = '\0';
	char * context;
	bool added = false;

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


