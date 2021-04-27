
#include <iostream>
template <typename T>
class Vector
{
public:
private:
	T* arr; //Array of elements
	int size; //The number of elements
public:
	Vector();
	~Vector();

	void push_front(T); //Insert element to front
	void push_back(T); //Insert element to back
	T pop_front(); //Get and remove element from front
	T pop_back(); //Get and remove element from back
	T peek(); //Get the first element
	int count(); //Get the number of elements in vector 
	T at(int); //Get element with index

	//Individual tasks
	void deleteRange(int, int); //Delete given range
	void reverse(); //Reverse order of elements

	//Lexicographic overloading of operators
	Vector<T>& operator=(const Vector<T>&);
	bool operator<(const Vector<T>&);
	bool operator>(const Vector<T>&);
	bool operator==(const Vector<T>&);
	bool operator!=(const Vector<T>&);
	bool operator<=(const Vector<T>&);
	bool operator>=(const Vector<T>&);

	//Operator overloading of 'cout'
	friend std::ostream& operator<<(std::ostream& out, const Vector<T>& This_Vector)
	{
		for (int i = 0; i < This_Vector.size; i++)
		{
			out << "  " << This_Vector.arr[i];
			if (i == (This_Vector.size - 1))
				out << "\n";
		}
		return out;
	}


};
template <typename T>Vector<T>::Vector()
{
	size = 0;
	arr = 0;
}
template <typename T>Vector<T>::~Vector()
{
	delete[] arr;
}

template <typename T>void Vector<T>::push_front(T element)
{
	T* new_arr = new T[size + 1];
	new_arr[0] = element;
	for (int i = 1; i <= size; i++)
	{
		new_arr[i] = arr[i - 1];
	}
	delete[] arr;
	arr = new_arr;
	size++;
}
template <typename T>void Vector<T>::push_back(T element)
{
	T* new_arr = new T[size + 1];
	for (int i = 0; i < size; i++)
	{
		new_arr[i] = arr[i];
	}
	new_arr[size] = element;
	delete[] arr;
	arr = new_arr;
	size++;
}
template <typename T>T Vector<T>::pop_front()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	T* new_arr = new T[size - 1];
	for (int i = 0; i < size - 1; i++)
	{
		new_arr[i] = arr[i + 1];
	}
	T element;
	element = arr[0];
	delete[] arr;
	arr = new_arr;
	size--;
	return element;
}
template <typename T>T Vector<T>::pop_back()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	T* new_arr = new T[size - 1];
	for (int i = 0; i < size - 1; i++)
	{
		new_arr[i] = arr[i];
	}
	T element;
	element = arr[size - 1];
	delete[] arr;
	arr = new_arr;
	size--;
	return element;
}
template <typename T>T Vector<T>::peek()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	return arr[0];
}
template <typename T>int Vector<T>::count()
{
	return size;
}
template <typename T>T Vector<T>::at(int index)
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	return arr[index];
}

template <typename T>void Vector<T>::deleteRange(int start, int end)
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return;
	}
	else
	{
		if ((size < end || size < start) || (start > end) || (start < 0))
		{
			std::cout << "Error! Range is incorrect.\n";
			return;
		}
	}
	int local_size = size - (end - start + 1);
	T* new_arr = new T[local_size];
	for (int i = 0; i < start; i++)
	{
		new_arr[i] = arr[i];
	}
	for (int i = start, j = end + 1; i < local_size, j < size; i++, j++)
	{
		new_arr[i] = arr[j];
	}
	delete[] arr;
	arr = new_arr;
	size = local_size;
}
template <typename T>void Vector<T>::reverse()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return;
	}
	T* new_arr = new T[size];
	for (int i = 0, j = size - 1; i < size; i++, j--)
	{
		new_arr[i] = arr[j];
	}
	delete[] arr;
	arr = new_arr;

}

template <typename T>Vector<T>& Vector<T>::operator=(const Vector<T>& Second_Vector)
{
	delete[] arr;
	size = Second_Vector.size;
	if (size == 0)
		return *this;
	arr = new T[size];
	for (int i = 0; i < size; i++)
	{
		arr[i] = Second_Vector.arr[i];
	}
	return *this;
}
template <typename T>bool Vector<T>::operator<(const Vector<T>& Second_Vector)
{
	if (size < Second_Vector.size)
	{
		if (size == 0)
			return true;
		for (int i = 0; i < size; i++)
		{
			if (arr[i] < Second_Vector.arr[i])
				return true;
		}
	}
	else
	{
		for (int i = 0; i < Second_Vector.size; i++)
		{
			if (arr[i] < Second_Vector.arr[i])
				return true;
		}
	}
	return false;
}
template <typename T>bool Vector<T>::operator>(const Vector<T>& Second_Vector)
{
	return !(*this < Second_Vector) && (*this != Second_Vector);
}
template <typename T>bool Vector<T>::operator==(const Vector<T>& Second_Vector)
{
	if (size == Second_Vector.size)
	{
		for (int i = 0; i < size; i++)
		{
			if (arr[i] != Second_Vector.arr[i])
				return false;
		}
	}
	else
	{
		return false;
	}
	return true;
}
template <typename T>bool Vector<T>::operator!=(const Vector<T>& Second_Vector)
{
	return !(*this == Second_Vector);
}
template <typename T>bool Vector<T>::operator<=(const Vector<T>& Second_Vector)
{
	if (*this < Second_Vector || *this == Second_Vector)
		return true;
	else
		return false;
}
template <typename T>bool Vector<T>::operator>=(const Vector<T>& Second_Vector)
{
	if (*this > Second_Vector || *this == Second_Vector)
		return true;
	else
		return false;

}
int main()
{}