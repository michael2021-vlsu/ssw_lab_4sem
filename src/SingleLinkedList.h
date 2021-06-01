#pragma once

#ifndef SingleLinkedList_D679BA40CEC24336AE47CA16782DFFBA
#define SingleLinkedList_D679BA40CEC24336AE47CA16782DFFBA

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <iterator>

#include <fstream>

template <typename T>
class SingleLinkedList {
	struct Node {
		std::shared_ptr<Node> *me;
		std::weak_ptr<Node> prew;
		T item;

		void init(std::shared_ptr<Node> *me, const T &item) {
			this->me = me;
			this->item = item;
		}
		void init(std::shared_ptr<Node> *me, const T &item, std::weak_ptr<Node> prew) {
			this->me = me;
			this->item = item;
			this->prew = prew;
		}
	};
	/*
	* It would be more efficient to use a "Node" array to reduce data fragmentation and decrease allocation calls count and increase perfomance.
	* However, this method is more descriptive and simple.
	* In addition, this approach allowed not to use raw memory manipulation functions (like memcpy).
	*/
	std::unique_ptr<std::shared_ptr<Node>[]> items;

	std::shared_ptr<Node> *fempty, *itemse;
	std::weak_ptr<Node> first;

	unsigned int capacitance, stored_count;
public:
	SingleLinkedList(unsigned int capacity = 8):capacitance(capacity) {
		if (!capacity) throw std::invalid_argument("Capacity must be at least 1!");

		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacity]);

		fempty = items.get();

		stored_count = 0;
		itemse = fempty + capacity;
	}

	SingleLinkedList(const std::initializer_list<T> &list): capacitance(list.size()) {
		if (!capacitance) throw std::invalid_argument("Capacity must be at least 1!");

		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);

		stored_count = capacitance;
		fempty = itemse = items.get() + capacitance;

		const T *citem = list.begin();
		std::shared_ptr<Node> *prew_my_node = nullptr;
		for (std::shared_ptr<Node> *itm = items.get(); itm != itemse; ++itm) {
			*itm = std::shared_ptr<Node>(new Node());
			if (prew_my_node) {
				(*prew_my_node)->init(prew_my_node, *citem++, *itm);
			} else
				first = *itm;

			prew_my_node = itm;
		}
		(*prew_my_node)->init(prew_my_node, *citem);
	}

	SingleLinkedList(const SingleLinkedList &arg): capacitance(arg.stored_count), stored_count(arg.stored_count) {
		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);
		fempty = itemse = items.get() + capacitance;

		std::weak_ptr<Node> current_other_node = arg.first;
		std::shared_ptr<Node> *prew_my_node = nullptr;
		for (std::shared_ptr<Node> *itm = items.get(); itm != itemse; ++itm) {
			*itm = std::shared_ptr<Node>(new Node());
			if (prew_my_node) {
				auto lck = current_other_node.lock();
				(*prew_my_node)->init(prew_my_node, lck->item, *itm);
				current_other_node = lck->prew;
			} else
				first = *itm;

			prew_my_node = itm;
		}
		(*prew_my_node)->init(prew_my_node, current_other_node.lock()->item);
	}

	SingleLinkedList &operator=(const SingleLinkedList &arg) {
		capacitance = stored_count = arg.stored_count;

		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);
		fempty = itemse = items.get() + capacitance;

		std::weak_ptr<Node> current_other_node = arg.first;
		std::shared_ptr<Node> *prew_my_node = nullptr;
		for (std::shared_ptr<Node> *itm = items.get(); itm != itemse; ++itm) {
			*itm = std::shared_ptr<Node>(new Node());
			if (prew_my_node) {
				auto lck = current_other_node.lock();
				(*prew_my_node)->init(prew_my_node, lck->item, *itm);
				current_other_node = lck->prew;
			} else
				first = *itm;

			prew_my_node = itm;
		}
		(*prew_my_node)->init(prew_my_node, current_other_node.lock()->item);

		return *this;
	}

	/*void logit(const char *title) {
		std::ofstream logger("lg.log", std::ios::out | std::ios::app);
		logger << "======== " << title << endl;
		logger << "== first: " << first.lock().get() << endl;
		logger << "== fempty - 1: " << fempty - 1 << endl;
		logger << "== fempty: " << fempty << endl;
		logger << "-------- sh_p - node_p - item - node_me - node_me_node - prew" << endl;
		for (std::shared_ptr<Node> *cwpos = items.get(); cwpos != fempty; ++cwpos) {
			logger << cwpos << '\t' << cwpos->get() << '\t' << (*cwpos)->item << '\t' << (*cwpos)->me << '\t' << (*cwpos)->me->get() << '\t' << (*cwpos)->prew.lock().get() << endl;
		}
		logger << "--------" << endl;
		std::weak_ptr<Node> current_node = first;
		while (!current_node.expired()) {
			logger << current_node.lock().get() << '\t' << current_node.lock()->item << endl;
			current_node = current_node.lock()->prew;
		}
		logger << "|| " << stored_count << endl;
		logger << endl << endl;
		logger.close();
	}*/

	void emplace(const T &item) {
		if (fempty != itemse) {
			if (first.expired()) {
				*fempty = std::shared_ptr<Node>(new Node());
				(*fempty)->init(fempty, item);
				first = *fempty++;
			} else {
				*fempty = std::shared_ptr<Node>(new Node());
				(*fempty)->init(fempty, item, first);
				first = *fempty++;
			}
		} else {
			std::unique_ptr<std::shared_ptr<Node>[]> new_items(new std::shared_ptr<Node>[capacitance <<= 1]);

			std::shared_ptr<Node> *prew_my_node = nullptr, *itm = new_items.get(), *to = itm + stored_count;
			std::weak_ptr<Node> current_other_node = first;

			do {
				*itm = std::shared_ptr<Node>(new Node());
				if (prew_my_node) {
					auto lck = current_other_node.lock();
					(*prew_my_node)->init(prew_my_node, lck->item, *itm);
					current_other_node = lck->prew;
				} else
					first = *itm;

				prew_my_node = itm++;
			} while (itm != to);
			*itm = std::shared_ptr<Node>(new Node());
			(*itm)->init(itm, item, first);
			first = *itm;

			(*prew_my_node)->init(prew_my_node, current_other_node.lock()->item, *itm);

			items = std::move(new_items);
			itemse = items.get() + capacitance;

			fempty = ++itm;
		}
		++stored_count;
	}

	void emplace(unsigned int index, const T &item) {
		if (index >= stored_count) throw std::out_of_range("Index out of range!");
		if (fempty != itemse) {
			std::weak_ptr<Node> current_node = first, prew_node;
			for (unsigned int i = 0; i != index; ++i) {
				prew_node = current_node;
				current_node = current_node.lock()->prew;
			}

			*fempty = std::shared_ptr<Node>(new Node());
			if (prew_node.expired()) {
				(*fempty)->init(fempty, item, first);
				first = *fempty;
			} else {
				auto lck = prew_node.lock();
				(*fempty)->init(fempty, item, lck->prew);
				lck->prew = *fempty;
			}

			++fempty;
		} else {
			std::unique_ptr<std::shared_ptr<Node>[]> new_items(new std::shared_ptr<Node>[capacitance <<= 1]);

			std::shared_ptr<Node> *itm = new_items.get(), *to = itm + stored_count + 1, *prew_my_node = nullptr;
			std::weak_ptr<Node> current_other_node = first;
			++index;
			for (unsigned int i = 0; itm != to; ++i) {
				if (i == index) {
					*itm = std::shared_ptr<Node>(new Node());

					(*prew_my_node)->init(prew_my_node, item, *itm);
					prew_my_node = itm++;
				}

				*itm = std::shared_ptr<Node>(new Node());
				if (prew_my_node) {
					auto lck = current_other_node.lock();
					(*prew_my_node)->init(prew_my_node, lck->item, *itm);
					current_other_node = lck->prew;
				} else
					first = *itm;

				prew_my_node = itm++;
			}
			(*prew_my_node)->init(prew_my_node, current_other_node.lock()->item);

			items = std::move(new_items);
			itemse = items.get() + capacitance;

			fempty = itm;
		}
		++stored_count;
	}

	void erase(unsigned int index) {
		if (index >= stored_count) throw std::out_of_range("Index out of range!");

		std::weak_ptr<Node> current_node = first;
		auto lck = current_node.lock();

		if (index) {
			for (unsigned int i = 1; i != index; ++i) {
				current_node = current_node.lock()->prew;
			}
			lck = current_node.lock();
			current_node = current_node.lock()->prew;

			if (!lck->prew.expired()) {
				lck->prew = lck->prew.lock()->prew;
			}
		} else {
			first = lck->prew;
		}
		
		lck = current_node.lock();
		if (*(--fempty) != lck) {
			*(lck->me) = *fempty;
			(*fempty)->me = lck->me;
		}
		fempty->reset();

		--stored_count;
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
			auto lck = first.lock();

			T item = lck->item;

			first = lck->prew;

			if (*(--fempty) != lck) {
				*(lck->me) = *fempty;
				(*fempty)->me = lck->me;
			}
			fempty->reset();

			--stored_count;
			return item;
		} else
			throw std::underflow_error("Nothing to Pop!");
	}


	unsigned int count() const {
		return stored_count;
	}


	bool operator==(const SingleLinkedList &arg) const {
		if (arg.stored_count != stored_count)
			return false;

		if (stored_count) {
			std::weak_ptr<Node> my = first, other = arg.first;

			do {
				auto myl = my.lock(), otherl = other.lock();

				if (myl->item != otherl->item)
					return false;

				my = myl->prew;
				other = otherl->prew;
			} while (!my.expired());
		}

		return true;
	}

	bool operator>(const SingleLinkedList &arg) const {
		if (stored_count) {
			std::weak_ptr<Node> my = first, other = arg.first;

			while (true) {
				auto myl = my.lock(), otherl = other.lock();

				if (myl->item > otherl->item)
					return true;
				else if (myl->item < otherl->item)
					return false;

				my = myl->prew;
				if (my.expired()) return false;

				other = otherl->prew;
				if (other.expired()) return true;
			}
		}

		return false;
	}

	bool operator!=(const SingleLinkedList &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const SingleLinkedList &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const SingleLinkedList &arg) const {
		return !(*this > arg);
	}

	bool operator<(const SingleLinkedList &arg) const {
		return *this <= arg && *this != arg;
	}

	class iterator {
		friend class SingleLinkedList;

		unsigned int index, err_index;
		std::weak_ptr<Node> ptr;
		iterator(unsigned int index, unsigned int err_index): index(index), err_index(err_index) { }
		iterator(unsigned int index, unsigned int err_index, std::weak_ptr<Node> current): index(index), err_index(err_index) {
			ptr = current;
		}
	public:
		using iterator_category = std::forward_iterator_tag;
		using value_type = T;
		using difference_type = std::ptrdiff_t;
		using pointer = T *;
		using reference = T &;

		unsigned int get_index() {
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
			return ptr.lock()->item;
		}

		T *operator->() const {
			if (index == err_index) throw std::logic_error("Can not dereference end iterator!");
			return &(ptr.lock()->item);
		}

		iterator &operator++() {
			if (index == err_index) throw std::out_of_range("Iterator out of range!");
			++index;
			ptr = ptr.lock()->prew;
			return *this;
		}

		iterator operator++(int) {
			if (index == err_index) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			++index;
			ptr = ptr.lock()->prew;
			return temp;
		}
	};

	iterator begin() const {
		return iterator(0, stored_count, first);
	}

	iterator end() const {
		return iterator(stored_count, stored_count);
	}

	SingleLinkedList merge_with_sorted(const SingleLinkedList &other) const {
		SingleLinkedList<T> temp(stored_count + other.stored_count);

		iterator il = begin(), ile = end(), io = other.begin(), ioe = other.end();

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

	SingleLinkedList get_min_max() {
		if (stored_count < 2) throw std::underflow_error("Count of elements must be at least 2!");

		SingleLinkedList<T>::iterator it = begin(), min = it, max = it++;

		for (SingleLinkedList<T>::iterator ite = end(); it != ite; ++it) {
			if (*it < *min)
				min = it;
			else if (*it > *max)
				max = it;
		}

		SingleLinkedList<T> new_buffer{ *min, *max };

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
std::ostream &operator<<(std::ostream &out, const SingleLinkedList<T> &buffer) {
	for (auto item : buffer) {
		out << item << std::endl;
	}
	return out;
}

#endif