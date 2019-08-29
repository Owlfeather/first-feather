#include "Basic_Elements.h"
#include "Algorithm_LtoR.h"
#include "Algorithm_TtoD.h"
#include "Alg_Elements.h"

//************************Программа************************

int main() {
	setlocale(0, "russian");
	cout << "Запуск!" << endl;

	string input_str = "-45";
	
	/*
	LtoR_MethodAlg alg1;
	alg1.SetRulesOfAlg();
	alg1.SetParsingStr(input_str);
	alg1.DoParse();
	*/

	TtoD_MethodAlg alg2;
	alg2.SetRulesOfAlg();
	alg2.SetParsingStr(input_str);
	alg2.DoParse();



	system("pause");
	return 0;
}

//**********************Подпрограммы************************


