#include "Alg_Elements.h"

//---------------------ItemRule---------------------

void ItemRule::SetRule(const ItemSymb & inp_l, const vector <ItemString> & inp_r) 
{
	left = inp_l;
	right = inp_r;
}

void ItemRule::PrintRule() 
{
	cout << string(left) << " ::= ";
	for (unsigned i = 0; i < right.size(); i++) {
		right[i].PrintString();
		if (right.size() - i != 1) cout << "|";

	}
}

//---------------------LtoR_Line---------------------

void LtoR_Line::SetLine(const string & inp_str, const RuleNum & inp_rnum) 
{
	cur_string = inp_str;
	rule_num = inp_rnum;
}

void LtoR_Line::PrintLine()
{
	string str_for_print;
	for (int i = 0; i < cur_string.size(); i++) {
		if (cur_string[i] != '\n') {
			str_for_print += cur_string[i];
		}
	}

	cout << "Строка: " << str_for_print;
	if (rule_num.fir_num > -1) {
		cout << " Правило: " << rule_num.fir_num + 1 << char(rule_num.sec_num + 224) << endl;
	}
	else
		if (rule_num.sec_num == -3) {
			cout << "       конец!" << endl;
		}
		else if (rule_num.sec_num == -4) {
			cout << "       разбор дальше невозможен!" << endl;
		}
		else {
			cout << "       тупик!" << endl;
		}
}

//---------------------ParseLog---------------------

void ParseLog::PrintLogltoR() 
{
	cout << endl << endl << "Лог:" << endl;
	for (int i = 0; i < records.size(); i++) {
		dynamic_cast<LtoR_Line *>(records[i])->PrintLine();
	}
}

void ParseLog::PrintLogTtoD()
{
	cout << endl << endl << "Лог:" << endl;
	for (int i = 0; i < records.size(); i++) {
		dynamic_cast<TtoD_Line *>(records[i])->PrintLine();
	}
}

void ParseLog::PrintLogLLk()
{
	cout << endl << endl << "Лог:" << endl;
	for (int i = 0; i < records.size(); i++) {
		dynamic_cast<LLk_TtoD_Line*>(records[i])->PrintLine();
	}
}





//---------------------TtoD_Line---------------------

void TtoD_Line::SetLine(const string & rec_str, const string & pars_str, const string & targ_str, unsigned type_of_l, TypeOfTtoDLine line_type, const RuleNum & inp_rnum)
{
	recognized = rec_str;
	cur_string = pars_str;
	target = targ_str;
	type = type_of_l;
	type_of_line = line_type;
	rule_num = inp_rnum;
}

void TtoD_Line::PrintLine()
{
	cout << endl << "Распознано: " << MakePrintable(recognized);
	cout << " Строка: " << MakePrintable(cur_string);
	cout << " Цель: " << MakePrintable(target);
//	cout << " Тип строки: " ;
//	switch (type_of_line)
//	{
//	case TypeOfTtoDLine::ROLLB_IMPOSS:
//		cout << "ROLLB_IMPOSS" << endl;
//		break;
//	case TypeOfTtoDLine::ROLLB_POSS:
//		cout << "ROLLB_POSS" << endl;
//		break;
//	case TypeOfTtoDLine::RULE_FIT:
//		cout << "RULE_FIT" << endl;
//		break;
//	case TypeOfTtoDLine::RULE_NOT_FIT:
//		cout << "RULE_NOT_FIT" << endl;
//		break;
//	case TypeOfTtoDLine::STATEMENT:
//		cout << "STATEMENT" << endl;
//		break;
//	}



	cout << " Тип: " << type;
	cout << "   ";
	switch (type)
	{
	case 1:
		cout << "?" << endl;
		break;
	case 2:
		cout << rule_num.fir_num+1 << char(rule_num.sec_num + 224) << " - ?" << endl;
		break;
	case 3:
		if (rule_num.sec_num == 0) {
			cout << rule_num.fir_num+1 << char(rule_num.sec_num + 224);
		}
		else {
			if (rule_num.fir_num == 3) {
				cout << rule_num.fir_num + 1 << "a..." << char(rule_num.sec_num + 224 - 1);
			}
			else {
				cout << rule_num.fir_num + 1 << "a..." << char(rule_num.sec_num + 224);
			}
			
		}
		cout << " - нет" << endl;
		break;
	case 4:
		cout << rule_num.fir_num+1 << char(rule_num.sec_num + 224) << " - да" << endl;
		break;
	default:
		cout << "Ошибка типа строки" << endl;
		break;
	}
	
}

string TtoD_Line::MakePrintable(string & str_with_seps)
{
	string str_for_print;
	for (int i = 0; i < str_with_seps.size(); i++) {
		if (str_with_seps[i] != '\n') {
			str_for_print += str_with_seps[i];
		}
	}
	return str_for_print;
}


//---------------------LLk_TtoD_Line---------------------


void LLk_TtoD_Line::SetLine(const string& pars_str, const string& st_str, const RuleNum& inp_rnum)
{
	cur_string = pars_str;
	stack_str = st_str;
	rule_num = inp_rnum;
}

void LLk_TtoD_Line::PrintLine()
{
	cout << endl << "Строка: " << cur_string;
	cout << " Стек: " << stack_str;


	if (rule_num.fir_num > 0) {
		if ((rule_num.fir_num == 1) || (rule_num.fir_num == 3)) {
			cout << " Правило: " << rule_num.fir_num + 1;
		}
		else {
			cout << " Правило: " << rule_num.fir_num + 1 << char(rule_num.sec_num + 224);
		}
	}
	else {
		switch (rule_num.fir_num)
		{
		case -1:
			cout << " Некорректный символ, разбор завершён";
			break;
		case -3:
			cout << "";
			break;
		case -10:
			cout << " Разбор завершён";
			break;
		}
	}


}
