#include "Basic_Elements.h"
#include "Algorithm_LtoR.h"
#include "Algorithm_TtoD.h"
#include "Alg_Elements.h"
#include "Algorithm_LLk_TtoD.h"

//************************Программа************************

int main() {
	setlocale(0, "russian");
	cout << "Запуск!" << endl;

	string input_str = "(а-d)+n";
	
	/*
	LtoR_MethodAlg alg1;
	alg1.SetRulesOfAlg();
	alg1.SetParsingStr(input_str);
	alg1.DoParse();
	*/
	/*
	TtoD_MethodAlg alg2;
	alg2.SetRulesOfAlg();
	alg2.SetParsingStr(input_str);
	alg2.DoParse();
	*/

	///*
	TtoD_LLk_MethodAlg alg3;
	alg3.SetRulesOfAlg();
	alg3.SetParsingStr(input_str);
	alg3.DoParse();
	//*/

	system("pause");
	return 0;
}


