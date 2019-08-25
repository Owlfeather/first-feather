#include "Basic_Elements.h"
//---------------------ItemSymb---------------------
void ItemSymb::SetSymb(const string & inp_cont, bool inp_term)
{
	content = inp_cont;
	term = inp_term;
}

void ItemSymb::SetSymb(const char & inp_cont, bool inp_term)
{
	content = { inp_cont };
	term = inp_term;
}

//--------------------ItemString--------------------

ItemString::ItemString(const string & orig_str)  // первоначальная установка строки
{	
	ItemSymb buffer;
	for (unsigned i = 0; i < orig_str.length(); i++) {
		buffer.SetSymb(orig_str[i]);
		cur_string.push_back(buffer);

		cout << "Добавлено в вектор: " << string(cur_string[i]) << endl;

	}
	cout << "Длина вектора: " << cur_string.size() << endl;
}

void ItemString::DeleteSymb(const int & first, const int & quantity) 
{
	cur_string.erase(cur_string.begin() + first, cur_string.begin() + first + quantity);
}

ItemString::operator string() const
{
	string result;
	for (int i = 0; i < cur_string.size(); i++) {
		result += string(cur_string[i]);
	}
	return result;
}

void ItemString::PrintString()
{
	for (unsigned i = 0; i < cur_string.size(); i++) {
		cout << string(cur_string[i]);
	}
}