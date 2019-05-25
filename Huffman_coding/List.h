#pragma once
template <class T>
class list_element
{
public:
	list_element* next;
	T data;
	list_element() {
		next = nullptr;
	}
	list_element(T user_data) {
		next = nullptr;
		
		data = user_data;
		
	}
	~list_element()
	{
		next = nullptr;
	}
};

template <class T>
class list
{
private:
	list_element<T>*tail;
	
public:
	int size_of_list;
	list_element<T>*head;
	list();
	list(T user_data);
	~list() { this->clear(); }
	void push_back(T user_data);
	T at(int index);
	int get_size();
	void clear();
	bool is_empty();
};

template <class T>
list<T>::list()
{
	head = nullptr;
	tail = nullptr;
	size_of_list = 0;
}

template <class T>
list<T>::list(T user_data)
{
	head = new list_element<T>*;
	head->data = user_data;
	tail = head;
	size_of_list = 0;
}

template <class T>
void list<T>::push_back(T user_data)// creates new element at the end of list
{
	list_element<T>* new_element = new list_element<T>;
	new_element->data = user_data;
	if (!head)
		head = new_element;
	else
		tail->next = new_element;
	size_of_list++;
	tail = new_element;
}

template <class T>
T list<T>::at(int index)//returns data from user-specified element
{
	if (!head)
		throw Head_is_null();
	if (index <= 0 || index > size_of_list)
		throw Wrong_index_of_element();
	list_element<T>* current = head;
	for (int i = 0; i < index - 1; ++i)
		current = current->next;
	return current->data;
}

template <class T>
int list<T>::get_size()//returns size of list
{
	return size_of_list;
}

template <class T>
void list<T>::clear()//clears all list
{
	if (head)
	{
		list_element<T>* temporary = head;
		while (temporary != nullptr)
		{
			temporary = head->next;
			/*delete head;*/
			head = temporary;
		}
		head = nullptr;
		tail = nullptr;
		size_of_list = 0;
	}

}

template <class T>
bool list<T>::is_empty()//returns true if list is empty else returns false
{
	return(!head);
}