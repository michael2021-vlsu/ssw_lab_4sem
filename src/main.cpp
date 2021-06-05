#include <iostream>
#include <chrono>

#include "tree_general.h"

#include "btree.h"
#include "Individual_BST.h"

void test_general_tree();
void test_bst();

int main() {
	std::cout << "==== BST Test ====" << std::endl;
	test_bst();
	std::cout << std::endl << "==== General Tree Test ====" << std::endl;
	test_general_tree();
}

void test_bst() {
	BinarySearchTree<float> bst{ 2, 4, 8, 16 };
	cout << "print: ";
	bst.print();
	cout << endl;
	bst.push(5);
	cout << "count: " << bst.count() << endl;
	bst.push(4);
	cout << "path: ";
	for (auto item : bst.path(4)) {
		cout << item << " ";
	}
	cout << endl;
	cout << "height: " << bst.height() << endl;
	bst.pop(4);
	cout << "print: ";
	bst.print();
	cout << endl;
	cout << "find 8: " << *bst.find(8) << endl;
	bst.push(9);
	bst.push(10);
	bst.push(11);
	cout << "unbalanced: ";
	bst.print();
	cout << endl;
	bst.balance();
	cout << "balanced: ";
	bst.print();
	cout << endl;

	BinarySearchTree<float> bst2;
	bst2.push(88);
	cout << "is bigger: " << (bst < bst2) << endl;
	cout << "is equal: " << (bst == bst2) << endl;

	cout << "Perfomance tests: " << endl;
	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.push(rand() / 3.0);
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "push x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.find(rand());
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "look for random key x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.pop(rand());
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "erase random key x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.path(rand());
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "path of random key x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.balance();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "balance x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 500; ++i) {
		bst.height();
		bst.count();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "count + height x500 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
}

