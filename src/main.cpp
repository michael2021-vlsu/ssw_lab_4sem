#include <iostream>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

#include "HashList.h"

using namespace std;

void Individual_Tasks() {
	HashList<std::string, std::vector<std::string>> countries(0.75, 2048);
	HashList<std::string, std::string> cities(0.75, 4096);

	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	{
		ifstream istr("cities.txt", ios::in);
		std::string buff, contry, city;
		while (!istr.eof()) {
			getline(istr, buff);
			size_t comma = buff.find_first_of(',');
			contry = buff.substr(0, comma);
			city = buff.substr(comma + 1);

			if (comma != SIZE_MAX) {
				auto ccpair = countries.find(contry);
				if (ccpair) {
					ccpair->push_back(city);
				} else {
					countries.emplace(contry, std::vector<std::string> { city });
				}
				cities.emplace(city, contry);

			}
		}
		istr.close();
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "file loaded in: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	{
		std::string buff;
		cout << "What you want to do?" << endl << "1) get cities by country" << endl << "2) check if city is a part of country" << endl;
		unsigned short select;
		cin >> select;
		cin.ignore(256, '\n');

		if (select == 1) {
			cout << "Enter the name of the country: ";
			getline(cin, buff);
			auto ccpair = countries.find(buff);
			if (ccpair) {
				cout << "Founded " << ccpair->size() << " cities:" << endl;
				for (auto city : *ccpair) {
					cout << city << endl;
				}
			} else
				cout << "File does not contain this country!";
		} else {
			cout << "Enter the name of city: ";
			getline(cin, buff);
			auto ccpair = cities.find(buff);
			if (ccpair) {
				cout << "Enter the name of the country: ";
				cin.clear();
				getline(cin, buff);
				if (buff == *ccpair) {
					cout << "This city is a part of mentioned country.";
				} else {
					cout << "This country have no such city!";
				}
			} else
				cout << "File does not contain this city!";
		}
	}

	cout << endl;
}

int main() {
	// Tests
	HashList<int, float> test(0.7, 5), test2{ std::pair<int, float>(8,15), std::pair<int, float>(9,25), std::pair<int, float>(8,45) };

	test.emplace(1, 1.1);
	test.emplace(2, 2.1);
	test.emplace(3, 3.1);
	test.emplace(4, 4.1);
	test.emplace(5, 5.1);
	test.emplace(6, 6.1);
	test.emplace(7, 7.1);
	test.emplace(8, 8.1);
	cout << test << endl;
	auto bg = test.begin(), ed = test.end();
	ed--;
	for (; bg != ed; --ed) {
		cout << ed->first << " : " << ed->second << endl;
	}
	cout << ed->first << " : " << ed->second << endl;

	test2.erase(8);
	test2.erase(9);

	cout << "This is smaller than none? " << (test < test2) << "\nThis is bigger than none? " << (test > test2) << "\nEqual? " << (test == test2) << endl;
	cout << "How much of items? " << test.count() << endl;
	test.erase(3);
	cout << "And now? " << test.count() << endl;
	cout << "What about the element with key 3? " << test.find(3) << endl << endl;

	cout << "Perfomance check..." << endl;

	std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 5000; ++i) {
		test2.emplace(rand(), rand() / 3.0);
	}
	std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
	std::cout << "emplace x5000 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 5000; ++i) {
		test2.find(rand());
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "look for random key x5000 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	begin = std::chrono::high_resolution_clock::now();
	for (size_t i = 0; i != 5000; ++i) {
		test2.erase(rand());
	}
	end = std::chrono::high_resolution_clock::now();
	std::cout << "erase for random key x5000 time: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";

	cout << "Tests passed!" << endl << endl;

	Individual_Tasks();

	return 0;
}