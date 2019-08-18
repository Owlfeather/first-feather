#include "Basic_Elements.h"
#include "Algorithm_LtoR.h"
#include "Alg_Elements.h"

//************************Программа************************

int main() {
	setlocale(0, "russian");
	cout << "Запуск!" << endl;

	string input_str = "-45";
	
	//ItemString ParsingString;
	//ParsingString.SetOutputString(input_str);

	LtoR_MethodAlg alg;
	alg.SetRulesOfAlg();
	alg.SetParsingStr(input_str);
	alg.DoParse();


	system("pause");
	return 0;
}

//**********************Подпрограммы************************


