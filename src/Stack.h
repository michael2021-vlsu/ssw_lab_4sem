#ifndef STACK
#define STACK

#include <exception>

/* BASED ON Stack.h */
template <typename T>
class Stack {
public:
	T *arr{ nullptr }; //Array of elements
	int size{ 0 }; //The number of elements
public:
	Stack() = default;
	Stack(const Stack<T> &);
	~Stack();

	void push(T); //Insert element to front
	T pop(); //Get and remove element from front
	T peek(); //Get the first element
	int count(); //Get the number of elements in stack 

	//Individual tasks
	void deleteRange(T, T); //Delete given range
	void reverse(); //Reverse order of elements

	//Lexicographic overloading of operators
	Stack<T> &operator=(const Stack<T> &);
	bool operator<(const Stack<T> &);
	bool operator>(const Stack<T> &);
	bool operator==(const Stack<T> &);
	bool operator!=(const Stack<T> &);
	bool operator<=(const Stack<T> &);
	bool operator>=(const Stack<T> &);
};

template <typename T>
Stack<T>::Stack(const Stack &stack) {
	size = stack.size;
	if (size == 0)
		arr = nullptr;
	else {
		arr = new T[size];
		for (int i = 0; i < size; i++) {
			arr[i] = stack.arr[i];
		}
	}
}

template <typename T>
Stack<T>::~Stack() {
	delete[] arr;
}

template <typename T>
void Stack<T>::push(T element) {
	T *new_arr = new T[size + 1];
	for (int i = 0; i < size; i++) {
		new_arr[i] = arr[i];
	}
	new_arr[size] = element;
	delete[] arr;
	arr = new_arr;
	size++;
}

template <typename T>
T Stack<T>::pop() {
	if (size == 0) {
		throw std::underflow_error("Error! Stack is empty.");
	}
	T *new_arr = new T[size - 1];
	for (int i = 0; i < size - 1; i++) {
		new_arr[i] = arr[i];
	}
	T element;
	element = arr[size - 1];
	delete[] arr;
	arr = new_arr;
	size--;
	return element;
}

template <typename T>
T Stack<T>::peek() {
	if (size == 0) {
		throw std::underflow_error("Error! Stack is empty.");
	}
	return arr[size - 1];
}

template <typename T>
int Stack<T>::count() {
	return size;
}

template <typename T>
void Stack<T>::deleteRange(T start, T end) {
	if (size == 0) {
		throw std::underflow_error("Error! Vector is empty.");
	} else {
		if (start > end) {
			throw std::out_of_range("Error! Range is incorrect.");
		}
	}

	int k = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] < start || arr[i]>end) {
			k++;
		}
	}
	T *new_arr = new T[k];
	k = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] < start || arr[i]>end) {
			new_arr[k] = arr[i];
			k++;
		}
	}
	delete[] arr;
	arr = new_arr;
	size = k;
}

template <typename T>
void Stack<T>::reverse() {
	if (size == 0) {
		throw std::underflow_error("Error! Stack is empty.");
	}
	T *new_arr = new T[size];
	for (int i = 0, j = size - 1; i < size; i++, j--) {
		new_arr[i] = arr[j];
	}
	delete[] arr;
	arr = new_arr;

}

template <typename T>
Stack<T> &Stack<T>::operator=(const Stack<T> &Second_Stack) {
	if (this == &Second_Stack)
		return *this;
	else
		if (Second_Stack.arr = 0) {
			delete[] arr;
			arr = nullptr;
			size = 0;
			return *this;
		} else {
			delete[] arr;
			size = Second_Stack.size;
			arr = new T[size];
			for (int i = 0; i < size; i++) {
				arr[i] = Second_Stack.arr[i];
			}
			return *this;
		}

}

template <typename T>
bool Stack<T>::operator<(const Stack<T> &Second_Stack) {
	if (this == &Second_Stack)
		return false;
	else
		if (size) {
			if (size < Second_Stack.size) {
				for (int i = 0; i < size; i++) {
					if (arr[i] < Second_Stack.arr[i])
						return true;
				}
			} else {
				if (Second_Stack.size == 0)
					return false;
				for (int i = 0; i < Second_Stack.size; i++) {
					if (arr[i] < Second_Stack.arr[i])
						return true;
				}
			}
		} else if (Second_Stack.size)
			return true;

		return false;
}

template <typename T>
bool Stack<T>::operator>(const Stack<T> &Second_Stack) {
	return !(*this < Second_Stack) && (*this != Second_Stack);
}

template <typename T>
bool Stack<T>::operator==(const Stack<T> &Second_Stack) {
	if (this == &Second_Stack)
		return true;
	else
		if (size == Second_Stack.size) {
			for (int i = 0; i < size; i++) {
				if (arr[i] != Second_Stack.arr[i])
					return false;
			}
		} else {
			return false;
		}
	return true;
}

template <typename T>
bool Stack<T>::operator!=(const Stack<T> &Second_Stack) {
	return !(*this == Second_Stack);
}

template <typename T>
bool Stack<T>::operator<=(const Stack<T> &Second_Stack) {
	if (*this < Second_Stack || *this == Second_Stack)
		return true;
	else
		return false;
}

template <typename T>
bool Stack<T>::operator>=(const Stack<T> &Second_Stack) {
	if (*this > Second_Stack || *this == Second_Stack)
		return true;
	else
		return false;

}

#endif