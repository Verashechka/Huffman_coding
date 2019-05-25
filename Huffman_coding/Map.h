#pragma once	
#include <Windows.h>
#include "List.h"

template<class T, class L>
class node
{
private:
	node<T, L>* uncle();
	node<T, L>* grandparent();
	node<T, L>* brother();
	bool is_left_child();
	node<T, L>* parent;
	node<T, L>* left_child;
	node<T, L>* right_child;
	bool color;
	char key;
	L data;
public:
	
	node()
	{
		parent = nullptr; left_child = nullptr; right_child = nullptr;
	};
	node(char user_key, L user_data)
	{
		key = user_key; data = user_data; parent = nullptr; left_child = nullptr; right_child = nullptr;
	}
	~node()
	{
		parent = nullptr; left_child = nullptr; right_child = nullptr;
	}
	template<class T, class L> friend class map_rb_tree;
};

template<class T, class L>
class map_rb_tree
{
	template<class T, class L>  friend class node;
private:
	node<T, L>* root;
	int number_of_nodes;

	node<T, L>* find(T key); // returns reference to node with requested key or null
	void clear_map(node<T, L>* current); // recursively clears map from leafs to root
	list<T>* get_key(list<T>*, node<T, L>* current);//recoursively creates list of keys
	list<L>* get_data(list<L>*, node<T, L>* current);//recoursively creates list of values
	node<T, L>* max_key(node<T, L>*);//returns node with max key
	node<T, L>* min_key(node<T, L>*);//returns node with min key of subtree
	void delete_node(node<T, L>* active_node);// deletes node
	void balance(node<T, L>* active_node);// balances map 
	node<T, L>* swap(node<T, L>* active_node);// swaps 2 elements (child and parent)
	void black_uncle(node<T, L>* active_node);// case of inserting node
	node<T, L>* left_rotate(node<T, L>* active_node);// rotates root of subtree left
	node<T, L>* right_rotate(node<T, L>* active_node);// rotates root of subtree right
	void double_black(node<T, L>*, bool is_fiction);// deletes double black node
	void red_brother_case(node<T, L>* db_node, bool is_fiction);// case of deleting double black node
	void black_nephews_case(node<T, L>* db_node, bool is_fiction);// case of deleting double black node
	void red_nephew_case(node<T, L>* db_node, bool is_fiction);// case of deleting double black node
public:


	map_rb_tree() : number_of_nodes(0), root(nullptr) {};// constructor
	~map_rb_tree() { this->clear(); }; // destructor

	bool is_empty(); // returns true if map is empty ++
	size_t get_size(); // returns size of map
	void clear(); // clears all map
	list<T>* get_keys(); // returns keys list
	list<L>* get_values(); // returns values list
	void remove(T key); // removes element by requested key
	void insert(T key, L data); // inserts element with requested key and data
	void output_map();
	L find_value(T key);
	void edit_data(T key, L new_data);
};

template<class T, class L>
list<T>* map_rb_tree<T, L>::get_keys()
{
	list<T> *head = new list<T>;
	node<T, L>* current = root;
	head = get_key(head, current);
	return head;
}

template<class T, class L>
list<T>* map_rb_tree<T, L>::get_key(list<T>* head, node<T, L>* current)
{

	if (current)
	{
		head = get_key(head, current->left_child);
		head = get_key(head, current->right_child);
		(*head).push_back(current->key);
	}
	return head;
}

template<class T, class L>
list<L>* map_rb_tree<T, L>::get_values()
{
	list<L> *head = new list<L>;
	node<T, L>* current = root;
	head = get_data(head, current);
	return head;
}

template<class T, class L>
list<L>* map_rb_tree<T, L>::get_data(list<L>* head, node<T, L>* current)
{
	if (current)
	{
		head = get_data(head, current->left_child);
		head = get_data(head, current->right_child);
		(*head).push_back(current->data);
	}
	return head;
}

template<class T, class L>
node<T, L>* map_rb_tree<T, L>::max_key(node<T, L>*top_node)
{
	node<T, L>* current = top_node;
	while (current->right_child)
		current = current->right_child;
	return current;
}

template<class T, class L>
node<T, L>* map_rb_tree<T, L>::min_key(node<T, L>*top_node)
{
	node<T, L>* current = top_node;
	while (current->left_child)
		current = current->left_child;
	return current;
}

