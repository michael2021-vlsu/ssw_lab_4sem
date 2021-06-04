#pragma once

#ifndef Individuals_SLL_D679BA40CEC24336AE47CA16782DFFBA
#define Individuals_SLL_D679BA40CEC24336AE47CA16782DFFBA

#include <exception>
#include "SingleLinkedList.h"

namespace idv
{
	template <typename T>
	SingleLinkedList<T> merge_with_sorted(const SingleLinkedList<T> &first, const SingleLinkedList<T> &second) {
		SingleLinkedList<T> temp(first.count() + second.count());

		auto il = first.begin(), ile = first.end(), io = second.begin(), ioe = second.end();

		while (il != ile && io != ioe) {
			if (*io > *il) {
				temp.push(*il++);
			} else {
				temp.push(*io++);
			}
		}

		for (; il != ile; ++il) {
			temp.push(*il);
		}

		for (; io != ioe; ++io) {
			temp.push(*io);
		}

		SingleLinkedList<T> buffer(temp.count());
		while (temp.count()) {
			buffer.push(temp.pop());
		}

		return buffer;
	}

	template <typename T>
	SingleLinkedList<T> get_min_max(SingleLinkedList<T> &arg) {
		if (arg.count() < 2) throw std::underflow_error("Count of elements must be at least 2!");

		auto it = arg.begin(), min = it, max = it++;

		for (auto ite = arg.end(); it != ite; ++it) {
			if (*it < *min)
				min = it;
			else if (*it > *max)
				max = it;
		}

		SingleLinkedList<T> new_buffer{ *min, *max };

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