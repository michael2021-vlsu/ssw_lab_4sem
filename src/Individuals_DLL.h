#pragma once

#ifndef Individuals_DLL_D679BA40CEC24336AE47CA16782DFFBA
#define Individuals_DLL_D679BA40CEC24336AE47CA16782DFFBA

#include <exception>
#include "DoubleLinkedList.h"

namespace idv
{
	template <typename T>
	DoubleLinkedList<T> merge_with_sorted(const DoubleLinkedList<T> &first, const DoubleLinkedList<T> &second) {
		DoubleLinkedList<T> new_buffer(first.count() + second.count());

		auto il = first.begin(), ile = first.end(), io = second.begin(), ioe = second.end();

		while (il != ile && io != ioe) {
			if (*io > *il) {
				new_buffer.push_back(*il++);
			} else {
				new_buffer.push_back(*io++);
			}
		}

		for (; il != ile; ++il) {
			new_buffer.push_back(*il);
		}

		for (; io != ioe; ++io) {
			new_buffer.push_back(*io);
		}

		return new_buffer;
	}

	template <typename T>
	DoubleLinkedList<T> get_min_max(DoubleLinkedList<T> &arg) {
		if (arg.count() < 2) throw std::underflow_error("Count of elements must be at least 2!");

		auto it = arg.begin(), min = it, max = it++;

		for (auto ite = arg.end(); it != ite; ++it) {
			if (*it < *min)
				min = it;
			else if (*it > *max)
				max = it;
		}

		DoubleLinkedList<T> new_buffer{ *max, *min };

		if (max.get_index() > min.get_index()) {
			arg.erase(max.get_index());
			arg.erase(min.get_index());
		} else {
			arg.erase(min.get_index());
			arg.erase(max.get_index());
		}

		return new_buffer;
	}
}

#endif