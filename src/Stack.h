#ifndef STACK
#define STACK

#include "Vector.h"

template <typename T>
class Stack : Vector<T> {
public:
	void push(T item) {
		this->push_back(item);
	}

	T pop() {
		return this->pop_back();
	}

	T peek() {
		return Vector<T>::peek();
	}

	int count() {
		Vector<T>::count();
	}

	void deleteRange(int min, int max) {
		Vector<T>::deleteRange(min, max);
	}

	void reverse() {
		Vector<T>::reverse();
	}

	bool operator<(const Stack<T> &item) {
		return Vector<T>::operator<(item);
	}

	bool operator>(const Stack<T> &item) {
		return Vector<T>::operator>(item);
	}

	bool operator==(const Stack<T> &item) {
		return Vector<T>::operator==(item);
	}
	
	bool operator!=(const Stack<T> &item) {
		return Vector<T>::operator!=(item);
	}
	
	bool operator<=(const Stack<T> &item) {
		return Vector<T>::operator<=(item);
	}
	
	bool operator>=(const Stack<T> &item) {
		return Vector<T>::operator>=(item);
	}
};
#endif