#pragma once

#ifndef DoubleLinkedList_D679BA40CEC24336AE47CA16782DFFBA
#define DoubleLinkedList_D679BA40CEC24336AE47CA16782DFFBA

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <iterator>

template <typename T>
class DoubleLinkedList {
	struct Node {
		std::shared_ptr<Node> *me{nullptr};
		std::weak_ptr<Node> prew, next;
		T item;

		void init(std::shared_ptr<Node> *me, const T &item) {
			this->me = me;
			this->item = item;
		}
		void init(std::shared_ptr<Node> *me, const T &item, std::weak_ptr<Node> prew) {
			this->me = me;
			this->item = item;
			this->prew = prew;

			if (!prew.expired())
				prew.lock()->next = *me;
		}
		void init(std::shared_ptr<Node> *me, const T &item, std::weak_ptr<Node> prew, std::weak_ptr<Node> next) {
			this->me = me;
			this->item = item;
			this->prew = prew;
			this->next = next;

			if (!prew.expired())
				prew.lock()->next = *me;

			if (!next.expired())
				next.lock()->prew = *me;
		}

		void remove() {
			if (!prew.expired()) prew.lock()->next = next;
			if (!next.expired()) next.lock()->prew = prew;
		}
	};
	/*
	* It would be more efficient to use a "Node" array to reduce data fragmentation and decrease allocation calls count and increase perfomance.
	* However, this method is more descriptive and simple.
	* In addition, this approach allowed not to use raw memory manipulation functions (like memcpy).
	*/
	std::unique_ptr<std::shared_ptr<Node>[]> items;

	std::shared_ptr<Node> *fempty, *itemse;
	std::weak_ptr<Node> first, last;

