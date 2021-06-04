#include <iostream>
#include <ctime>
#include <cstdlib>
#include <queue>
#include <vector>
#include <list>
#include <forward_list>
#include <algorithm>

#include <chrono>
#include "STD_CircularBuffer.h"
#include "Individuals_Circular.h"

#include "DoubleLinkedList.h"
#include "Individuals_DLL.h"

#include "SingleLinkedList.h"
#include "Individuals_SLL.h"

using namespace std;

void UnitTest_CircularBuffer();
void UnitTest_DoubleLinkedList();
void UnitTest_SingleLinkedList();

int main() {
	srand(time(nullptr));

	cout << "Unit tests:" << endl;
	UnitTest_CircularBuffer();
	UnitTest_DoubleLinkedList();
	UnitTest_SingleLinkedList();

	cout << endl << "Perfomance tests:" << endl;

	/* CircularBuffer region */
	STD_CircularBuffer<float> cb1(20);
	
	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		cb1.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		cb1.push(abs(rand()) / 1000.0F);
		cb1.push(abs(rand()) / 1000.0F);
		cb1.pop();
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::circular_buffer emulator time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	CircularBuffer<float> cb2(20);

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		cb2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		cb2.push(abs(rand()) / 1000.0F);
		cb2.push(abs(rand()) / 1000.0F);
		cb2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The CircularBuffer.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n\n";

	/* Double-Linked List region */
	list<float> l1;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		l1.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		l1.push_front(abs(rand()) / 1000.0F);
		l1.push_front(abs(rand()) / 1000.0F);
		l1.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::list time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	DoubleLinkedList<float> l2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		l2.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		l2.push_front(abs(rand()) / 1000.0F);
		l2.push_front(abs(rand()) / 1000.0F);
		l2.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The DoubleLinkedList.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	/* Single-Linked List region */
	forward_list<float> sl1;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		sl1.push_front(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		sl1.push_front(abs(rand()) / 1000.0F);
		sl1.push_front(abs(rand()) / 1000.0F);
		sl1.pop_front();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::forward_list time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	SingleLinkedList<float> sl2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		sl2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		sl2.push(abs(rand()) / 1000.0F);
		sl2.push(abs(rand()) / 1000.0F);
		sl2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The DoubleLinkedList.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	return 0;
}

/* CIRCULAR BUFFER UNIT TEST */
void UnitTest_CircularBuffer() {
	STD_CircularBuffer<float> cb1(10);
	CircularBuffer<float> cb2(10);

	float tmp1;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 50;
		if (tmp2 == 0) { //Merge sorted
			while (cb1.size() > 5)
				cb1.pop();
			while (cb2.count() > 5)
				cb2.pop();

			CircularBuffer<float> second({-1.48, 152, 8875, -256.337, -16.8574});
			second = idv::merge_with_sorted(cb2, second);

			auto cvec = cb1.to_vector();
			vector<float> vsecond({ -1.48, 152, 8875, -256.337, -16.8574 });
			vector<float> result;
			std::merge(cvec.begin(), cvec.end(), vsecond.begin(), vsecond.end(), std::back_inserter(result)); //Sequence NOT ordered, but it has no matter for this test.

			for (auto bg = second.begin(), ed = second.end(); bg != ed; ++bg) {
				if (result[bg.get_index()] != *bg) {
					cout << "circilar buffer: 1 - fail! merge" << " | Test #" << i << endl;
					return;
				}
			}
		} else if (tmp2 == 2) { //Get min & max
			if (cb2.count() > 1) {
				auto second = idv::get_min_max(cb2);

				auto cvec = cb1.to_vector();
				auto min = min_element(cvec.begin(), cvec.end());
				if (*min != second.pop()) {
					cout << "circilar buffer: 1 - fail! min" << " | Test #" << i << endl;
					return;
				}
				cvec.erase(min);
				auto max = max_element(cvec.begin(), cvec.end());
				if (*max != second.pop()) {
					cout << "circilar buffer: 1 - fail! max" << " | Test #" << i << endl;
					return;
				}
				cvec.erase(max);
				cb1.from_vector(cvec);
			}
		} else if (tmp2 < 10) { //Erase
			tmp2 = cb2.count();
			if (tmp2 != 0) {
				tmp2 = rand() % cb2.count();
				
				cb1.erase(tmp2);
				cb2.erase(tmp2);
			}
		} else if (tmp2 < 20) { //Insert
			tmp1 = rand() / 3.0;

			try {
				tmp2 = cb2.count();
				if (tmp2 != 0) {
					tmp2 = rand() % cb2.count();

					cb1.insert(tmp2, tmp1);
					cb2.insert(tmp2, tmp1);
				}
			} catch (const std::exception &) {
				cout << "circilar buffer: 1 - fail! insert" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 40) { //Push
			tmp1 = rand() / 3.0;

			try {
				cb1.push(tmp1);
				cb2.push(tmp1);
			} catch (const std::exception &) {
				cout << "circilar buffer: 1 - fail! push" << " | Test #" << i << endl;
				return;
			}
		} else { //Pop
			if (cb1.size() != cb2.count()) {
				cout << "circilar buffer: 1 - fail! pop1" << " | Test #" << i << endl;
				return;
			}

			if (cb2.count() != 0) {
				try {
					tmp1 = cb1.front();
					cb1.pop();

					if (tmp1 != cb2.pop()) {
						cout << "circilar buffer: 1 - fail! pop2" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "circilar buffer: 1 - fail! pop3" << " | Test #" << i << endl;
					return;
				}
			}
		}
	}

	{ //Verify
		auto vec = cb1.to_vector();
		auto oth = vec.begin();
		for (auto myb = cb2.begin(), mye = cb2.end(); myb != mye; ++myb) {
			if (*myb != *oth++) {
				cout << "circilar buffer: 2 - fail! verify" << endl;
				return;
			}
		}
	}

	CircularBuffer<int> cb3(10), cb4(8);

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a circular offset
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.push(0);
			cb4.push(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.pop();
			cb4.pop();
		}

		cb3.push(1); cb3.push(3); cb3.push(2); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2); cb4.push(4);
		if (cb3 != cb4 || !(cb3 == cb4) || !(cb3 <= cb4) || !(cb3 >= cb4)) {
			cout << "circilar buffer: 3 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop(); cb4.pop();

		cb3.push(1); cb3.push(3); cb3.push(3); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2); cb4.push(4);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "circilar buffer: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "circilar buffer: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop(); cb4.pop();

		cb3.push(1); cb3.push(3); cb3.push(2); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "circilar buffer: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "circilar buffer: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop();
	}

	cout << "circilar buffer: passed!" << endl;
}

