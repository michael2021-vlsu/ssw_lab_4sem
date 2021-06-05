#ifndef TREE__GENERAL
#define TREE__GENERAL
#include <iostream>
#include "Vector.h"

template <typename T>
struct tree_g {
	T data = 0;
	tree_g<T>* son = nullptr;
	tree_g<T>* brother = nullptr;
	tree_g() {};
	tree_g(T data) : data(data) {};
};

template <typename T>
struct link_list {
public:
	T data;
	link_list<T>* next_data;
	link_list() {
		this->data = 0;
		this->next_data = nullptr;
	}
	link_list(T data) {
		this->data = data;
		this->next_data = nullptr;
	}
	void print(link_list<T>* list) {
		if (list == nullptr)
			return;
		std::cout << "   " << list->data;
		if (list->next_data != 0)
			print(list->next_data);
		return;
	}

};
template <typename T>
void delete_list(link_list<T>* L) {
	if (L == nullptr)
		return;
	if (L->next_data != nullptr) {
		delete_list(L->next_data);
		delete L->next_data;
		return;
	}
}

template <typename T>
class tree_general {
private://Поля
	tree_g<T>* unit = nullptr;

private://Методы
	void delete_tree(tree_g<T>* unit) {
		if (unit == nullptr)
			return;
		delete_tree(unit->son);
		delete_tree(unit->brother);
		delete unit;
	}
	void push(tree_g<T>* unit, T node, T data) {
		//Поиск узла
		if (unit == nullptr) {
			this->unit = new tree_g<T>(data);
			return;
		}
		unit = find_element(unit, node);
		//Обращение к сыну этого узла
		if (unit->son == 0) {
			unit->son = new tree_g<T>(data);
			return;
		}
		unit = unit->son;
		//Поиск последнего брата этого сына
		while (unit->brother != nullptr)
			unit = unit->brother;
		//Добавление элемента в братья последнему брату
		unit->brother = new tree_g<T>(data);
		return;
		std::cout << unit->data << std::endl;
	}
	T pop(tree_g<T>* unit, T element) {
		T data_element = 0;
		tree_g<T>* local_element;
		tree_g<T>* before_local;
		tree_g<T>* local;
		//существует ли элемент
		local_element = find_element(unit, element);
		before_local = find_unit_before_son(unit, local_element);
		//Какой узел хранит на него ссылку: сын или брат
		if (before_local == 0) {
			before_local = find_unit_before_brother(unit, local_element);
			if (before_local == 0) {
				//Удаляемый элемен является корнем дерева?
				if (local_element == unit) {
					this->unit = unit->son;
					data_element = local_element->data;
					delete local_element;
					return data_element;
				}
			}
			else {
				//Есть ли сыновья у удаляемого элемента?
				if (local_element->son != 0) {//да
					//Есть ли сын у предыдущего элемента?
					if (before_local->son != 0) {//да
						local = before_local->son;
						//Находим последнего брата этого сына
						while (local->brother != 0) {
							local = local->brother;
						}
						local->brother = local_element->son;
					}
					else {//нет
						before_local->son = local_element->son;
					}
				}
				//Есть ли брат у удоляемого элемента
				if (local_element->brother != 0) {
					before_local->brother = local_element->brother;
				}
				else {
					before_local->brother = nullptr;
				}
				local_element->son = nullptr;
				local_element->brother = nullptr;
				data_element = local_element->data;
				delete local_element;
			}
		}
		else {
			//Есть ли брат у удаляемого элемента
			if (local_element->brother != 0) {//да
				//Есть ли сын у удаляемого элемента
				if (local_element->son != 0) {//да
					local = local_element->brother;
					//Есть ли сын у брата удаляемого элемента
					if (local->son != 0) {//да
						local = local->son;
						while (local->brother != 0)
							local = local->brother;
						local->brother = local_element->son;
					}
					else {//нет
						local->son = local_element->son;
					}
				}
				before_local->son = local_element->brother;
			}
			else {//нет
			 //Есть ли сын у удаляемого элемента
				if (local_element->son != 0) {
					before_local->son = local_element->son;
				}
				else {
					before_local->son = nullptr;
				}
			}
			//Удаление элемента
			local_element->son = nullptr;
			local_element->brother = nullptr;
			data_element = local_element->data;
			delete local_element;
		}
		return data_element;

	}
	tree_g<T>* find_unit_before_brother(tree_g<T>* unit, tree_g<T>* element) {
		tree_g<T>* local = nullptr;
		if (unit == nullptr)
			return 0;
		if (unit->brother == element)
			return unit;
		local = find_unit_before_brother(unit->son, element);
		if (local != 0)
			return local;
		local = find_unit_before_brother(unit->brother, element);
		if (local != 0)
			return local;
		return local;

	}
	tree_g<T>* find_unit_before_son(tree_g<T>* unit, tree_g<T>* element) {
		tree_g<T>* local = nullptr;
		if (unit == nullptr)
			return 0;
		if (unit->son == element)
			return unit;
		local = find_unit_before_son(unit->son, element);
		if (local != 0)
			return local;
		local = find_unit_before_son(unit->brother, element);
		if (local != 0)
			return local;
		return local;

	}
	tree_g<T>* find_element(tree_g<T>* unit, T element) {
		tree_g<T>* local = nullptr;
		if (unit == nullptr)
			return 0;
		if (unit->data == element)
			return unit;
		local = find_element(unit->son, element);
		if (local != 0) {
			return local;
		}
		local = find_element(unit->brother, element);
		if (local != 0) {
			return local;
		}
		if (local == 0 && unit == this->unit)
			throw std::underflow_error("Element is not found");
		return local;
	}
	int count(tree_g<T>* unit) {
		int sum = 0;
		if (unit == nullptr)
			return 0;
		sum = sum + count(unit->son);
		sum = sum + count(unit->brother);
		sum++;
		return sum;
	}
	int height(tree_g<T>* unit) {
		if (unit == 0)
			return 0;
		int height_max = 0;
		for (tree_g<T>* unit_local = unit->son; unit_local; unit_local = unit_local->brother) {
			int height_local = height(unit_local);
			if (height_local > height_max)
				height_max = height_local;
		}
		return ++height_max;
	}

