#include <iostream>
#include "tree_general.h"

void test_general_tree();

int main()
{
	test_general_tree();
}

void test_general_tree()
{
	tree_general<int> test_int;
	tree_general<int> test_int_2;
	tree_g<int>* test_find_int;
	tree_general<float> test_float;
	tree_general<float> test_float_2;
	tree_g<float>* test_find_float;
	tree_general<char> test_char;
	tree_general<char> test_char_2;
	tree_g<char>* test_find_char;
	//Тест для test_int
	for (int i = 0; i < 43; ++i)
	{
		if (i < 5)
		{
			test_int.push((i - 1), i);
			test_int_2.push((i - 1), i);
		}
		else if (i <10)
		{
			test_int.push(1, i);
			test_int_2.push(1, i);
		}
		else if (i < 20)
		{
			test_int.push(i % 10, i);
			test_int_2.push(i % 10, i);
		}
		else if (i==20)
		{
			std::cout << "The firsh tree after 'push' is " << std::endl;
			test_int.print();
			std::cout << "Height of tree is " << test_int.height()<<std::endl;
		}
		else if (i < 30)
		{
			test_int.pop(i % 10);
		}
		else if (i == 30)
		{
			std::cout << "The firsh tree after 'pop' is " << std::endl;
			test_int.print();
		}
		else if (i < 35)
		{
			std::cout << "Amount of tree is " << test_int.count() << std::endl;
			test_int.pop((i % 10) + 10);
		}
		else if (i == 35)
		{
			test_int.print();
		}
		else if (i < 41)
		{
			std::cout << "Path from peek to "<< (i % 10 + 10)<< " is " << test_int.path((i%10)+10) << std::endl;

		}
		else if (i == 41)
		{
			test_find_int = test_int.find(i % 10 -1);
			test_find_int = test_find_int->son;
			std::cout << "Find element " << (i % 10 - 1) << ". It son is " << test_find_int->data<<"." << std::endl;
		}
		else if (i == 42) 
		{
				std::cout << "First tree is" << std::endl;
				test_int.print();

				std::cout << "Secont tree is" << std::endl;
				test_int_2.print();

				std::cout << "First < Second is ";
				if (test_int < test_int_2)
					std::cout << "true" << std::endl;
				else
					std::cout << "false"<<std::endl;

				std::cout << "First > Second is ";
				if (test_int > test_int_2)
					std::cout << "true" << std::endl;
				else
					std::cout << "false" << std::endl;

				std::cout << "First == Second is ";
				if (test_int == test_int_2)
					std::cout << "true" << std::endl;
				else
					std::cout << "false" << std::endl;

				std::cout << "First <= Second is ";
				if (test_int <= test_int_2)
					std::cout << "true" << std::endl;
				else
					std::cout << "false" << std::endl;
		}

	}
}