template<class T, class L> // deletes node
inline void map_rb_tree<T, L>::delete_node(node<T, L>* active_node)
{
	if (!active_node->left_child && !active_node->right_child)
	{	// deleting node is a leaf
		// 3 cases: red leaf, root, black leaf
		if (active_node->color)
		{	// deleting node is red
			if (active_node->is_left_child())
			{	 // deleting node is left child
				active_node->parent->left_child = nullptr;
			}
			else
			{	// deleting node is right child
				active_node->parent->right_child = nullptr;
			}
			delete active_node;
		}
		else
		{	// deleting node is black
			if (active_node == this->root)
			{	// deleting node is root
				delete active_node;
				this->root = nullptr;
			}
			else
			{	// deleting node is black and not root
				node<T, L>* fiction_node = new node<T, L>;
				fiction_node->color = false;
				fiction_node->data = active_node->data;
				fiction_node->key = active_node->key;
				fiction_node->parent = active_node->parent;
				if (active_node->is_left_child())
					active_node->parent->left_child = fiction_node;
				else
					active_node->parent->right_child = fiction_node;
				delete active_node;
				// db for fiction
				double_black(fiction_node, true);
			}
		}
	}
	else
	{
		if (active_node->left_child&&active_node->right_child)
		{	// deleting node has 2 children
			node<T, L>* new_active_node = max_key(active_node->left_child);
			active_node->key = new_active_node->key;
			active_node->data = new_active_node->data;
			delete_node(new_active_node);
		}
		else
		{	// deleting node has 1 child
			node<T, L>* child;
			child = (active_node->left_child) ? active_node->left_child : active_node->right_child;
			if (active_node == this->root)
			{	// deleting node is root
				this->root = child;
				child->color = false;
				child->parent = nullptr;
			}
			else
			{
				if (active_node->is_left_child())
				{	// deleting node has left child
					active_node->parent->left_child = child;
				}
				else
				{	// deleting node has right child
					active_node->parent->right_child = child;
				}
				child->parent = active_node->parent;
				if (active_node->color)
				{	//deleting node is red
					delete active_node;
				}
				else
				{	//deleting node is black

					delete active_node;
					if (child->color)
						child->color = false;
					else
					{
						// db
						double_black(child, false);
					}
				}
			}
		}
	}
}

template<class T, class L> // calls function of deleting element
void map_rb_tree<T, L>::remove(T key)
{
	node<T, L>* current = this->find(key);
	if (current)
	{
		this->delete_node(current);
		this->number_of_nodes--;
	}
	else
	{
		throw Element_doesnt_exist();
	}
}

template<class T, class L> // returns reference to node with requested key or null
node<T, L>* map_rb_tree<T, L>::find(T key)
{
	node<T, L>*current = root;
	while (current != nullptr)
	{
		if (key != current->key)
		{
			if (key > current->key)
				current = current->right_child;
			else
				current = current->left_child;
		}
		else return current;
	}
	return current;
}

template<class T, class L> // retutns size of map
size_t map_rb_tree<T, L>::get_size()
{
	return this->number_of_nodes;
}

template<class T, class L> // insert element into map 
void map_rb_tree<T, L>::insert(T key, L data)
{
	if (this->find(key) == nullptr) // if key doesn't exist, then we insert element. Otherwise we throw an error
	{
		// create new node
		node<T, L>* new_node = new node<T, L>;
		new_node->key = key;
		new_node->data = data;
		new_node->left_child = nullptr;
		new_node->right_child = nullptr;
		if (root == nullptr) // map is empty, element becomes black root
		{
			new_node->parent = nullptr;
			root = new_node;
			new_node->color = false;
		}
		else // map isn't empty
		{
			new_node->color = true;
			node<T, L>* current = this->root;
			node<T, L>* temporary = current;
			while (current) // find place to insert
			{
				temporary = current;
				if (current->key > new_node->key)
					current = current->left_child;
				else
					current = current->right_child;
			}
			new_node->parent = temporary;
			if (temporary->key > new_node->key) // if key of temporary > than key of new_node, then new_node becomes left child
				temporary->left_child = new_node;
			//new_node becomes right child
			else
				temporary->right_child = new_node;
			if (temporary->color)//if parent of new node is red then balance map
			{
				this->balance(new_node);
			}
		}
		this->number_of_nodes++;
	}
	else
		throw Key_already_exists();
}

template<class T, class L> // is map empty
bool map_rb_tree<T, L>::is_empty()
{
	return(!root);
}

template<class T, class L> // returns grandparent of requested element 
node<T, L>* node<T, L>::grandparent()
{
	return this->parent->parent;
}

template<class T, class L> // returns uncle of requested element 
node<T, L>* node<T, L>::uncle()
{
	node<T, L>* grandparent;
	grandparent = this->grandparent();
	return(this->parent->is_left_child()) ? grandparent->right_child : grandparent->left_child;
}

