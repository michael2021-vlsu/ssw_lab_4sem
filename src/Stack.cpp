#include <iostream>

#include "Stack.h"

using namespace std;

int main()
{
	Stack<int> test;
	test.push(10);
	test.push(25);

	cout << test.pop() << endl << test.pop() << endl;
	
	return 0;
}