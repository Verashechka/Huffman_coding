#include"CppUnitTest.h"
#include "stdafx.h"
#include <iostream>
#include <string>
#include "E:\Вера\учёба\АиСД\4 семестр\Huffman_coding\Huffman_coding\Errors.h"
#include "E:\Вера\учёба\АиСД\4 семестр\Huffman_coding\Huffman_coding\Map.h"
#include "E:\Вера\учёба\АиСД\4 семестр\Huffman_coding\Huffman_coding\Huffman.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Huffman_test
{
	string user_message,coded_message = "", decoded_message="";
	list<Element*> code_map;


	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(is_size_of_code_map_right)
		{
			user_message = "hello world";
			code_map = map_of_codes(user_message);
			Assert::AreEqual(8, code_map.get_size());
		}
		TEST_METHOD(are_symbol_codes_right)
		{
			string code[] = { "1100","1101","1110","10","1111","000","01","001"};
			user_message = "hello world";
			code_map = map_of_codes(user_message);
			for (int i = 0; i < 8; i++)
			{
				Assert::AreEqual(code[i], code_map.at(i+1)->code);
			}
		}
		TEST_METHOD(is_data_of_code_map_right)
		{
			string code[] = { " ","e","d","l","r","w","o","h" };
			user_message = "hello world";
			code_map = map_of_codes(user_message);
			for (int i = 0; i < 8; i++)
			{
				Assert::AreEqual(code[i], code_map.at(i + 1)->symbols);
			}
		}
		TEST_METHOD(is_coded_message_right)
		{// a=00, b=01, c=1
			user_message = "abbccc";
			code_map = map_of_codes(user_message);
			coded_message = coding_text(user_message, code_map);
			string str = "000101111";
			Assert::AreEqual(str, coded_message);
		}
		TEST_METHOD(are_user_and_decoded_messages_equal)
		{
			user_message = "hello world";
			code_map = map_of_codes(user_message);
			coded_message = coding_text(user_message, code_map);
			decoded_message = decoding(code_map, coded_message);
			Assert::AreEqual(decoded_message, user_message);
		}
	};
}