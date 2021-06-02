#include <iostream>
#include <queue>
#include <vector>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <algorithm>

#include <chrono>

#include "queue.h"
#include "Vector.h"
#include "Stack.h"

using namespace std;

void UnitTest_Queue();
void UnitTest_Vector();
void UnitTest_Stack();


int main() {
	srand(time(nullptr));

	cout << "Unit tests:" << endl;
	UnitTest_Queue();
	UnitTest_Vector();
	UnitTest_Stack();

	cout << endl << "Perfomance tests:" << endl;

	/* Queue region */
	std::queue<float> q1;
	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		q1.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		q1.push(abs(rand()) / 1000.0F);
		q1.push(abs(rand()) / 1000.0F);
		q1.pop();
		q1.pop();
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::queue time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Queue<float> q2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		q2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		q2.push(abs(rand()) / 1000.0F);
		q2.push(abs(rand()) / 1000.0F);
		q2.pop();
		q2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The queue.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n\n";
	
	/* Vector region */
	std::vector<float> v1;
	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		v1.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		v1.push_back(abs(rand()) / 1000.0F);
		v1.push_back(abs(rand()) / 1000.0F);
		v1.pop_back();
		v1.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::vector time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Vector<float> v2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		v2.push_back(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		v2.push_back(abs(rand()) / 1000.0F);
		v2.push_back(abs(rand()) / 1000.0F);
		v2.pop_back();
		v2.pop_back();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The Vector.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n\n";
	
	/* Stack region */
	std::stack<float> s1;
	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		s1.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		s1.push(abs(rand()) / 1000.0F);
		s1.push(abs(rand()) / 1000.0F);
		s1.pop();
		s1.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The std::stack time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";

	Stack<float> s2;

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i < 6; ++i) {
		s2.push(abs(rand()) / 1000.0F);
	}
	for (size_t i = 0; i < 200; ++i) {
		s2.push(abs(rand()) / 1000.0F);
		s2.push(abs(rand()) / 1000.0F);
		s2.pop();
		s2.pop();
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "The Stack.h time: " << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << " us\n";
	
	return 0;
}

/* UNIT TEST - QUEUE */
void UnitTest_Queue() {
	queue<float> q1;
	Queue<float> q2;

	float tmp1, tmp3;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 50;
		if (tmp2 == 0) { //Reverse
			tmp2 = q2.count();
			if (tmp2 != 0) {
				q2.reverse();

				stack<float> st;
				for (unsigned short j = 0; j != tmp2; ++j) {
					st.push(q1.front());
					q1.pop();
				}
				for (unsigned short j = 0; j != tmp2; ++j) {
					q1.push(st.top());
					st.pop();
				}
			}
		} else if (tmp2 == 2) { //Delete Range
			tmp2 = q2.count();
			if (tmp2 != 0) {
				tmp1 = rand() / 32767.0;
				tmp3 = tmp1 + rand() / 512.0;
				q2.removeInRange(tmp1, tmp3);

				vector<float> st;
				for (unsigned short j = 0; j != tmp2; ++j) {
					if (q1.front() < tmp1 || q1.front() > tmp3)
						st.push_back(q1.front());
					q1.pop();
				}
				for (unsigned short j = 0; j != st.size(); ++j) {
					q1.push(st[j]);
				}
			}
		} else if (tmp2 == 3) { //capacity
			tmp2 = q2.count() + rand() % 15;
			q2.capacity(tmp2 ? tmp2 : 1);
		} else if (tmp2 < 25) { //Push
			tmp1 = rand() / 3.0;

			try {
				q1.push(tmp1);
				q2.push(tmp1);
			} catch (const std::exception &) {
				cout << "queue: 1 - fail! push" << " | Test #" << i << endl;
				return;
			}
		} else { //Pop
			if (q1.size() != q2.count()) {
				cout << "queue: 1 - fail! pop1" << " | Test #" << i << endl;
				return;
			}

			if (q2.count() != 0) {
				try {
					tmp1 = q1.front();
					q1.pop();

					if (tmp1 != q2.peek()) {
						cout << "queue: 1 - fail! pop2" << " | Test #" << i << endl;
						return;
					}
					if (tmp1 != q2.pop()) {
						cout << "queue: 1 - fail! pop3" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "queue: 1 - fail! pop4" << " | Test #" << i << endl;
					return;
				}
			}
		}
	}

	Queue<int> q3(10), q4(8);

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a circular offset
		for (unsigned short j = 0; j != 3; ++j) {
			q3.push(0);
			q4.push(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			q3.pop();
			q4.pop();
		}

		q3.push(1); q3.push(3); q3.push(2); q3.push(4);
		q4.push(1); q4.push(3); q4.push(2); q4.push(4);
		if (q3 != q4 || !(q3 == q4) || !(q3 <= q4) || !(q3 >= q4)) {
			cout << "queue: 2 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		q3.pop(); q3.pop(); q3.pop(); q3.pop();
		q4.pop(); q4.pop(); q4.pop(); q4.pop();

		q3.push(1); q3.push(3); q3.push(3); q3.push(4);
		q4.push(1); q4.push(3); q4.push(2); q4.push(4);
		if (q3 < q4 || q3 <= q4) {
			cout << "queue: 2 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(q3 > q4) || !(q3 >= q4) ) {
			cout << "queue: 2 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		q3.pop(); q3.pop(); q3.pop(); q3.pop();
		q4.pop(); q4.pop(); q4.pop(); q4.pop();
		
		q3.push(1); q3.push(3); q3.push(2); q3.push(4);
		q4.push(1); q4.push(3); q4.push(2);
		if (q3 < q4 || q3 <= q4) {
			cout << "queue: 2 - fail! 4" << " | Test #" << i << endl;
			return;
		}

		if (!(q3 > q4) || !(q3 >= q4)) {
			cout << "queue: 2 - fail! 5" << " | Test #" << i << endl;
			return;
		}

		q3.pop(); q3.pop(); q3.pop(); q3.pop();
		q4.pop(); q4.pop(); q4.pop();
	}

	cout << "queue: passed!" << endl;
}

/* UNIT TEST - VECTOR */
void UnitTest_Vector() {
	vector<float> v1; //Reference STD vector
	Vector<float> v2; //Testing vector

	float tmp1, tmp3;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 70;
		if (tmp2 == 0) { //Reverse
			tmp2 = v2.count();
			if (tmp2 != 0) {
				v2.reverse();

				std::reverse(v1.begin(), v1.end()); //Reverse reference vector
			}
		} else if (tmp2 == 2) { //Delete Range
			tmp2 = v2.count();
			if (tmp2 != 0) {
				tmp1 = rand() / 32767.0;
				tmp3 = tmp1 + rand() / 512.0;
				
				v2.deleteRange(tmp1, tmp3);

				vector<float> tvec;
				for (auto bg = v1.begin(), ed = v1.end(); bg != ed; ++bg) { //Go for all items in reference vector
					if (*bg < tmp1 || *bg > tmp3) //Check item's range condition
						tvec.push_back(*bg); //If ok, copy item to new vector
				}
				v1 = tvec; //Update reference vector
			}
		} else if (tmp2 < 10) { //at
			if (v1.size() != v2.count()) {
				cout << "vector: 1 - fail! at1" << " | Test #" << i << endl;
				return;
			}

			if (v2.count() != 0) {
				tmp2 = abs(rand()) % v2.count();

				if (v1.at(tmp2) != v2.at(tmp2)) {
					cout << "vector: 1 - fail! at2" << " | Test #" << i << endl;
					return;
				}
			}
		} else if (tmp2 < 25) { //Push_back
			tmp1 = rand() / 3.0;

			try {
				v1.push_back(tmp1);
				v2.push_back(tmp1);
			} catch (const std::exception &) {
				cout << "vector: 1 - fail! push_back" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 40) { //Push_front
			tmp1 = rand() / 3.0;

			try {
				v1.insert(v1.begin(), tmp1);
				v2.push_front(tmp1);
			} catch (const std::exception &) {
				cout << "vector: 1 - fail! push_front" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 55) { //Pop_back
			if (v1.size() != v2.count()) {
				cout << "vector: 1 - fail! pop_back1" << " | Test #" << i << endl;
				return;
			}

			if (v2.count() != 0) {
				try {
					tmp1 = v1.back();
					v1.pop_back();

					if (tmp1 != v2.pop_back()) {
						cout << "vector: 1 - fail! pop_back2" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "vector: 1 - fail! pop_back3" << " | Test #" << i << endl;
					return;
				}
			}
		} else if (tmp2 < 70) { //Pop_front
			if (v1.size() != v2.count()) {
				cout << "vector: 1 - fail! pop_front1" << " | Test #" << i << endl;
				return;
			}

			if (v2.count() != 0) {
				try {
					tmp1 = v1.front();
					v1.erase(v1.begin());

					if (tmp1 != v2.pop_front()) {
						cout << "vector: 1 - fail! pop_front2" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "vector: 1 - fail! pop_front3" << " | Test #" << i << endl;
					return;
				}
			}
		}
	}

	Vector<int> v3, v4;

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a circular offset
		for (unsigned short j = 0; j != 3; ++j) {
			v3.push_back(0);
			v4.push_back(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			v3.pop_front();
			v4.pop_front();
		}

		v3.push_back(1); v3.push_back(3); v3.push_back(2); v3.push_back(4);
		v4.push_back(1); v4.push_back(3); v4.push_back(2); v4.push_back(4);
		if (v3 != v4 || !(v3 == v4) || !(v3 <= v4) || !(v3 >= v4)) {
			cout << "vector: 2 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		v3.pop_front(); v3.pop_front(); v3.pop_front(); v3.pop_front();
		v4.pop_front(); v4.pop_front(); v4.pop_front(); v4.pop_front();

		v3.push_back(1); v3.push_back(3); v3.push_back(3); v3.push_back(4);
		v4.push_back(1); v4.push_back(3); v4.push_back(2); v4.push_back(4);
		if (v3 < v4 || v3 <= v4) {
			cout << "vector: 2 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(v3 > v4) || !(v3 >= v4)) {
			cout << "vector: 2 - fail! 3" << " | Test #" << i << endl;
			return;
		}

		v3.pop_front(); v3.pop_front(); v3.pop_front(); v3.pop_front();
		v4.pop_front(); v4.pop_front(); v4.pop_front(); v4.pop_front();
		
		v3.push_back(1); v3.push_back(3); v3.push_back(2); v3.push_back(4);
		v4.push_back(1); v4.push_back(3); v4.push_back(2);
		if (v3 < v4 || v3 <= v4) {
			cout << "vector: 2 - fail! 4" << " | Test #" << i << endl;
			return;
		}

		if (!(v3 > v4) || !(v3 >= v4)) {
			cout << "vector: 2 - fail! 5" << " | Test #" << i << endl;
			return;
		}

		v3.pop_front(); v3.pop_front(); v3.pop_front(); v3.pop_front();
		v4.pop_front(); v4.pop_front(); v4.pop_front();
	}

	cout << "vector: passed!" << endl;
}

/* UNIT TEST - STACK */
void UnitTest_Stack() {
	vector<float> v1; //Reference STD vector
	Stack<float> v2; //Testing stack

	float tmp1, tmp3;
	int tmp2;
	for (unsigned short i = 0; i != 5000; ++i) {
		tmp2 = abs(rand()) % 70;
		if (tmp2 == 0) { //Reverse
			tmp2 = v2.count();
			if (tmp2 != 0) {
				v2.reverse();

				std::reverse(v1.begin(), v1.end()); //Reverse reference vector
			}
		} else if (tmp2 == 2) { //Delete Range
			tmp2 = v2.count();
			if (tmp2 != 0) {
				tmp1 = rand() / 32767.0;
				tmp3 = tmp1 + rand() / 512.0;

				v2.deleteRange(tmp1, tmp3);

				vector<float> tvec;
				for (auto bg = v1.begin(), ed = v1.end(); bg != ed; ++bg) { //Go for all items in reference vector
					if (*bg < tmp1 || *bg > tmp3) //Check item's range condition
						tvec.push_back(*bg); //If ok, copy item to new vector
				}
				v1 = tvec; //Update reference vector
			}
		} else if (tmp2 < 25) { //Push_back
			tmp1 = rand() / 3.0;

			try {
				v1.push_back(tmp1);
				v2.push(tmp1);
			} catch (const std::exception &) {
				cout << "stack: 1 - fail! push_back" << " | Test #" << i << endl;
				return;
			}
		} else if (tmp2 < 55) { //Pop_back
			if (v1.size() != v2.count()) {
				cout << "stack: 1 - fail! pop_back1" << " | Test #" << i << endl;
				return;
			}

			if (v2.count() != 0) {
				try {
					tmp1 = v1.back();
					v1.pop_back();

					if (tmp1 != v2.pop()) {
						cout << "vector: 1 - fail! pop_back2" << " | Test #" << i << endl;
						return;
					}
				} catch (const std::exception &) {
					cout << "vector: 1 - fail! pop_back3" << " | Test #" << i << endl;
					return;
				}
			}
		} 
	}

	Stack<int> v3, v4;

	for (unsigned short i = 0; i != 10; ++i) {
		//Making a circular offset
		for (unsigned short j = 0; j != 3; ++j) {
			v3.push(0);
			v4.push(0);
		}
		for (unsigned short j = 0; j != 3; ++j) {
			v3.pop();
			v4.pop();
		}

		v3.push(1); v3.push(3); v3.push(2); v3.push(4);
		v4.push(1); v4.push(3); v4.push(2); v4.push(4);
		if (v3 != v4 || !(v3 == v4) || !(v3 <= v4) || !(v3 >= v4)) {
			cout << "stack: 2 - fail! 1" << " | Test #" << i << endl;
			return;
		}
		v3.pop(); v3.pop(); v3.pop(); v3.pop();
		v4.pop(); v4.pop(); v4.pop(); v4.pop();

		v3.push(1); v3.push(3); v3.push(3); v3.push(4);
		v4.push(1); v4.push(3); v4.push(2); v4.push(4);
		if (v3 < v4 || v3 <= v4) {
			cout << "stack: 2 - fail! 2" << " | Test #" << i << endl;
			return;
		}

		if (!(v3 > v4) || !(v3 >= v4)) {
			cout << "stack: 2 - fail! 3" << " | Test #" << i << endl;
			return;
		}
		v3.pop(); v3.pop(); v3.pop(); v3.pop();
		v4.pop(); v4.pop(); v4.pop(); v4.pop();

		v3.push(1); v3.push(3); v3.push(2); v3.push(4);
		v4.push(1); v4.push(3); v4.push(2);
		if (v3 < v4 || v3 <= v4) {
			cout << "stack: 2 - fail! 4" << " | Test #" << i << endl;
			return;
		}

		if (!(v3 > v4) || !(v3 >= v4)) {
			cout << "stack: 2 - fail! 5" << " | Test #" << i << endl;
			return;
		}
		v3.pop(); v3.pop(); v3.pop(); v3.pop();
		v4.pop(); v4.pop(); v4.pop();
	}

	cout << "stack: passed!" << endl;
}