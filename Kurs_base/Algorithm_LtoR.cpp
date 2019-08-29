#include "Algorithm_LtoR.h"

//---------------------LtoR_MethodAlg---------------------

void LtoR_MethodAlg::SetRulesOfAlg()
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

unsigned LtoR_MethodAlg::FindMaxQuantity() 
{
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

RuleNum LtoR_MethodAlg::FindRuleNum(const RuleNum & rulenum) 
{
	cout << endl << "Производится разбор строки: ";
	parsing_str.PrintString();

	int rules_number = rules.size();		// суммарное число правил
	int subrules_number;					// число подпунктов (вариантов расшифровки) одного правила

	for (int i = rulenum.fir_num; i < rules_number; i++) {
		subrules_number = rules[i].RightSize();
		for (int j = rulenum.sec_num; j < subrules_number; j++) {
			if (parsing_item == rules[i][j]) {

				cout << endl << endl << "СОВПАДЕНИЕ";
				cout << endl << "Рассматриваемая конструкция: ";
				parsing_item.PrintString();
				cout << endl;
				cout << "Совпадение: правило №" << i << ", пункт №" << j << " : ";
				rules[i][j].PrintString();
				cout << endl;
				return RuleNum{ i, j };
			}
		}
	}
	return RuleNum{ -1, -1 };
}

void LtoR_MethodAlg::TransformAccordingRule(const ItemSymb & substr, const unsigned start, const unsigned num_of_cleaned)
{
	parsing_str[start] = substr;
	if (num_of_cleaned > 1) {
		parsing_str.DeleteSymb(start + 1, num_of_cleaned - 1);
	}
}

void LtoR_MethodAlg::WriteToLog(const RuleNum cur_rule_num) 
{
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

ItemString LtoR_MethodAlg::RestoreStringFromLog(const string & log_str)
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

void LtoR_MethodAlg::Rollback() 
{
	cout << endl << "Выполняется откат назад" << endl;
	parsing_str = RestoreStringFromLog((*(parsing_log[parsing_log.Size() - 2])).GetCurString());
	RuleNum prev_rule = (*(parsing_log[parsing_log.Size() - 2])).GetRuleNum();

	parsing_item = rules[prev_rule.fir_num][prev_rule.sec_num];

	cout << endl << "Разбираемая строка: ";
	parsing_str.PrintString();
	cout << endl << "Будет выполняться поиск конструкции: ";
	parsing_item.PrintString();
	cout << endl;
}

RuleNum LtoR_MethodAlg::GetNextRule()
{
	RuleNum prev_rule = (*(parsing_log[parsing_log.Size() - 2])).GetRuleNum();
	if (prev_rule.sec_num == rules[prev_rule.fir_num].RightSize() - 1) prev_rule.fir_num++;
	else prev_rule.sec_num++;
	cout << endl << "Начнём поиск с правила: " << prev_rule.fir_num << ", " << prev_rule.sec_num << endl;
	return prev_rule;
}

bool LtoR_MethodAlg::DoParse() 
{
	int okey = 0;
	unsigned entry_point = 0;		// указывает на следующий для добавления в parsing_item символ
	parsing_item.SetString({ parsing_str[entry_point] });	// разбираемый участок строки
	RuleNum next_rule = { 0, 0 };

	unsigned quantity = FindMaxQuantity();

	while (okey == 0) {
		RuleNum rule_num = FindRuleNum(next_rule);

		if (rule_num.fir_num != -1) {		// Если правило нашлось

			cout << endl << "Замена: ";
			parsing_item.PrintString();
			cout << " на ";
			cout << string(rules[rule_num.fir_num].GetLeft());

			// запись в лог
			WriteToLog(rule_num);
			//

			// замена строк
			TransformAccordingRule(rules[rule_num.fir_num].GetLeft(), entry_point, parsing_item.Length());
			parsing_item.SetString({ parsing_str[0] });
			//

			if (rule_num.fir_num == 0) {
				// поменяли на аксиому
				if (parsing_str.Length() == 1) {
					// выявлено целое, конец разбора

					WriteToLog({ -3, -3 }); // КОД -3 - конец разбора
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
			if ((parsing_item.Length() == 1) && (parsing_item[0].IsTerm())) {

				//запись в лог
				WriteToLog({ -4, -4 });

				cout << endl << "Ошибка, неопознанный символ : ";
				parsing_item.PrintString();
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
			if ((parsing_item.Length() < quantity) && (entry_point != parsing_str.Length() - 1)) {
				parsing_item.AddSymb(parsing_str[entry_point + parsing_item.Length()]);
			}
			else {
				parsing_item.DeleteSymb(0, 1);
				entry_point++;
			}
			next_rule = { 0, 0 };
			// в начало
		}
	}
	parsing_log.PrintLogltoR();
	return true;
}

void LtoR_MethodAlg::SetParsingStr(ItemString inp_str)
{
	parsing_str = inp_str;
}
