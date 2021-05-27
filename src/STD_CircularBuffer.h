#pragma once

#include <queue>
#include <vector>

using namespace std;

/* std::circular_buffer simple emulator */
template <typename T>
class STD_CircularBuffer : public queue<T> {
	using queue<T>::emplace;
	unsigned int capacity;
public:
	STD_CircularBuffer(unsigned int capacity): queue<T>(), capacity(capacity) {
	}

	void push(const T &item) {
		if (this->size() == capacity) {
			this->pop();
		}
		queue<T>::push(item);
	}

	//Slow
	void erase(unsigned int index) {
		std::queue<T> temp;
		for (unsigned int i = 0, sz = this->size(); i != sz; ++i) {
			if (i != index) {
				temp.push(this->front());
				this->pop();
			} else
				this->pop();
		}
		while (temp.size()) {
			queue<T>::push(temp.front());
			temp.pop();
		}
	}

	//Slow
	void insert(unsigned int index, const T &item) {
		std::queue<T> temp;
		if (this->size()) {
			unsigned int i = 0, sz = this->size();
			if (sz == capacity) {
				this->pop(); --sz;
			}

			if (index == sz) {
				for (; i != sz; ++i) {
					temp.push(this->front());
					this->pop();
				}
				temp.push(item);
			} else {
				for (; i != sz; ++i) {
					if (index == i)
						temp.push(item);

					temp.push(this->front());
					this->pop();
				}
			}

			do {
				queue<T>::push(temp.front());
				temp.pop();
			} while (temp.size());
		}
	}

	vector<T> to_vector() {
		std::vector<T> temp;
		for (unsigned int i = 0, sz = this->size(); i != sz; ++i) {
			temp.push_back(this->front());
			this->pop();
		}
		for (T item : temp) {
			queue<T>::push(item);
		}
		return temp;
	}

	void from_vector(vector<T> &inp) {
		while (this->size()) {
			this->pop();
		}
		for (T item : inp) {
			queue<T>::push(item);
		}
	}
};