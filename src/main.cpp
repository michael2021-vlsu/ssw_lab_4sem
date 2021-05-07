#include <iostream>
#include <queue>
#include <vector>
#include <stack>

#include <chrono>

#include "queue.h"
#include "Vector.h"
#include "Stack.h"

using namespace std;

int main() {
	/* Queue region */
	std::queue<float> q1;
	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		q1.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		q1.push(abs(rand()) / 1000.0F);
		q1.push(abs(rand()) / 1000.0F);
		q1.pop();
		q1.pop();
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::queue time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Queue<float> q2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		q2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		q2.push(abs(rand()) / 1000.0F);
		q2.push(abs(rand()) / 1000.0F);
		q2.pop();
		q2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The queue.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";
	
	/* Vector region */
	std::vector<float> v1;
	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		v1.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		v1.push_back(abs(rand()) / 1000.0F);
		v1.push_back(abs(rand()) / 1000.0F);
		v1.pop_back();
		v1.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::vector time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Vector<float> v2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		v2.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		v2.push_back(abs(rand()) / 1000.0F);
		v2.push_back(abs(rand()) / 1000.0F);
		v2.pop_back();
		v2.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The Vector.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";
	
	/* Stack region */
	std::stack<float> s1;
	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		s1.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		s1.push(abs(rand()) / 1000.0F);
		s1.push(abs(rand()) / 1000.0F);
		s1.pop();
		s1.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::stack time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Stack<float> s2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		s2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		s2.push(abs(rand()) / 1000.0F);
		s2.push(abs(rand()) / 1000.0F);
		s2.pop();
		s2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The Stack.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";
	
	return 0;
}