template<class T, class L> // is requested node left child or not
bool node<T, L>::is_left_child()
{
	return (this->parent->left_child == this) ? true : false;
}

template<class T, class L> // balances map 
void map_rb_tree<T, L>::balance(node<T, L>* active_node)
{
	if (this->root == active_node)//if current node is root then recolour into black
		active_node->color = false;
	else
	{
		if (active_node->uncle()) // if uncle exists
		{
			if ((active_node->uncle())->color) // uncle is red
			{
				active_node->parent->color = false;
				active_node->uncle()->color = false;
				active_node->grandparent()->color = true;
				if (active_node->grandparent() == root)
				{
					balance(active_node->grandparent());

				}
				else
				{
					if (active_node->grandparent()->parent->color)
					{
						balance(active_node->grandparent());
					}
				}
			}
			else //uncle is black
				this->black_uncle(active_node);
		}
		else
			this->black_uncle(active_node);
	}
}

template<class T, class L> // swaps 2 elements (child and parent)
node<T, L>* map_rb_tree<T, L>::swap(node<T, L>* active_node)
{
	node<T, L>* temporary = active_node->parent;
	if (active_node->is_left_child())
	{
		active_node->parent = active_node->grandparent();
		active_node->parent->right_child = active_node;
		active_node->right_child = temporary;
		temporary->left_child = nullptr;
		temporary->parent = active_node;
	}
	else
	{
		active_node->parent = active_node->grandparent();
		active_node->parent->left_child = active_node;
		active_node->left_child = temporary;
		temporary->right_child = nullptr;
		temporary->parent = active_node;
	}
	return temporary;
}

template<class T, class L>
node<T, L>* node<T, L>::brother()
{
	if (this->is_left_child())
		return this->parent->right_child;
	else
		return this->parent->left_child;
}

template<class T, class L>	// rotates root of subtree left
node<T, L>* map_rb_tree<T, L>::left_rotate(node<T, L>* active_node)
{
	node<T, L>* new_top = active_node->right_child;
	// node is root => right child becomes new root
	if (active_node == this->root)
	{
		new_top->parent = nullptr;
		this->root = new_top;
	}
	// node wasn't root
	else
	{
		new_top->parent = active_node->parent;
		if (active_node->is_left_child())
			active_node->parent->left_child = new_top;
		else
			active_node->parent->right_child = new_top;
	}
	active_node->right_child = new_top->left_child;
	if (new_top->left_child)
	{
		new_top->left_child->parent = active_node;
	}
	new_top->left_child = active_node;
	active_node->parent = new_top;
	return new_top;
}

template<class T, class L> // rotates root of subtree right
node<T, L>* map_rb_tree<T, L>::right_rotate(node<T, L>* active_node)
{
	node<T, L>* new_top = active_node->left_child;
	if (active_node == root)
	{
		new_top->parent = nullptr;
		root = new_top;
	}
	else
	{
		new_top->parent = active_node->parent;
		if (active_node->is_left_child())
			active_node->parent->left_child = new_top;
		else
			active_node->parent->right_child = new_top;
	}
	active_node->left_child = new_top->right_child;
	if (new_top->right_child)
	{
		new_top->right_child->parent = active_node;
	}
	new_top->right_child = active_node;
	active_node->parent = new_top;
	return new_top;
}

template<class T, class L> // deletes double black node
inline void map_rb_tree<T, L>::double_black(node<T, L>* db_node, bool is_fiction)
{
	node<T, L>* brother = db_node->brother();
	if (brother->color)
	{	// brother is red
		red_brother_case(db_node, is_fiction);
	}
	else
	{
		if (brother->left_child&&brother->right_child)
		{	//has 2 children
			if (brother->left_child->color || brother->right_child->color)
			{// red child case
				red_nephew_case(db_node, is_fiction);
			}
			else
			{
				//black child case
				black_nephews_case(db_node, is_fiction);
			}
		}
		else
		{
			if (!brother->left_child && !brother->right_child)
			{// black children case
				black_nephews_case(db_node, is_fiction);
			}
			else
			{
				node<T, L>* child = (brother->left_child) ? brother->left_child : brother->right_child;
				if (child->color)
				{//red child case
					red_nephew_case(db_node, is_fiction);
				}
				else
				{
					//black child case
					black_nephews_case(db_node, is_fiction);
				}
			}

		}
	}
}

