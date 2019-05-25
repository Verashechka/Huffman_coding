#pragma once

#include <iostream>
#include <string>
#include "Errors.h"
#include "Map.h"
#include "Huffman.h"
using namespace std;

struct Element
{
	Element* min_child;
	Element* max_child;
	Element* parent;
	string symbols;
	bool was_used;
	string code;
	size_t frequency;
	Element() { code = ""; };
	Element(string symb, size_t freq) : symbols(symb), frequency(freq)
	{
		min_child = nullptr;
		max_child = nullptr;
		parent = nullptr;
		was_used = false;
		code = "";
	}
	Element(Element* child1, Element* child2)
	{
		was_used = false;
		child1->was_used = true;
		child2->was_used = true;
		if (child1->frequency > child2->frequency)
		{
			min_child = child2;
			max_child = child1;
		}
		else
		{
			min_child = child1;
			max_child = child2;
		}
		frequency = max_child->frequency + min_child->frequency;
		symbols = min_child->symbols + max_child->symbols;
		child1->parent = this;
		child2->parent = this;
		parent = nullptr;
		code = "";
	}
	~Element() {};
};

string coding_text(string user_message, list<Element*> code_tree)
{// returns coded string
	string coded_message = "";
	for (size_t i = 0; i < user_message.size(); i++)
	{
		string symbol = " ";
		symbol[0] = user_message[i];
		list_element<Element*>* symb = code_tree.head;
		while (symb->data->symbols != symbol)
			symb = symb->next;
		coded_message += symb->data->code;
	}
	return coded_message;
}
string decoding(list<Element*> code_tree, string coded_message)
{//returns decoded string
	list_element<Element*>* symb = code_tree.head;
	int position = coded_message.find(symb->data->code), last_position = 0;
	string decoded_message = "";
	bool flag = true;
	while (flag)
	{
		symb = code_tree.head;
		position = coded_message.find(symb->data->code, last_position);
		while (position != last_position)
		{
			symb = symb->next;
			position = coded_message.find(symb->data->code, last_position);
		}
		last_position = last_position + symb->data->code.size();
		decoded_message += symb->data->symbols;
		if (last_position == coded_message.size())
			flag = false;
	}
	return decoded_message;
}
void coding(Element * node, string bit)
{//creates codes for symbols
	node->code = bit;
	if (node->max_child)
		coding(node->max_child, bit + "1");
	if (node->min_child)
		coding(node->min_child, bit + "0");
}
list<Element*> map_of_codes(string user_data)
{// returns list of symbols with codes
	list<char> symbols_list;
	list<Element*> code_tree;
	char symbol;
	size_t frequency;
	map_rb_tree<char, size_t> map;
	// creates map with symbols and frequencies
	for (size_t i = 0; i < user_data.size(); i++)
	{
		try
		{
			frequency = map.find_value(user_data[i]);
			map.edit_data(user_data[i], frequency + 1);
		}
		catch (...)
		{
			map.insert(user_data[i], 1);
		}
	}
	symbols_list = *map.get_keys();
	// creates list of Elements
	for (size_t i = 0; i < symbols_list.get_size(); i++)
	{
		string temp = "";
		temp += symbols_list.at(i + 1);
		Element* list_el = new Element(temp, map.find_value(temp[0]));
		code_tree.push_back(list_el);
	}
	cout << "\nmap:" << endl;
	for (size_t i = 1; i < code_tree.get_size() + 1; i++)
	{
		cout << code_tree.at(i)->symbols << " " << code_tree.at(i)->frequency << endl;
	}
	// huffman
	Element* root = code_tree.at(1);
	// while string of root doesnt consist of all symbols
	while (root->symbols.size() != code_tree.get_size())
	{
		Element* el_ef1 = nullptr, *el_ef2 = nullptr; 
		// freq will be max of possible
		size_t freq_min1 = (1 << (sizeof(size_t) * 8 - 1)), freq_min2 = (1 << (sizeof(size_t) * 8 - 1));
		for (size_t i = 0; i < code_tree.get_size(); i++)
		{
			Element* subroot = code_tree.at(i + 1);
			bool flag = true;
			while (flag)
			{
				if (!subroot->was_used)
				{
					if (subroot->frequency < freq_min1)
					{
						if (el_ef1)
						{
							el_ef1->was_used = false;
						}
						if (el_ef2)
						{
							el_ef2->was_used = false;
						}
						freq_min2 = freq_min1;
						el_ef2 = el_ef1;
						freq_min1 = subroot->frequency;
						el_ef1 = subroot;
						if (el_ef1)
						{
							el_ef1->was_used = true;
						}
						if (el_ef2)
						{
							el_ef2->was_used = true;
						}
					}
					else
					{
						if (subroot->frequency < freq_min2)
						{

							if (el_ef2)
							{
								el_ef2->was_used = false;
							}
							el_ef2 = subroot;
							freq_min2 = subroot->frequency;
							if (el_ef2)
							{
								el_ef2->was_used = true;
							}
						}
					}

				}

				subroot = subroot->parent;
				if (!subroot) flag = false;
			}
		}
		Element* new_parent = new Element(el_ef1, el_ef2);
		while (root->parent != nullptr)
			root = root->parent;
	}
	// creating codes of symbols
	coding(root, "");
	return code_tree;
}