	unsigned int capacitance, stored_count;
public:
	DoubleLinkedList(unsigned int capacity = 8):capacitance(capacity) {
		if (!capacity) throw std::invalid_argument("Capacity must be at least 1!");

		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacity]);

		fempty = items.get();

		stored_count = 0;
		itemse = fempty + capacity;
	}

	DoubleLinkedList(const std::initializer_list<T> &list): capacitance(list.size()) {
		if (!capacitance) throw std::invalid_argument("Capacity must be at least 1!");

		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);

		stored_count = capacitance;
		fempty = itemse = items.get() + capacitance;

		const T *citem = list.begin();
		std::shared_ptr<Node> prew_node = nullptr;
		for (std::shared_ptr<Node> *cwpos = items.get(); cwpos != itemse; ++cwpos, ++citem) {
			*cwpos = std::shared_ptr<Node>(new Node());
			(*cwpos)->init(cwpos, *citem, prew_node);

			prew_node = *cwpos;
		}

		first = items[0];
		last = prew_node;
	}

	DoubleLinkedList(const DoubleLinkedList &arg): capacitance(arg.stored_count), stored_count(arg.stored_count) {
		items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);
		fempty = itemse = items.get() + capacitance;

		std::weak_ptr<Node> prew_my_node, current_other_node = arg.first;
		for (std::shared_ptr<Node> *itm = items.get(); itm != itemse; ++itm) {
			auto lck = current_other_node.lock();

			*itm = std::shared_ptr<Node>(new Node());
			(*itm)->init(itm, lck->item, prew_my_node);

			prew_my_node = *itm;
			current_other_node = lck->next;
		}

		first = items[0];
		last = prew_my_node;
	}

	DoubleLinkedList &operator=(const DoubleLinkedList &arg) {
		if (this != &arg) {
			capacitance = stored_count = arg.stored_count;

			items = std::unique_ptr<std::shared_ptr<Node>[]>(new std::shared_ptr<Node>[capacitance]);
			fempty = itemse = items.get() + capacitance;

			std::weak_ptr<Node> prew_my_node, current_other_node = arg.first;
			for (std::shared_ptr<Node> *itm = items.get(); itm != itemse; ++itm) {
				auto lck = current_other_node.lock();

				*itm = std::shared_ptr<Node>(new Node());
				(*itm)->init(itm, lck->item, prew_my_node);

				prew_my_node = *itm;
				current_other_node = lck->next;
			}

			first = items[0];
			last = prew_my_node;
		}

		return *this;
	}

	/*void logit(const char *title) {
		std::ofstream logger("lg.log", std::ios::out | std::ios::app);
		logger << "======== " << title << endl;
		logger << "== first: " << first.lock().get() << endl;
		logger << "== last: " << last.lock().get() << endl;
		logger << "== fempty - 1: " << fempty - 1 << endl;
		logger << "== fempty: " << fempty << endl;
		logger << "-------- sh_p - node_p - item - node_me - node_me_node - prew - next" << endl;
		for (std::shared_ptr<Node> *cwpos = items.get(); cwpos != fempty; ++cwpos) {
			logger << cwpos << '\t' << cwpos->get() << '\t' << (*cwpos)->item << '\t' << (*cwpos)->me << '\t' << (*cwpos)->me->get() << '\t' << (*cwpos)->prew.lock().get() << '\t' << (*cwpos)->next.lock().get() << endl;
		}
		logger << "--------" << endl;
		std::weak_ptr<Node> current_node = first;
		while (!current_node.expired()) {
			logger << current_node.lock().get() << '\t' << current_node.lock()->item << endl;
			current_node = current_node.lock()->next;
		}
		logger << "|| " << stored_count << endl;
		logger << endl << endl;
		logger.close();
	}*/

	void emplace(const T &item) {
		if (fempty != itemse) {
			if (last.expired() || first.expired()) {
				*fempty = std::shared_ptr<Node>(new Node());
				(*fempty)->init(fempty, item);
				last = first = *fempty++;
			} else {
				*fempty = std::shared_ptr<Node>(new Node());
				(*fempty)->init(fempty, item, last);
				last = *fempty++;
			}
		} else {
			std::unique_ptr<std::shared_ptr<Node>[]> new_items(new std::shared_ptr<Node>[capacitance <<= 1]);

			std::shared_ptr<Node> *itm = new_items.get();
			std::weak_ptr<Node> prew_my_node, current_other_node = first;
			do {
				auto lck = current_other_node.lock();

				*itm = std::shared_ptr<Node>(new Node());
				(*itm)->init(itm, lck->item, prew_my_node);

				prew_my_node = *itm++;
				current_other_node = lck->next;
			} while (!current_other_node.expired());
			
			items = std::move(new_items);
			itemse = items.get() + capacitance;

			first = items[0];
			last = *itm = std::shared_ptr<Node>(new Node());
			(*itm)->init(itm, item, prew_my_node);

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
				current_node = current_node.lock()->next;
			}

			*fempty = std::shared_ptr<Node>(new Node());
			(*fempty)->init(fempty, item, prew_node, current_node);

			if (!index)
				first = *fempty;

			++fempty;
		} else {
			std::unique_ptr<std::shared_ptr<Node>[]> new_items(new std::shared_ptr<Node>[capacitance <<= 1]);

			std::shared_ptr<Node> *itm = new_items.get();
			std::weak_ptr<Node> prew_my_node, current_other_node = first;
			for (unsigned int i = 0; !current_other_node.expired(); ++i) {
				if (i == index) {
					*itm = std::shared_ptr<Node>(new Node());
					(*itm)->init(itm, item, prew_my_node);
					prew_my_node = *itm++;
				}

				auto lck = current_other_node.lock();

				*itm = std::shared_ptr<Node>(new Node());
				(*itm)->init(itm, lck->item, prew_my_node);

				prew_my_node = *itm++;
				current_other_node = lck->next;
			}

			items = std::move(new_items);
			itemse = items.get() + capacitance;

			first = items[0];
			last = prew_my_node;
			fempty = itm;
		}
		++stored_count;
	}

	void erase(unsigned int index) {
		if (index >= stored_count) throw std::out_of_range("Index out of range!");

		std::weak_ptr<Node> current_node = first;
		for (unsigned int i = 0; i != index; ++i) {
			current_node = current_node.lock()->next;
		}

		auto lck = current_node.lock();
		lck->remove();

		if (lck == last.lock())
			last = lck->prew;

		if (lck == first.lock())
			first = lck->next;

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

	void push_back(T item) {
		emplace(item);
	}

	void push_front(T item) {
		if (stored_count)
			emplace(0, item);
		else
			emplace(item);
	}

	T pop_back() {
		if (stored_count) {
			auto lck = last.lock();

			T item = lck->item;
			
			lck->remove();
			last = lck->prew;

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

	T pop_front() {
		if (stored_count) {
			auto lck = first.lock();

			T item = lck->item;

			lck->remove();
			first = lck->next;

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


	bool operator==(const DoubleLinkedList &arg) const {
		if (arg.stored_count != stored_count)
			return false;

		if (stored_count) {
			std::weak_ptr<Node> my = first, other = arg.first;

			do {
				auto myl = my.lock(), otherl = other.lock();

				if (myl->item != otherl->item)
					return false;

				my = myl->next;
				other = otherl->next;
			} while (!my.expired());
		}

		return true;
	}

	bool operator>(const DoubleLinkedList &arg) const {
		if (stored_count) {
			if (arg.stored_count) {
				std::weak_ptr<Node> my = first, other = arg.first;

				while (true) {
					auto myl = my.lock(), otherl = other.lock();

					if (myl->item > otherl->item)
						return true;
					else if (myl->item < otherl->item)
						return false;

					my = myl->next;
					if (my.expired()) return false;

					other = otherl->next;
					if (other.expired()) return true;
				}
			} else
				return true;
		}

		return false;
	}

	bool operator!=(const DoubleLinkedList &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const DoubleLinkedList &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const DoubleLinkedList &arg) const {
		return !(*this > arg);
	}

	bool operator<(const DoubleLinkedList &arg) const {
		return *this <= arg && *this != arg;
	}

	class iterator {
		friend class DoubleLinkedList;

		unsigned int index, err_index;
		std::weak_ptr<Node> ptr;
		iterator(std::weak_ptr<Node> current, unsigned int index, unsigned int err_index): ptr(current), index(index), err_index(err_index) { }
	public:
		using iterator_category = std::bidirectional_iterator_tag;
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
			if (++index != err_index)
				ptr = ptr.lock()->next;
			return *this;
		}

		iterator operator++(int) {
			if (index == err_index) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			if (++index != err_index)
				ptr = ptr.lock()->next;
			return temp;
		}

		iterator &operator--() {
			if (index == 0) throw std::out_of_range("Iterator out of range!");
			if (index-- != err_index)
				ptr = ptr.lock()->prew;
			return *this;
		}

		iterator operator--(int) {
			if (index == 0) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			if (index-- != err_index)
				ptr = ptr.lock()->prew;
			return temp;
		}
	};

	iterator begin() const {
		return iterator(first, 0, stored_count);
	}

	iterator end() const {
		return iterator(last, stored_count, stored_count);
	}

	DoubleLinkedList merge_with_sorted(const DoubleLinkedList &other) const {
		DoubleLinkedList<T> buffer(stored_count + other.stored_count);

		iterator il = begin(), ile = end(), io = other.begin(), ioe = other.end();

		while (il != ile && io != ioe) {
			if (*io > *il) {
				buffer.push_back(*il++);
			} else {
				buffer.push_back(*io++);
			}
		}

		for (; il != ile; ++il) {
			buffer.push_back(*il);
		}

		for (; io != ioe; ++io) {
			buffer.push_back(*io);
		}

		return buffer;
	}

	DoubleLinkedList get_min_max() {
		if (stored_count < 2) throw std::underflow_error("Count of elements must be at least 2!");

		DoubleLinkedList<T>::iterator it = begin(), min = it, max = it++;

		for (DoubleLinkedList<T>::iterator ite = end(); it != ite; ++it) {
			if (*it < *min)
				min = it;
			else if (*it > *max)
				max = it;
		}

		DoubleLinkedList<T> new_buffer{ *max, *min };

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
std::ostream &operator<<(std::ostream &out, const DoubleLinkedList<T> &buffer) {
	for (auto item : buffer) {
		out << item << std::endl;
	}
	return out;
}

#endif