#ifndef HashList_D679BA40CEC24336AE47CA16782DFFBA
#define HashList_D679BA40CEC24336AE47CA16782DFFBA

#include <cstring>
#include <iostream>
#include <stdexcept>
#include <functional>
#include <iterator>
#include <utility>
#include <memory>


template <typename Tk, typename Ti, class Hasher = std::hash<Tk>>
class HashList {
	/*
	* It would be more efficient to use a "Node" array to reduce data fragmentation and decrease allocation calls count and increase perfomance.
	* However, this method is more descriptive and simple.
	* In addition, this approach allowed not to use raw memory manipulation functions (like memcpy).
	*/
	std::shared_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]> chunks;
	std::unique_ptr<size_t[]> chunks_icount;

	size_t chunks_count = 1,
		chunk_capacity = 1024, chunk_items_count_threshold = 768;

	Hasher ghash;

	inline void HashIncrement(size_t &hash) const {
		if (++hash == chunk_capacity)
			hash = 0;
	}

	inline size_t CalcCount() const {
		size_t summ = 0;
		for (size_t i = 0; i != chunks_count; ++i) {
			summ += chunks_icount[i];
		}
		return summ;
	}
public:
	HashList(const HashList &arg) = delete;
	HashList &operator=(const HashList &arg) = delete;

	HashList(float fulness_coefficient = 0.75, size_t capacity = 1024): chunk_capacity(capacity) {
		chunks = std::shared_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]>(new std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[chunks_count]);
		chunks[0] = std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>(new std::shared_ptr<std::pair<Tk, Ti>>[chunk_capacity]);

		chunks_icount = std::unique_ptr<size_t[]>(new size_t[1]);
		chunks_icount[0] = 0;

		if (fulness_coefficient <= 0 || fulness_coefficient >= 1) throw std::out_of_range("Coefficient must be between 0 and 1!");
		chunk_items_count_threshold = capacity * fulness_coefficient;
	}

	HashList(const std::initializer_list<std::pair<Tk, Ti>> &list) {
		chunks = std::shared_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]>(new std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[chunks_count]);
		if (list.size() * 4 < 1024) {
			chunks[0] = std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>(new std::shared_ptr<std::pair<Tk, Ti>>[chunk_capacity]);
		} else {
			chunk_capacity = list.size() << 2;
			chunk_items_count_threshold = chunk_capacity * 0.75;
			chunks[0] = std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>(new std::shared_ptr<std::pair<Tk, Ti>>[chunk_capacity]);
		}
		chunks_icount = std::unique_ptr<size_t[]>(new size_t[1]);
		chunks_icount[0] = 0;

		for (auto pitem : list) {
			size_t ohash = ghash(pitem.first) % chunk_capacity;

			bool need_more = true;
			if (chunks[0][ohash].get() == nullptr) {
				chunks[0][ohash] = std::make_unique<std::pair<Tk, Ti>>(pitem.first, pitem.second);
				++chunks_icount[0];
				need_more = false;
			} else if (chunks[0][ohash]->first == pitem.first) {
				chunks[0][ohash] = std::make_unique<std::pair<Tk, Ti>>(pitem.first, pitem.second);
				need_more = false;
			}

			if (need_more) {
				//Search for existing
				size_t nhash = ohash;
				HashIncrement(nhash);

				while (chunks[0][nhash].get() != nullptr && nhash != ohash) {
					if (chunks[0][nhash]->first == pitem.first) {
						chunks[0][nhash] = std::make_unique<std::pair<Tk, Ti>>(pitem.first, pitem.second);
						return;
					}
					HashIncrement(nhash);
				}

				//Insert
				chunks[0][nhash] = std::make_unique<std::pair<Tk, Ti>>(pitem.first, pitem.second);
				++chunks_icount[0];
			}
		}
	}

	void emplace(const Tk &key, const Ti &item) {
		size_t ohash = ghash(key) % chunk_capacity;

		size_t min_items_in = 0;
		for (size_t chi = 0, min_items_count = SIZE_MAX; chi != chunks_count; ++chi) {
			if (chunks[chi][ohash].get() == nullptr) {
				chunks[chi][ohash] = std::make_unique<std::pair<Tk, Ti>>(key, item);
				++chunks_icount[chi];
				return;
			} else if (chunks[chi][ohash]->first == key) {
				chunks[chi][ohash] = std::make_unique<std::pair<Tk, Ti>>(key, item);
				return;
			}

			if (chunks_icount[chi] < min_items_count)
				min_items_in = chi;
		}

		if (chunks_icount[min_items_in] > chunk_items_count_threshold) {
			size_t new_chunks_count = chunks_count + 1;
			std::shared_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]> new_chunks =
				std::shared_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]>(new std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[new_chunks_count]);

			std::unique_ptr<size_t[]>new_chunks_icount = std::unique_ptr<size_t[]>(new size_t[new_chunks_count]);

			for (size_t chi = 0; chi != chunks_count; ++chi) {
				new_chunks[chi] = std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>(new std::shared_ptr<std::pair<Tk, Ti>>[chunk_capacity]);
				for (size_t i = 0; i != chunk_capacity; ++i) {
					new_chunks[chi][i] = chunks[chi][i];
				}
				new_chunks_icount[chi] = chunks_icount[chi];
			}
			new_chunks[chunks_count] = std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>(new std::shared_ptr<std::pair<Tk, Ti>>[chunk_capacity]);
			new_chunks_icount[chunks_count] = 0;

			chunks_count = new_chunks_count;

			chunks = new_chunks;
			chunks_icount = std::move(new_chunks_icount);
		}

		//Search for existing
		size_t nhash = ohash;
		HashIncrement(nhash);

		for (size_t chi = 0, min_items_count = SIZE_MAX; chi != chunks_count; ++chi) {
			while (chunks[chi][nhash].get() != nullptr && nhash != ohash) {
				if (chunks[chi][nhash]->first == key) {
					chunks[chi][nhash] = std::make_unique<std::pair<Tk, Ti>>(key, item);
					return;
				}
				HashIncrement(nhash);
			}
			nhash = ohash;
			HashIncrement(nhash);

			if (chunks_icount[chi] < min_items_count)
				min_items_in = chi;
		}

		//Insert
		while (true) {
			if (chunks[min_items_in][ohash].get() == nullptr) {
				chunks[min_items_in][ohash] = std::make_unique<std::pair<Tk, Ti>>(key, item);
				++chunks_icount[min_items_in];
				return;
			}
			HashIncrement(ohash);
		}
	}

	Ti* find(const Tk& key) const {
		size_t ohash = ghash(key) % chunk_capacity;

		for (size_t chi = 0; chi != chunks_count; ++chi) {
			if (chunks[chi][ohash].get() == nullptr) {
				continue;
			} else if (chunks[chi][ohash]->first == key) {
				return &(chunks[chi][ohash]->second);
			}
		}

		//Search for existing
		size_t nhash = ohash;
		HashIncrement(nhash);

		for (size_t chi = 0; chi != chunks_count; ++chi) {
			while (chunks[chi][nhash].get() != nullptr && nhash != ohash) {
				if (chunks[chi][nhash]->first == key) {
					return &(chunks[chi][nhash]->second);
				}
				HashIncrement(nhash);
			}
			nhash = ohash;
			HashIncrement(nhash);
		}

		return nullptr;
	}

	bool erase(const Tk &key) {
		size_t ohash = ghash(key) % chunk_capacity;

		for (size_t chi = 0; chi != chunks_count; ++chi) {
			if (chunks[chi][ohash].get() == nullptr) {
				continue;
			} else if (chunks[chi][ohash]->first == key) {
				chunks[chi][ohash].reset();
				--chunks_icount[chi];
				return true;
			}
		}

		//Search for existing
		size_t nhash = ohash;
		HashIncrement(nhash);

		for (size_t chi = 0; chi != chunks_count; ++chi) {
			while (chunks[chi][nhash].get() != nullptr && nhash != ohash) {
				if (chunks[chi][nhash]->first == key) {
					chunks[chi][nhash].reset();
					--chunks_icount[chi];
					return true;
				}
				HashIncrement(nhash);
			}
			nhash = ohash;
			HashIncrement(nhash);
		}

		return false;
	}

	size_t count() const {
		return CalcCount();
	}

	class iterator {
		friend class HashList;

		//Base info
		std::weak_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]> chunks;
		size_t chunks_count, chunk_capacity;

		//Uniq info
		size_t current_chunk, current_item;
		//First valid item
		size_t valid_chunk_min, valid_item_min;

		iterator(std::weak_ptr<std::shared_ptr<std::shared_ptr<std::pair<Tk, Ti>>[]>[]> chunks, size_t chunks_count, size_t chunk_capacity, size_t current_chunk, size_t current_item, bool pure):
			chunks(chunks), chunks_count(chunks_count), chunk_capacity(chunk_capacity), current_chunk(current_chunk), current_item(current_item) {
			if (pure && current_item == 0) {
				while (!chunks.lock()[this->current_chunk][this->current_item].get()) {
					if (++this->current_item == chunk_capacity) {
						if (++this->current_chunk == chunks_count) {
							--this->current_chunk;
							break;
						}
						this->current_item = 0;
					}
				};
				valid_chunk_min = this->current_chunk;
				valid_item_min = this->current_item;
			} else {
				current_chunk = 0;
				current_item = 0;
				while (!chunks.lock()[current_chunk][current_item].get()) {
					if (++current_item == chunk_capacity) {
						if (++current_chunk == chunks_count) {
							--current_chunk;
							break;
						}
						current_item = 0;
					}
				};
				valid_chunk_min = current_chunk;
				valid_item_min = current_item;
			}
		}
	public:
		using iterator_category = std::bidirectional_iterator_tag;
		using value_type = std::pair<Tk, Ti>;
		using difference_type = std::ptrdiff_t;
		using pointer = std::pair<Tk, Ti> *;
		using reference = std::pair<Tk, Ti> &;

		bool operator!=(iterator const &other) const {
			return !(current_item == other.current_item && current_chunk == other.current_chunk);
		}

		bool operator==(iterator const &other) const {
			return current_item == other.current_item && current_chunk == other.current_chunk;
		}

		std::pair<Tk, Ti> &operator*() const {
			if (current_item == chunk_capacity) throw std::logic_error("Can not dereference end iterator!");
			return *(chunks.lock()[current_chunk][current_item]);
		}

		std::pair<Tk, Ti> *operator->() const {
			if (current_item == chunk_capacity) throw std::logic_error("Can not dereference end iterator!");
			return chunks.lock()[current_chunk][current_item].get();
		}

		iterator &operator++() {
			if (current_item == chunk_capacity) throw std::out_of_range("Iterator out of range!");
			do {
				if (++current_item == chunk_capacity) {
					if (++current_chunk == chunks_count) {
						--current_chunk;
						break;
					}
					current_item = 0;
				}
			} while (!chunks.lock()[current_chunk][current_item].get());
			return *this;
		}

		iterator operator++(int) {
			if (current_item == chunk_capacity) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			do {
				if (++current_item == chunk_capacity) {
					if (++current_chunk == chunks_count) {
						--current_chunk;
						break;
					}
					current_item = 0;
				}
			} while (!chunks.lock()[current_chunk][current_item].get());
			return temp;
		}

		iterator &operator--() {
			if (current_item == valid_item_min && current_chunk == valid_chunk_min) throw std::out_of_range("Iterator out of range!");
			do {
				if (current_item-- == 0) {
					--current_chunk;
					current_item = chunk_capacity - 1;
				}
			} while (!chunks.lock()[current_chunk][current_item].get());
			return *this;
		}

		iterator operator--(int) {
			if (current_item == valid_item_min && current_chunk == valid_chunk_min) throw std::out_of_range("Iterator out of range!");
			iterator temp(*this);
			do {
				if (current_item-- == 0) {
					--current_chunk;
					current_item = chunk_capacity - 1;
				}
			} while (!chunks.lock()[current_chunk][current_item].get());
			return temp;
		}
	};

	iterator begin() const {
		return iterator(chunks, chunks_count, chunk_capacity, 0, 0, true);
	}

	iterator end() const {
		return iterator(chunks, chunks_count, chunk_capacity, chunks_count - 1, chunk_capacity, false);
	}

	bool operator==(const HashList &arg) const {
		auto my_count = CalcCount(), other_count = arg.CalcCount();

		if (my_count != other_count)
			return false;

		if (my_count) {
			auto my = begin(), mye = end(), other = arg.begin();

			do {
				if (*my != *other)
					return false;

				++my; ++other;
			} while (my != mye);
		}

		return true;
	}

	bool operator>(const HashList &arg) const {
		auto my_count = CalcCount(), other_count = arg.CalcCount();

		if (my_count != 0) {
			if (other_count != 0) {
				auto my = begin(), mye = end(), other = arg.begin(), othere = arg.end();

				while (true) {
					if (*my > *other)
						return true;
					else if (*my < *other)
						return false;

					if (++my == mye) return false;
					if (++other == othere) return true;
				}
			} else
				return true;
		}

		return false;
	}

	bool operator!=(const HashList &arg) const {
		return !(*this == arg);
	}

	bool operator>=(const HashList &arg) const {
		return *this > arg || *this == arg;
	}

	bool operator<=(const HashList &arg) const {
		return !(*this > arg);
	}

	bool operator<(const HashList &arg) const {
		return *this <= arg && *this != arg;
	}
};

template<typename Ti, typename Ts>
std::ostream &operator<<(std::ostream &out, const HashList<Ti, Ts> &buffer) {
	for (auto item : buffer) {
		out << item.first << " : " << item.second << std::endl;
	}
	return out;
}

#endif