﻿#pragma once

#ifndef CircularBuffer_D679BA40CEC24336AE47CA16782DFFBA
#define CircularBuffer_D679BA40CEC24336AE47CA16782DFFBA

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <memory>

template <typename T>
class CircularBuffer {
	std::unique_ptr<T[]> items;
	
	T *itemse, *pick, *place;

	unsigned int capacitance, stored_count;

public:
	CircularBuffer(unsigned int capacity):capacitance(capacity) {
		if (!capacity) throw std::invalid_argument("Capacity must be at least 1!");

		items = std::unique_ptr<T[]>(new T[capacity]);
		pick = place = items.get();

		stored_count = 0;
		itemse = place + capacity;
	}

	CircularBuffer(const std::initializer_list<T> &list): capacitance(list.size()) {
		if (!capacitance) throw std::invalid_argument("Capacity must be at least 1!");
		
		items = std::unique_ptr<T[]>(new T[capacitance]);
		pick = place = items.get();

		stored_count = capacitance;
		itemse = place + capacitance;

		{
			const T *citem = list.begin();
			for (T *cwpos = pick; cwpos != itemse; ++cwpos, ++citem)
				*cwpos = *citem;
		}
	}

	CircularBuffer(const CircularBuffer &arg): capacitance(arg.capacitance), stored_count(arg.stored_count) {
		items = std::unique_ptr<T[]>(new T[capacitance]);
		itemse = items.get() + capacitance;

		place = items.get() + (arg.place - arg.items.get());
		pick = items.get() + (arg.pick - arg.items.get());

		memcpy(items.get(), arg.items.get(), capacitance * sizeof(T));
	}

	CircularBuffer &operator=(const CircularBuffer &arg) {
		if (this != &arg) {
			capacitance = arg.capacitance;
			stored_count = arg.stored_count;

			items = std::unique_ptr<T[]>(new T[capacitance]);
			itemse = items.get() + capacitance;

			place = items.get() + (arg.place - arg.items.get());
			pick = items.get() + (arg.pick - arg.items.get());

			memcpy(items.get(), arg.items.get(), capacitance * sizeof(T));
		}
		
		return *this;
	}

	void emplace(const T &item) {
		if (place == pick) { // Common case
			if (stored_count) { //Ring is full, rewriting
				new (place) T(item); //Assigning an item to a memory address.
				if (++pick == itemse) {
					pick = items.get();
				}
				place = pick;
			} else { // Ring is empty, all is ok
				new (place) T(item); //Assigning an item to a memory address.
				++stored_count;
				if (++place == itemse) {
					place = items.get();
				}
			}
		} else {
			new (place) T(item); //Assigning an item to a memory address.
			++stored_count;
			if (++place == itemse) {
				place = items.get();
			}
		}
	}

	void emplace(unsigned int index, const T &item) {
		if (index >= stored_count) throw std::out_of_range("Index out of range!");

		unsigned int must_steps = stored_count - index;

		T *back = place, *front;

		//Getting empty place
		if (place == pick) {
			if (++pick == itemse) {
				pick = items.get();
			}
			--must_steps; //It is necessary to account for the change in index from the deletion of the oldest element
		} else
			++stored_count;

		if (++place == itemse) {
			place = items.get();
		}

		//Shifting items
		for (unsigned int cstep = 0; cstep != must_steps; ++cstep) {
			front = back;
			if (--back < items.get()) {
				back = itemse - 1;
			}
			*front = *back;
		}
		
		//Emplacing
		new (back) T(item);
	}

	void erase(unsigned int index) {
		if (index >= stored_count) throw std::out_of_range("Index out of range!");

		T *back, *front = pick;

		--stored_count;

		//Moving to needed item
		for (unsigned int cstep = 0; cstep != index; ++cstep) {
			if (++front == itemse) {
				front = items.get();
			}
		}

		if (--place < items.get()) {
			place = itemse - 1;
		}

		//Shifting items
		while (front != place) 	{
			back = front;
			if (++front == itemse) {
				front = items.get();
			}
			*back = *front;
		}
	}

	unsigned int capacity() const {
		return capacitance;
	}

	void insert(unsigned int index, T item) {
		emplace(index, item);
	}

	void push(T item) {
		emplace(item);
	}

	T pop() {
		if (stored_count) {
			T *item = pick;
			--stored_count;
			if (++pick == itemse) {
				pick = items.get();
			}
			return *item;
		} else
			throw std::underflow_error("Nothing to Pop!");
	}

