#ifndef Individual_HL_D679BA40CEC24336AE47CA16782DFFBA
#define Individual_HL_D679BA40CEC24336AE47CA16782DFFBA

#include <fstream>
#include <chrono>
#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <memory>

#include "HashList.h"

namespace idv
{
	

	class Individual {
		std::unique_ptr<HashList<std::string, std::vector<std::string>>> countries, cities;

	public:
		Individual() {
			countries = std::unique_ptr<HashList<std::string, std::vector<std::string>>>(new HashList<std::string, std::vector<std::string>>(0.75, 2048));
			cities = std::unique_ptr<HashList<std::string, std::vector<std::string>>>(new HashList<std::string, std::vector<std::string>>(0.75, 4096));

			std::chrono::high_resolution_clock::time_point begin = std::chrono::high_resolution_clock::now();
			{
				std::ifstream istr("cities.txt", std::ios::in);
				std::string buff, contry, city;
				while (!istr.eof()) {
					getline(istr, buff);
					size_t comma = buff.find_first_of(',');
					contry = buff.substr(0, comma);
					city = buff.substr(comma + 1);

					if (comma != SIZE_MAX) {
						std::vector<std::string> *ccpair = countries->find(contry);
						if (ccpair) {
							ccpair->push_back(city);
						} else {
							countries->emplace(contry, std::vector<std::string> { city });
						}
						ccpair = cities->find(city);
						if (ccpair) {
							ccpair->push_back(contry);
						} else {
							cities->emplace(city, std::vector<std::string> { contry });
						}
					}
				}
				istr.close();
			}
			std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
			std::cout << "cities.txt loaded in: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms\n";
		}

		std::vector<std::string> cities_by_country(const std::string& country) const {
			auto ccpair = countries->find(country);
			if (ccpair) {
				return *ccpair;
			} else
				return std::vector<std::string>();
		}

		enum class IsInCityAnswer { NO, YES, DONT_KNOW };

		IsInCityAnswer is_city_in_country(const std::string &country, const std::string &city) const {
			auto ccpair = cities->find(city);
			if (ccpair) {
				for (auto item : *ccpair) {
					if (item == country) {
						return IsInCityAnswer::YES;
					}
				}
				return IsInCityAnswer::NO;
			} 
			return IsInCityAnswer::DONT_KNOW;
		}
	};
}

#endif
