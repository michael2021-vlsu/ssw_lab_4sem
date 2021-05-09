#include <iostream>

#include "CircularBuffer.h"

using namespace std;


int main() {
	CircularBuffer<float> b3{ 2 , 68, 1, -135, 45, 3};
	
	cout << b3 << endl;

	return 0;
}