void test_general_tree() {

	tree_general<int> test_int;
	tree_general<int> test_int_2;
	tree_g<int>* test_find_int;

	link_list<float>* list_float;
	tree_general<float> test_float;
	tree_general<float> test_float_2;
	tree_g<float>* test_find_float;

	link_list<char>* list_char;
	tree_general<char> test_char;
	tree_general<char> test_char_2;
	tree_g<char>* test_find_char;

	//Test for test_int
	std::cout << "Test int:" << std::endl;
	for (int i = 0; i < 100; ++i) {
		if (i < 5) {
			test_int.push((i - 1), i);
			test_int_2.push((i - 1), i);
		}
		else if (i < 10) {
			test_int.push(1, i);
			test_int_2.push(1, i);
		}
		else if (i < 20) {
			test_int.push(i % 10, i);
			test_int_2.push(i % 10, i);
		}
		else if (i == 20) {
			std::cout << "The firsh tree after 'push' is " << std::endl;
			test_int.print();
			std::cout << "Height of tree is " << test_int.height() << std::endl;
		}
		else if (i < 30) {
			test_int.pop(i % 10);
		}
		else if (i == 30) {
			std::cout << "The firsh tree after 'pop' is " << std::endl;
			test_int.print();
		}
		else if (i < 35) {
			std::cout << "Amount of tree is " << test_int.count() << std::endl;
			test_int.pop((i % 10) + 10);
		}
		else if (i == 35) {
			test_int.push(16, 16);
			test_int.print();
		}
		else if (i < 41) {
			Vector<int> V;
			V = test_int.path(&V, (i % 10 + 10));
			std::cout << "Path from peek to " << (i % 10 + 10) << " is " << V << std::endl;
		}
		else if (i == 41) {
			test_int.pop(16);
			test_find_int = test_int.find(i % 10 - 1);
			if (test_find_int->son == nullptr)
				test_find_int == nullptr;
			else
				test_find_int = test_find_int->son;
			std::cout << "Find element " << (i % 10 - 1) << ". It son is " << test_find_int->data << "." << std::endl;
		}
		else if (i == 42) {
			std::cout << "First tree is" << std::endl;
			test_int.print();

			std::cout << "Secont tree is" << std::endl;
			test_int_2.print();

			std::cout << "First < Second is ";
			if (test_int < test_int_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

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
		else if (i == 43) {
			link_list<int> list_int;
			test_int.conv_tree_to_linklist(&list_int);
			std::cout << "Converted tree to linked list. List is ";
			list_int.print(&list_int);
			std::cout << std::endl;
			delete_list(&list_int);
		}
		else if (i < 54) {
			test_int_2.pop(i % 10 + 10);
			if (i > 43 && i < 48)
				test_int.push(0, i % 4 + 11);
		}
		else if (i == 54) {
			tree_general<int> test_int_empty;

			std::cout << "First tree is" << std::endl;
			test_int.print();

			std::cout << "Secont tree is" << std::endl;
			test_int_2.print();

			std::cout << "Addition second tree with first tree." << std::endl;
			test_int_2.addition_tree(&test_int);

			std::cout << "First tree is" << std::endl;
			test_int.print();

			std::cout << "Secont tree is" << std::endl;
			test_int_2.print();

			std::cout << "Addition first tree with empty tree" << std::endl;
			test_int.addition_tree(&test_int_empty);

			std::cout << "First tree is" << std::endl;
			test_int.print();

			std::cout << "Addition first tree with first tree" << std::endl;
			test_int.addition_tree(&test_int);

			std::cout << "First tree is" << std::endl;
			test_int.print();
		}

	}

	std::cout << "\n\n\n\n\nTest float:" << std::endl;
	//Test for test_float
	for (int i = 0; i < 100; ++i) {
		if (i < 5) {
			test_float.push((i - 1), i);
			test_float_2.push((i - 1), i);
		}
		else if (i < 10) {
			test_float.push(1, i);
			test_float_2.push(1, i);
		}
		else if (i < 20) {
			test_float.push(i % 10, i);
			test_float_2.push(i % 10, i);
		}
		else if (i == 20) {
			std::cout << "The firsh tree after 'push' is " << std::endl;
			test_float.print();
			std::cout << "Height of tree is " << test_float.height() << std::endl;
		}
		else if (i < 30) {
			test_float.pop(i % 10);
		}
		else if (i == 30) {
			std::cout << "The firsh tree after 'pop' is " << std::endl;
			test_float.print();
		}
		else if (i < 35) {
			std::cout << "Amount of tree is " << test_float.count() << std::endl;
			test_float.pop((i % 10) + 10);
		}
		else if (i == 35) {
			test_float.push(16, 16);
			test_float.print();
		}
		else if (i < 41) {
			Vector<int> V;
			V = test_float.path(&V, (i % 10 + 10));
			std::cout << "Path from peek to " << (i % 10 + 10) << " is " << V << std::endl;
		}
		else if (i == 41) {
			test_float.pop(16);
			test_find_float = test_float.find(i % 10 - 1);
			if (test_find_float->son == nullptr)
				test_find_float == nullptr;
			else
				test_find_float = test_find_float->son;
			std::cout << "Find element " << (i % 10 - 1) << ". It son is " << test_find_float->data << "." << std::endl;
		}
		else if (i == 42) {
			std::cout << "First tree is" << std::endl;
			test_float.print();

			std::cout << "Secont tree is" << std::endl;
			test_float_2.print();

			std::cout << "First < Second is ";
			if (test_float < test_float_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First > Second is ";
			if (test_float > test_float_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First == Second is ";
			if (test_float == test_float_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First <= Second is ";
			if (test_float <= test_float_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;
		}
		else if (i == 43) {
			link_list<float> list_float;
			test_float.conv_tree_to_linklist(&list_float);
			std::cout << "Converted tree to linked list. List is ";
			list_float.print(&list_float);
			std::cout << std::endl;
			delete_list(&list_float);
		}
		else if (i < 54) {
			test_float_2.pop(i % 10 + 10);
			if (i > 43 && i < 48)
				test_float.push(0, i % 4 + 11);
		}
		else if (i == 54) {
			tree_general<float> test_float_empty;

			std::cout << "First tree is" << std::endl;
			test_float.print();

			std::cout << "Secont tree is" << std::endl;
			test_float_2.print();

			std::cout << "Addition second tree with first tree." << std::endl;
			test_float_2.addition_tree(&test_float);

			std::cout << "First tree is" << std::endl;
			test_float.print();

			std::cout << "Secont tree is" << std::endl;
			test_float_2.print();

			std::cout << "Addition first tree with empty tree" << std::endl;
			test_float.addition_tree(&test_float_empty);

			std::cout << "First tree is" << std::endl;
			test_float.print();

			std::cout << "Addition first tree with first tree" << std::endl;
			test_float.addition_tree(&test_float);

			std::cout << "First tree is" << std::endl;
			test_float.print();
		}

	}

	std::cout << "\n\n\n\n\nTest char:" << std::endl;
	//Test for test_char
	for (int j = 60, i = 60; i < 200; ++i) {
		if (i < (j + 5)) {
			test_char.push((i - 1), i);
			test_char_2.push((i - 1), i);
		}
		else if (i < (j + 10)) {
			test_char.push(i - 5, i);
			test_char_2.push(i - 5, i);
		}
		else if (i < (j + 20)) {
			test_char.push(i % 10 + j, i);
			test_char_2.push(i % 10 + j, i);
		}
		else if (i == (j + 20)) {
			std::cout << "The firsh tree after 'push' is " << std::endl;
			test_char.print();
			std::cout << "Height of tree is " << test_char.height() << std::endl;
		}
		else if (i < (j + 30)) {
			test_char.pop(i % 10 + j);
		}
		else if (i == (j + 30)) {
			std::cout << "The firsh tree after 'pop' is " << std::endl;
			test_char.print();
		}
		else if (i < (j + 35)) {
			std::cout << "Amount of tree is " << test_char.count() << std::endl;
			test_char.pop((i % 10) + j + 10);
		}
		else if (i == (j + 35)) {
			test_char.push(j, j);
			test_char.print();
		}
		else if (i < (j + 41)) {
			Vector<int> V;
			V = test_char.path(&V, j);
			std::cout << "Path from peek to " << j << " is " << V << std::endl;
		}
		else if (i == (j + 41)) {
			test_char.pop(j);
			test_find_char = test_char.find(i % 10 - 1 + j);
			if (test_find_char->son == nullptr) {
				test_find_char == nullptr;
				std::cout << "Find element " << (i % 10 - 1 + j) << ". It son no detected." << std::endl;
			}
			else {
				test_find_char = test_find_char->son;
				std::cout << "Find element " << (i % 10 - 1 + j) << ". It son is " << test_find_char->data << "." << std::endl;
			}
		}
		else if (i == (j + 42)) {
			std::cout << "First tree is" << std::endl;
			test_char.print();

			std::cout << "Secont tree is" << std::endl;
			test_char_2.print();

			std::cout << "First < Second is ";
			if (test_char < test_char_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First > Second is ";
			if (test_char > test_char_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First == Second is ";
			if (test_char == test_char_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;

			std::cout << "First <= Second is ";
			if (test_char <= test_char_2)
				std::cout << "true" << std::endl;
			else
				std::cout << "false" << std::endl;
		}
		else if (i == (j + 43)) {
			link_list<char> list_char;
			test_char.conv_tree_to_linklist(&list_char);
			std::cout << "Converted tree to linked list. List is ";
			list_char.print(&list_char);
			std::cout << std::endl;
			delete_list(&list_char);
		}
		else if (i < (j + 54)) {
			test_char_2.pop(i % 10 + 10 + j);
			if (i > (j + 43) && i < (j + 48))
				test_char.push(j, i % 4 + 11 + j);
		}
		else if (i == (j + 54)) {
			tree_general<char> test_char_empty;

			std::cout << "First tree is" << std::endl;
			test_char.print();

			std::cout << "Secont tree is" << std::endl;
			test_char_2.print();

			std::cout << "Addition second tree with first tree." << std::endl;
			test_char_2.addition_tree(&test_char);

			std::cout << "First tree is" << std::endl;
			test_char.print();

			std::cout << "Secont tree is" << std::endl;
			test_char_2.print();

			std::cout << "Addition first tree with empty tree" << std::endl;
			test_char.addition_tree(&test_char_empty);

			std::cout << "First tree is" << std::endl;
			test_char.print();

			std::cout << "Addition first tree with first tree" << std::endl;
			test_char.addition_tree(&test_char);

			std::cout << "First tree is" << std::endl;
			test_char.print();
		}

	}

}