/* DOUBLE LINKED LIST UNIT TEST */
void UnitTest_DoubleLinkedList() {
	list<float> cb1;
	DoubleLinkedList<float> cb2(10);

	float tmp1;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 60;
		if (tmp2 == 0) { //Merge sorted
			DoubleLinkedList<float> second({ -1.48, 152, 8875, -256.337, -16.8574 });
			second = idv::merge_with_sorted(cb2, second);

			vector<float> vsecond({ -1.48, 152, 8875, -256.337, -16.8574 });
			vector<float> result;
			std::merge(cb1.begin(), cb1.end(), vsecond.begin(), vsecond.end(), std::back_inserter(result)); //Sequence NOT ordered, but it has no matter for this test.

			for (auto bg = second.begin(), ed = second.end(); bg != ed; ++bg) {
				if (result[bg.get_index()] != *bg) {
					cout << "double-linked list: 1 - fail! merge" << " | Test #" << i << endl;
					return;
				}
			}
		} else if (tmp2 == 2) { //Get min & max
			if (cb2.count() > 1) {
				auto second = idv::get_min_max(cb2);

				auto min = min_element(cb1.begin(), cb1.end());
				if (*min != second.pop_back()) {
					cout << "double-linked list: 1 - fail! min" << " | Test #" << i << endl;
					return;
				}
				cb1.erase(min);
				auto max = max_element(cb1.begin(), cb1.end());
				if (*max != second.pop_back()) {
					cout << "double-linked list: 1 - fail! max" << " | Test #" << i << endl;
					return;
				}
				cb1.erase(max);
			}
		} else if (tmp2 < 10) { //Erase
			tmp2 = cb2.count();
			if (tmp2 != 0) {
				tmp2 = rand() % cb2.count();

				auto itemp = cb1.begin();
				for (int j = 0; j != tmp2; ++j, ++itemp);
				cb1.erase(itemp);

				cb2.erase(tmp2);
			}
		} else if (tmp2 < 20) { //Insert
			tmp1 = rand() / 3.0;

			try {
				tmp2 = cb2.count();
				if (tmp2 != 0) {
					tmp2 = rand() % cb2.count();

					auto itemp = cb1.begin();
					for (int j = 0; j != tmp2; ++j, ++itemp);
					cb1.insert(itemp, tmp1);

					cb2.insert(tmp2, tmp1);
				}
			} catch (const std::exception &) {
				cout << "double-linked list: 1 - fail! insert" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 30) { //Push back
			tmp1 = rand() / 3.0;

			try {
				cb1.push_back(tmp1);

				cb2.push_back(tmp1);
			} catch (const std::exception &) {
				cout << "double-linked list: 1 - fail! push back" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 40) { //Push front
			tmp1 = rand() / 3.0;

			try {
				cb1.push_front(tmp1);

				cb2.push_front(tmp1);
			} catch (const std::exception &) {
				cout << "double-linked list: 1 - fail! push front" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 50) { //Pop back
			if (cb1.size() != cb2.count()) {
				cout << "double-linked list: 1 - fail! pop1 back" << " | Test #" << i << endl;
				return;
			}

			if (cb2.count() != 0) {
				try {
					tmp1 = cb1.back();
					cb1.pop_back();

					if (tmp1 != cb2.pop_back()) {
						cout << "double-linked list: 1 - fail! pop2 back" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "double-linked list: 1 - fail! pop3 back" << " | Test #" << i << endl;
					return;
				}
			}
		} else { //Pop front
			if (cb1.size() != cb2.count()) {
				cout << "double-linked list: 1 - fail! pop1 front" << " | Test #" << i << endl;
				return;
			}

			if (cb2.count() != 0) {
				try {
					tmp1 = cb1.front();
					cb1.pop_front();

					if (tmp1 != cb2.pop_front()) {
						cout << "double-linked list: 1 - fail! pop2 front" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "double-linked list: 1 - fail! pop3 front" << " | Test #" << i << endl;
					return;
				}
			}
		}
	}

	{ //Verify
		auto oth = cb1.begin();
		for (auto myb = cb2.begin(), mye = cb2.end(); myb != mye; ++myb) {
			if (*myb != *oth++) {
				cout << "double-linked list: 2 - fail! verify" << endl;
				return;
			}
		}
	}

	DoubleLinkedList<int> cb3(10), cb4(8);

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a offset
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.push_back(0);
			cb4.push_back(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.pop_front();
			cb4.pop_front();
		}

		cb3.push_back(1); cb3.push_back(3); cb3.push_back(2); cb3.push_back(4);
		cb4.push_back(1); cb4.push_back(3); cb4.push_back(2); cb4.push_back(4);
		if (cb3 != cb4 || !(cb3 == cb4) || !(cb3 <= cb4) || !(cb3 >= cb4)) {
			cout << "double-linked list: 3 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		cb3.pop_front(); cb3.pop_front(); cb3.pop_front(); cb3.pop_front();
		cb4.pop_front(); cb4.pop_front(); cb4.pop_front(); cb4.pop_front();

		cb3.push_back(1); cb3.push_back(3); cb3.push_back(3); cb3.push_back(4);
		cb4.push_back(1); cb4.push_back(3); cb4.push_back(2); cb4.push_back(4);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "double-linked list: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "double-linked list: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop_back(); cb3.pop_back(); cb3.pop_back(); cb3.pop_back();
		cb4.pop_back(); cb4.pop_back(); cb4.pop_back(); cb4.pop_back();

		cb3.push_back(1); cb3.push_back(3); cb3.push_back(2); cb3.push_back(4);
		cb4.push_back(1); cb4.push_back(3); cb4.push_back(2);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "double-linked list: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "double-linked list: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop_back(); cb3.pop_back(); cb3.pop_back(); cb3.pop_back();
		cb4.pop_back(); cb4.pop_back(); cb4.pop_back();
	}

	cout << "double-linked list: passed!" << endl;
}


/* SINGLE LINKED LIST UNIT TEST */
void UnitTest_SingleLinkedList() {
	list<float> cb1;
	SingleLinkedList<float> cb2(10);

	float tmp1;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 60;
		if (tmp2 == 0) { //Merge sorted
			SingleLinkedList<float> second({ -1.48, 152, 8875, -256.337, -16.8574 });
			second = idv::merge_with_sorted(cb2, second);

			vector<float> vsecond({ -1.48, 152, 8875, -256.337, -16.8574 });
			vector<float> result;
			std::merge(cb1.begin(), cb1.end(), vsecond.begin(), vsecond.end(), std::back_inserter(result)); //Sequence NOT ordered, but it has no matter for this test.

			for (auto bg = second.begin(), ed = second.end(); bg != ed; ++bg) {
				if (result[bg.get_index()] != *bg) {
					cout << "single-linked list: 1 - fail! merge" << " | Test #" << i << endl;
					return;
				}
			}
		} else if (tmp2 == 2) { //Get min & max
			if (cb2.count() > 1) {
				auto second = idv::get_min_max(cb2);

				auto min = min_element(cb1.begin(), cb1.end());
				if (*min != second.pop()) {
					cout << "single-linked list: 1 - fail! min" << " | Test #" << i << endl;
					return;
				}
				cb1.erase(min);
				auto max = max_element(cb1.begin(), cb1.end());
				if (*max != second.pop()) {
					cout << "single-linked list: 1 - fail! max" << " | Test #" << i << endl;
					return;
				}
				cb1.erase(max);
			}
		} else if (tmp2 < 10) { //Erase
			tmp2 = cb2.count();
			if (tmp2 != 0) {
				tmp2 = rand() % cb2.count();

				auto itemp = cb1.begin();
				for (int j = 0; j != tmp2; ++j, ++itemp);
				cb1.erase(itemp);

				cb2.erase(tmp2);
			}
		} else if (tmp2 < 20) { //Insert
			tmp1 = rand() / 3.0;

			try {
				tmp2 = cb2.count();
				if (tmp2 != 0) {
					tmp2 = rand() % cb2.count();

					auto itemp = cb1.begin();
					for (int j = 0; j != tmp2; ++j, ++itemp);
					cb1.insert(itemp, tmp1);

					cb2.insert(tmp2, tmp1);
				}
			} catch (const std::exception &) {
				cout << "single-linked list: 1 - fail! insert" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 30) { //Push front
			tmp1 = rand() / 3.0;

			try {
				cb1.push_front(tmp1);

				cb2.push(tmp1);
			} catch (const std::exception &) {
				cout << "single-linked list: 1 - fail! push" << " | Test #" << i << endl;
				return;
			}
		} else { //Pop front
			if (cb1.size() != cb2.count()) {
				cout << "double-linked list: 1 - fail! pop1" << " | Test #" << i << endl;
				return;
			}

			if (cb2.count() != 0) {
				try {
					tmp1 = cb1.front();
					cb1.pop_front();

					if (tmp1 != cb2.pop()) {
						cout << "single-linked list: 1 - fail! pop2" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "single-linked list: 1 - fail! pop3" << " | Test #" << i << endl;
					return;
				}
			}
		}
	}

	{ //Verify
		auto oth = cb1.begin();
		for (auto myb = cb2.begin(), mye = cb2.end(); myb != mye; ++myb) {
			if (*myb != *oth++) {
				cout << "single-linked list: 2 - fail! verify" << endl;
				return;
			}
		}
	}

	SingleLinkedList<int> cb3(10), cb4(8);

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a offset
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.push(0);
			cb4.push(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			cb3.pop();
			cb4.pop();
		}

		cb3.push(1); cb3.push(3); cb3.push(2); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2); cb4.push(4);
		if (cb3 != cb4 || !(cb3 == cb4) || !(cb3 <= cb4) || !(cb3 >= cb4)) {
			cout << "single-linked list: 3 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop(); cb4.pop();

		cb3.push(1); cb3.push(3); cb3.push(3); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2); cb4.push(4);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "single-linked list: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "single-linked list: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop(); cb4.pop();

		cb3.push(1); cb3.push(3); cb3.push(2); cb3.push(4);
		cb4.push(1); cb4.push(3); cb4.push(2);
		if (cb3 < cb4 || cb3 <= cb4) {
			cout << "single-linked list: 3 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(cb3 > cb4) || !(cb3 >= cb4)) {
			cout << "single-linked list: 3 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		cb3.pop(); cb3.pop(); cb3.pop(); cb3.pop();
		cb4.pop(); cb4.pop(); cb4.pop();
	}

	cout << "single-linked list: passed!" << endl;
}