	unsigned int count() const {
		return stored_count;
	}


	bool operator==(const CircularBuffer &arg) const {
		if (arg.stored_count != stored_count)
			return false;

		if (stored_count) {
			T *pk = pick, *em = place;
			T *pk2 = arg.pick;

			do {
				if (*pk != *pk2)
					return false;

				if (++pk == itemse)
					pk = items.get();
				if (++pk2 == arg.itemse)
					pk2 = arg.items.get();
			} while (pk != em);
		}

		return true;
	}

	bool operator>(const CircularBuffer &arg) const {
		if (stored_count) {
			T *pk = pick, *em = place;
			T *pk2 = arg.pick, *em2 = arg.place;

			while (true) {
				if (*pk > *pk2)
					return true;
				else if (*pk < *pk2)
					return false;

				if (++pk == itemse)
					pk = items.get();
				if (pk == em) return false;
				if (++pk2 == arg.itemse)
					pk2 = arg.items.get();
				if (pk2 == em2) return true;
			}
		}

		return false;
	}

	bool operator!=(const CircularBuffer &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const CircularBuffer &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const CircularBuffer &arg) const {
		return !(*this > arg);
	}

	bool operator<(const CircularBuffer &arg) const {
		return *this <= arg && *this != arg;
	}

	class iterator {
		friend class CircularBuffer;

		unsigned int index, err_index;
		T *ptr, *base, *end;
		iterator(T *current, T *base, T *end, unsigned int index, unsigned int err_index): ptr(current), base(base), end(end), index(index), err_index(err_index){ }
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T*;
		using reference = T&;

		unsigned int get_index() const {
			return index;
		}

		bool operator!=(iterator const &other) const {
			return index != other.index;
		}

		bool operator==(iterator const &other) const {
			return index == other.index;
		}

		T &operator*() const {
			if (index == err_index) throw std::logic_error("Can not dereference end iterator!");
			return *ptr;
		}

		T *operator->() const {
			if (index == err_index) throw std::logic_error("Can not dereference end iterator!");
			return ptr;
		}

		iterator &operator++() {
			if (index == err_index) throw std::out_of_range("Iterator out of range!");
			++index;
			if (++ptr == end) {
				ptr = base;
			}
			return *this;
		}

		iterator operator++(int) {
			if (index == err_index) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			++index;
			if (++ptr == end) {
				ptr = base;
			}
			return temp;
		}

		iterator &operator--() {
			if (index == 0) throw std::out_of_range("Iterator out of range!");
			--index;
			if (--ptr < base) {
				ptr = end - 1;
			}
			return *this;
		}

		iterator operator--(int) {
			if (index == 0) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			--index;
			if (--ptr < base) {
				ptr = end - 1;
			}
			return temp;
		}
	};

	iterator begin() const {
		return iterator(pick, items.get(), itemse, 0, stored_count);
	}

	iterator end() const {
		return iterator(place, items.get(), itemse, stored_count, stored_count);
	}

	CircularBuffer merge_with_sorted(const CircularBuffer &buffer) const {
		CircularBuffer<T> new_buffer(stored_count + buffer.stored_count);

		iterator il = begin(), ile = end(), io = buffer.begin(), ioe = buffer.end();

		while (il != ile && io != ioe) {
			if (*io > *il) {
				new_buffer.push(*il++);
			} else {
				new_buffer.push(*io++);
			}
		}

		for (; il != ile; ++il) {
			new_buffer.push(*il);
		}

		for (; io != ioe; ++io) {
			new_buffer.push(*io);
		}

		return new_buffer;
	}

	CircularBuffer get_min_max() {
		if (stored_count < 2) throw std::underflow_error("Count of elements must be at least 2!");

		CircularBuffer<T>::iterator it = begin(), min = it, max = it++;

		for (CircularBuffer<T>::iterator ite = end(); it != ite; ++it) {
			if (*it < *min)
				min = it;
			else if (*it > *max)
				max = it;
		}

		CircularBuffer<T> new_buffer{ *min, *max };

		if (max.get_index() > min.get_index()) {
			erase(max.get_index());
			erase(min.get_index());
		} else {
			erase(min.get_index());
			erase(max.get_index());
		}

		return new_buffer;
	}
};

template<typename T>
std::ostream &operator<<(std::ostream &out, const CircularBuffer<T> &buffer) {
	for (auto item : buffer) {
		out << item << std::endl;
	}
	return out;
}

#endif