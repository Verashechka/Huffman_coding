#pragma once

#include <exception>
using namespace std;
class Key_already_exists : public exception
{
public:
	Key_already_exists() : exception("This key already exists!\n")
	{
	}
};
class Element_doesnt_exist :public exception
{
public:
	Element_doesnt_exist() :exception("Element with this key doesn't exist!\n")
	{
	}
};

class Head_is_null :public exception
{
public:
	Head_is_null() :exception("...\n")
	{
	}
};
class Wrong_index_of_element :public exception
{
public:
	Wrong_index_of_element() :exception("...\n")
	{
	}
};