template<class T, class L> // case of deleting double black node
inline void map_rb_tree<T, L>::red_brother_case(node<T, L>* db_node, bool is_fiction)
{
	node<T, L>* parent = db_node->parent;

	db_node->parent->color = true;
	db_node->brother()->color = false;
	if (db_node->is_left_child())
		parent = left_rotate(parent);
	else
		parent = right_rotate(parent);
	double_black(db_node, is_fiction);
}

template<class T, class L>// case of deleting double black node
inline void map_rb_tree<T, L>::black_nephews_case(node<T, L>* db_node, bool is_fiction)
{
	node<T, L>* parent = db_node->parent;
	db_node->brother()->color = true;
	if (is_fiction)
	{
		if (db_node->is_left_child())
		{
			parent->left_child = nullptr;
		}
		else
		{
			parent->right_child = nullptr;
		}
		delete db_node;
	}
	if (parent->color)
		parent->color = false;
	else
	{
		if (parent != this->root)
			double_black(parent, false);
	}
}

template<class T, class L>// case of deleting double black node
inline void map_rb_tree<T, L>::red_nephew_case(node<T, L>* db_node, bool is_fiction)
{
	node<T, L>* brother = db_node->brother();
	node<T, L>* red_nephew;
	if (db_node->is_left_child())
	{ // db node is left
		if (brother->right_child)
		{// brother has right child
			if (brother->right_child->color)
			{//right child is red
				red_nephew = brother->right_child;
				brother->color = db_node->parent->color;
				db_node->parent->color = false;
				red_nephew->color = false;
			}
			else
			{//left child is red
				red_nephew = brother->left_child;
				red_nephew->color = db_node->parent->color;
				db_node->parent->color = false;
				red_nephew = swap(red_nephew);
			}

		}
		else
		{// brother has only left child and it's red
			red_nephew = brother->left_child;
			red_nephew->color = db_node->parent->color;
			db_node->parent->color = false;
			red_nephew = swap(red_nephew);
		}
		db_node->parent = left_rotate(db_node->parent);
	}
	else
	{// db node is right
		if (brother->left_child)
		{// brother has left child
			if (brother->left_child->color)
			{//left child is red
				red_nephew = brother->left_child;
				brother->color = db_node->parent->color;
				db_node->parent->color = false;
				red_nephew->color = false;
			}
			else
			{//red child is red
				red_nephew = brother->right_child;
				red_nephew->color = db_node->parent->color;
				db_node->parent->color = false;
				red_nephew = swap(red_nephew);
			}

		}
		else
		{// brother has only right child and it's red
			red_nephew = brother->right_child;
			red_nephew->color = db_node->parent->color;
			db_node->parent->color = false;
			red_nephew = swap(red_nephew);
		}
		db_node->parent = right_rotate(db_node->parent);
	}
	if (is_fiction)
	{
		if (db_node->is_left_child())
			db_node->parent->left_child = nullptr;
		else
			db_node->parent->right_child = nullptr;
		delete db_node;
	}
}


template<class T, class L> // case of inserting node
void map_rb_tree<T, L>::black_uncle(node<T, L>* active_node)
{
	node<T, L>* grandparent = active_node->grandparent();
	// if active node is left child of right child OR it's right child of left child then we swaps node and his parent
	if (((!(active_node->parent->is_left_child())) && (active_node->is_left_child())) || ((active_node->parent->is_left_child()) && (!(active_node->is_left_child()))))
		active_node = swap(active_node);
	grandparent->color = true;
	active_node->parent->color = false;
	// rotates grandparent right
	if (active_node->is_left_child())
	{
		this->right_rotate(grandparent);
	}
	// rotates grandparent left
	else
	{
		this->left_rotate(grandparent);
	}
}


template <class T, class L> // recursively clears map from leafs to root
void map_rb_tree <T, L>::clear_map(node<T, L>* current)
{
	if (current)
	{
		if (current->right_child)
			clear_map(current->right_child);
		if (current->left_child)
			clear_map(current->left_child);
		number_of_nodes--;
		delete current;
		current = nullptr;
	}
}

template<class T, class L> // calls function of clearing map
void map_rb_tree<T, L>::clear()
{
	if (this->root) // map is not empty, call function to clear it
	{
		this->clear_map(this->root);
		this->root = nullptr;
	}

}

template<class T, class L>
L map_rb_tree<T, L>::find_value(T key)
{
	node<T, L>* node = this->find(key);
	if (node)
		return node->data;
	else throw Element_doesnt_exist();
}

template<class T, class L>
inline void map_rb_tree<T, L>::edit_data(T key, L new_data)
{
	node<T, L>* node = this->find(key);
	if (node)
	{
		node->data = new_data;
	}
	else throw Element_doesnt_exist();
}
