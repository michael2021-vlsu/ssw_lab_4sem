﻿#pragma once

#ifndef Queue_D2AFDC6572624C1F8B98572614DEF89D
#define Queue_D2AFDC6572624C1F8B98572614DEF89D

#include <cstring>
#include <stdexcept>

template <typename T>
class Queue {
	T *items, *itemse, *pick, *place;
	unsigned int capacitance, stored_count;

public:
	Queue(unsigned int capacity = 8):capacitance(capacity) {
		if (!capacity) throw std::invalid_argument("Capacity must be at least 1!");

		pick = place = items = reinterpret_cast<T *>(malloc(capacity * sizeof(T)));
		if (!items) throw std::runtime_error("You don't have enough memory!");

		stored_count = 0;
		itemse = items + capacity;
	}
	Queue(const Queue &) = delete;
	Queue operator=(const Queue &) = delete;

	~Queue() {
		free(items);
	}

	void emplace(const T &item) {
		if (place != pick) { // Common case
			new (place) T(item); //Assigning an item to a memory address.
			++stored_count;
			if (++place == itemse) {
				place = items;
			}
		} else {
			if (!stored_count) { // Ring is empty, all is ok
				new (place) T(item); //Assigning an item to a memory address.
				++stored_count;
				if (++place == itemse) {
					place = items;
				}
			} else { //Ring is full, expanding needed
				unsigned int capacity_new = capacitance << 1;
				if (capacitance > capacity_new) throw std::out_of_range("You have reached the UNSIGNED INT limit!");

				T *new_pointer = reinterpret_cast<T *>(realloc(items, capacity_new * sizeof(T)));
				if (!new_pointer) throw std::runtime_error("You don't have enough memory!");

				unsigned int epic_offset = place - items;
				items = new_pointer;

				if (epic_offset) {
					pick = new_pointer + capacity_new - capacitance + epic_offset;
					memcpy(pick, new_pointer + epic_offset, (capacitance - epic_offset) * sizeof(T));
					place = new_pointer + epic_offset;
				} else {
					place = new_pointer + capacitance;
					pick = new_pointer;
				}
				capacitance = capacity_new;
				itemse = items + capacitance;

				new (place) T(item); //Assigning an item to a memory address.
				++stored_count;
				if (++place == itemse) {
					place = items;
				}
			}
		}
	}

	unsigned int capacity() const {
		return capacitance;
	}

	void capacity(unsigned int new_capacity) {
		if (!new_capacity) throw std::out_of_range("Capacity must be at least 1!");
		if (stored_count > new_capacity) throw std::out_of_range("The capacity of the structure cannot be less than the number of elements in it!");

		T *new_pointer = reinterpret_cast<T *>(malloc(new_capacity * sizeof(T)));
		if (!new_pointer) throw std::runtime_error("You don't have enough memory!");

		if (stored_count) {
			if (place > pick || place == items) {
				memcpy(new_pointer, pick, stored_count * sizeof(T));
			} else {
				memcpy(new_pointer, pick, (itemse - pick) * sizeof(T));
				memcpy(new_pointer + (itemse - pick), items, (place - items) * sizeof(T));
			}
		}

		pick = new_pointer;
		if (new_capacity == stored_count)
			place = new_pointer;
		else
			place = new_pointer + stored_count;

		capacitance = new_capacity;

		free(items); items = new_pointer;
		itemse = items + new_capacity;
	}

	void push(T item) {
		emplace(item);
	}

	bool isNotEmpty() const {
		return stored_count;
	}

	T peek() const {
		if (!stored_count) throw std::underflow_error("Nothing to Peek!");

		return *pick;
	}

	T pop() {
		if (stored_count) {
			T item = *pick;
			pick->~T();

			--stored_count;
			if (++pick == itemse) {
				pick = items;
			}
			return item;
		} else
			throw std::underflow_error("Nothing to Pop!");
	}

	unsigned int count() const {
		return stored_count;
	}

	void reverse() {
		if (stored_count > 1) {
			T tmp;
			T *pk = pick, *em = place - 1;
			if (em < items) em = itemse - 1;

			while (pk != em) {
				tmp = *pk; *pk = *em; *em = tmp;

				if (++pk == itemse) pk = items;
				if (pk == em) break;

				if (--em < items) em = itemse - 1;
			}
		}
	}

	void removeInRange(T min, T max) {
		if (stored_count) {
			T *tbuff = reinterpret_cast<T *>(malloc(capacitance * sizeof(T))), *cpos = tbuff;
			if (!tbuff) throw std::runtime_error("You don't have enough memory!");

			T *pk = pick, *em = place;
			do {
				if (*pk < min || *pk > max) {
					memcpy(cpos, pk, sizeof(T));
					++cpos;
				} else
					pk->~T();

				if (++pk == itemse) pk = items;
			} while (pk != em);

			pick = tbuff;

			stored_count = cpos - tbuff;
			if (stored_count == capacitance)
				place = tbuff;
			else
				place = tbuff + stored_count;
			
			free(items);
			items = tbuff;
			itemse = tbuff + capacitance;
		}
	}

	bool operator==(const Queue &arg) const {
		if (arg.stored_count != stored_count)
			return false;

		if (stored_count) {
			T *pk = pick, *em = place;
			T *pk2 = arg.pick;

			do {
				if (*pk != *pk2)
					return false;

				if (++pk == itemse)
					pk = items;
				if (++pk2 == arg.itemse)
					pk2 = arg.items;
			} while (pk != em);
		}

		return true;
	}

	bool operator>(const Queue &arg) const {
		if (stored_count) {
			if (arg.stored_count) {
				T *pk = pick, *em = place;
				T *pk2 = arg.pick, *em2 = arg.place;

				while (true) {
					if (*pk > *pk2)
						return true;
					else if (*pk < *pk2)
						return false;

					if (++pk == itemse)
						pk = items;
					if (pk == em) return false;
					if (++pk2 == arg.itemse)
						pk2 = arg.items;
					if (pk2 == em2) return true;
				}
			} else
				return true;
		}

		return false;
	}

	bool operator!=(const Queue &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const Queue &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const Queue &arg) const {
		return !(*this > arg);
	}

	bool operator<(const Queue &arg) const {
		return *this <= arg && *this != arg;
	}
};

#endif