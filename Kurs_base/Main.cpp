#include "Elements.h"

//************************���������************************

int main() {
	setlocale(0, "russian");
	cout << "������!" << endl;

	string input_str = "-45";
	
	ItemString ParsingString;
	ParsingString.SetOutputString(input_str);

	LtoR_MethodAlg alg;
	alg.SetRulesOfAlg();


	system("pause");
	return 0;
}

//**********************������������************************


