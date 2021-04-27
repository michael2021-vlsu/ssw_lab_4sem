
#include <iostream>

class Vector
{
public:
private:
	int* arr; //Array of elements
	int size; //The number of elements
public:
	Vector();
	~Vector();

	void push_front(int); //Insert element to front
	void push_back(int); //Insert element to back
	int pop_front(); //Get and remove element from front
	int pop_back(); //Get and remove element from back
	int peek(); //Get the first element
	int count(); //Get the number of elements in vector 
	int at(int); //Get element with index

	//Individual tasks
	void deleteRange(int, int); //Delete given range
	void reverse(); //Reverse order of elements

	//Lexicographic overloading of operators
	Vector& operator=(const Vector&);
	bool operator<(const Vector&);
	bool operator>(const Vector&);
	bool operator==(const Vector&);
	bool operator!=(const Vector&);
	bool operator<=(const Vector&);
	bool operator>=(const Vector&);

	//Operator overloading of 'cout'
	friend std::ostream& operator<<(std::ostream&, const Vector&);

};
Vector::Vector()
{
	size = 0;
	arr = 0;
}
Vector::~Vector()
{
	delete[] arr;
}

void Vector::push_front(int element)
{
	int* new_arr = new int[size + 1];
	new_arr[0] = element;
	for (int i = 1; i <= size; i++)
	{
		new_arr[i] = arr[i - 1];
	}
	delete[] arr;
	arr = new_arr;
	size++;
}
void Vector::push_back(int element)
{
	int* new_arr = new int[size + 1];
	for (int i = 0; i < size; i++)
	{
		new_arr[i] = arr[i];
	}
	new_arr[size] = element;
	delete[] arr;
	arr = new_arr;
	size++;
}
int Vector::pop_front()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	int* new_arr = new int[size - 1];
	for (int i = 0; i < size - 1; i++)
	{
		new_arr[i] = arr[i + 1];
	}
	int element;
	element = arr[0];
	delete[] arr;
	arr = new_arr;
	size--;
	return element;
}
int Vector::pop_back()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	int* new_arr = new int[size - 1];
	for (int i = 0; i < size - 1; i++)
	{
		new_arr[i] = arr[i];
	}
	int element;
	element = arr[size - 1];
	delete[] arr;
	arr = new_arr;
	size--;
	return element;
}
int Vector::peek()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	return arr[0];
}
int Vector::count()
{
	return size;
}
int Vector::at(int index)
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return 0;
	}
	return arr[index];
}

void Vector::deleteRange(int start, int end)
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
	int* new_arr = new int[local_size];
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
void Vector::reverse()
{
	if (size == 0)
	{
		std::cout << "Error! Vector is empty.\n";
		return;
	}
	int* new_arr = new int[size];
	for (int i = 0, j = size - 1; i < size; i++, j--)
	{
		new_arr[i] = arr[j];
	}
	delete[] arr;
	arr = new_arr;

}

Vector& Vector::operator=(const Vector& Second_Vector)
{
	delete[] arr;
	size = Second_Vector.size;
	if (size == 0)
		return *this;
	arr = new int[size];
	for (int i = 0; i < size; i++)
	{
		arr[i] = Second_Vector.arr[i];
	}
	return *this;
}
bool Vector::operator<(const Vector& Second_Vector)
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
bool Vector::operator>(const Vector& Second_Vector)
{
	return !(*this < Second_Vector) && (*this != Second_Vector);
}
bool Vector::operator==(const Vector& Second_Vector)
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
bool Vector::operator!=(const Vector& Second_Vector)
{
	return !(*this == Second_Vector);
}
bool Vector::operator<=(const Vector& Second_Vector)
{
	if (*this < Second_Vector || *this == Second_Vector)
		return true;
	else
		return false;
}
bool Vector::operator>=(const Vector& Second_Vector)
{
	if (*this > Second_Vector || *this == Second_Vector)
		return true;
	else
		return false;

}

std::ostream& operator<<(std::ostream& out, const Vector& This_Vector)
{
	for (int i = 0; i < This_Vector.size; i++)
	{
		out << "  " << This_Vector.arr[i];
		if (i == (This_Vector.size - 1))
			out << "\n";
	}
	return out;
}
int main()
{}