	Vector<int>* path(Vector<int>* V, tree_g<T>* unit, T node) {
		if (unit->data == node) {
			V->push_back(path_element(this->unit, unit) - 1);
		}
		if (unit->son != 0)
			path(V, unit->son, node);
		if (unit->brother != 0)
			path(V, unit->brother, node);
		return V;
	}
	int path_element(tree_g<T>* unit, tree_g<T>* node) {
		if (unit == nullptr)
			return 0;
		int local = 0;
		if (unit == node)
			return 1;
		if (local == 0)
			local = path_element(unit->son, node);
		if (local != 0)
		{
			++local;
			return local;
		}
		if (local == 0)
			local = path_element(unit->brother, node);
		if (local != 0)
		{
			return local;
		}
		if (local == 0 && unit == this->unit)
			throw std::underflow_error("Element is not found");
		return local;
	}

	int path_element(tree_g<T>* unit, T node) {
		if (unit == nullptr)
			return 0;
		int local = 0;
		if (unit->data == node)
			return 1;
		if (local == 0)
			local = path_element(unit->son, node);
		if (local != 0)
		{
			++local;
			return local;
		}
		if (local == 0)
			local = path_element(unit->brother, node);
		if (local != 0)
		{
			return local;
		}
		if (local == 0 && unit == this->unit)
			throw std::underflow_error("Element is not found");
		return local;
	}
	void print(tree_g<T>* unit, int count_scape) {
		if (unit == 0)
			return;
		for (int i = 0; i < count_scape; i++)
			std::cout << " ";
		std::cout << unit->data << std::endl;
		if (unit->son != 0) {
			++count_scape;
			print(unit->son, count_scape);
			--count_scape;
		}
		if (unit->brother != 0) {
			print(unit->brother, count_scape);
		}

	}

