#include <iostream>
#include <string>
#include "Errors.h"
#include "Map.h"
#include "Huffman.h"
using namespace std;

int main()
{
	list<Element*> code_map;
	string user_message;
	cout << "enter the text:\n\n";
	getline(cin, user_message);
	cout << "size of message: " << user_message.size() * 8 << endl;
	// creating map
	code_map = map_of_codes(user_message);
	cout << "\ncodes of symbols:" << endl;
	for (size_t i = 0; i < code_map.get_size(); i++)
		cout << code_map.at(i + 1)->symbols << ' ' << code_map.at(i + 1)->code << endl;
	// coding_text
	string coded_message = coding_text(user_message, code_map);
	cout <<"\ncoded message:\n"<< coded_message << endl;
	cout << "size of coded message: " << coded_message.size() << endl;
	// decoding
	string decoded_message = decoding(code_map, coded_message);
	cout << "\ndecoded message:\n" << decoded_message << endl<<endl;
	double first = (user_message.size() * 8);
	double second = coded_message.size();
	double compression = first / second;
	cout << "data compression: " << compression << endl << endl;
	system("pause");
	return 0;
}