	//Individual tasks
	link_list<T>* conv_tree_to_linklist(link_list<T>* unit_list, tree_g<T>* unit_tree) {
		if (unit_tree == nullptr)
			throw std::underflow_error("Tree is empty");
		unit_list->data = unit_tree->data;
		if (unit_tree->son != 0) {
			unit_list->next_data = new link_list<T>;
			unit_list = conv_tree_to_linklist(unit_list->next_data, unit_tree->son);
		}
		if (unit_tree->brother != 0) {
			unit_list->next_data = new link_list<T>;
			unit_list = conv_tree_to_linklist(unit_list->next_data, unit_tree->brother);
		}
		return unit_list;
	}
	void addition_tree(tree_g<T>* unit, tree_g<T>* sec_unit) {
		if (unit == nullptr)
			if (sec_unit == nullptr) {
				//unit - 0, sec_unit - 0
				return;
			}
			else {//unit - 0, sec_unit - no empty
				this->unit = new tree_g<T>;
				unit = this->unit;
				addition_tree_push(unit, sec_unit);
				return;
			}
		else
			if (sec_unit == nullptr) {
				//unit - no empty, sec_unit - 0
				return;
			}
			else {//unit - no empty, sec_unit - no empty
				if (unit == sec_unit) {
					tree_general<T> local;
					local.addition_tree(local.unit, sec_unit);
					if (unit->son != 0) {
						unit = unit->son;
						while (unit->brother != 0) {
							unit = unit->brother;
						}
						unit->brother = new tree_g<T>();
						addition_tree_push(unit->brother, local.unit);
					}
					else {
						unit->son = new tree_g<T>();
						addition_tree_push(unit->son, local.unit);
					}
					return;
				}
				else {
					if (unit->son != 0) {
						unit = unit->son;
						while (unit->brother != 0) {
							unit = unit->brother;
						}
						unit->brother = new tree_g<T>();
						addition_tree_push(unit->brother, sec_unit);
					}
					else {
						unit->son = new tree_g<T>();
						addition_tree_push(unit->son, sec_unit);
					}
					return;
				}
			}
	}
	void addition_tree_push(tree_g<T>* unit, tree_g<T>* sec_unit) {
		unit->data = sec_unit->data;
		if (sec_unit->son != 0) {
			unit->son = new tree_g<T>();
			addition_tree_push(unit->son, sec_unit->son);
		}
		if (sec_unit->brother != 0) {
			unit->brother = new tree_g<T>();
			addition_tree_push(unit->brother, sec_unit->brother);
		}
	}
public:
	~tree_general() {
		delete_tree(unit);
	}
	void push(T node, T data) {
		push(unit, node, data);
	}
	T pop(T element) {
		return pop(unit, element);
	}
	int count() {
		return count(unit);
	}
	tree_g<T>* find(T element) {
		return find_element(unit, element);
	}
	int height() {
		return height(unit);
	}
	void print() {
		print(unit, 0);
	}
	Vector<int>* path(Vector<int>* V, T node) {
		return path(V, unit, node);
	}

	//Individual tasks
	void conv_tree_to_linklist(link_list<T>* unit_list) {
		conv_tree_to_linklist(unit_list, unit);
	}
	void addition_tree(tree_general<T>* sec_tree) {
		addition_tree(unit, sec_tree->unit);
	}

	bool operator<(tree_general<T>& second_tree) {
		if (this == &second_tree)
			return false;

		return (count() < second_tree.count());
	}
	bool operator>(tree_general<T>& second_tree) {
		return (second_tree < *this);
	}
	bool operator==(tree_general<T>& second_tree) {
		if (this == &second_tree)
			return true;
		return (count() == second_tree.count());
	}
	bool operator!=(tree_general<T>& second_tree) {
		return !(*this == second_tree);
	}
	bool operator<=(tree_general<T>& second_tree) {
		return ((*this < second_tree) || (*this == second_tree));
	}
	bool operator>=(tree_general<T>& second_tree) {
		return ((*this > second_tree) || (*this == second_tree));
	}

};
#endif