#ifndef GENERATOR_H_
#define GENERATOR_H_

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <type_traits>
#include <random>
#include <exception>
#include <cassert>

// Compile with flag -std=c++17

/**
 * @brief Generate vector with random (numeric) values
 * @param[in] N - the size of vector (count of elements)
 *
 * @return vector with random values
 * @note if pass void like a type of template, will throw exception
 */
template <class T>
auto generate(const size_t N) {
    static_assert(!std::is_void<T>::value, "<E> Can't generate array of void");
    std::vector<T> res;

    assert(N != 0);
    assert(N < res.max_size());

    res.reserve(N);
    std::random_device rd;
    std::mt19937 mt(rd());

    if constexpr (std::is_arithmetic<T>::value) {
	auto gen = [&]() {
	    if constexpr (std::is_floating_point<T>::value) {
	        std::uniform_real_distribution<T> dist(1, 100); // rand from 1 to 100
    		for (size_t i = 0; i < N; i++) {
    		    res.emplace_back(dist(mt));
    		}
	    } else {
	        std::uniform_int_distribution<T> dist(1, 100);
    		for (size_t i = 0; i < N; i++) {
    		    res.emplace_back(dist(mt));
    		}
	    }
	};

	gen(); // lambda (non name) function

    } else {
    	std::throw_with_nested(std::runtime_error("<E> Can't work with unknown types"));
    }

    return res;
}


template<>
auto generate<std::string> (const size_t N) { // template specialization for std::string
    std::vector<std::string> res;

    assert(N != 0);
    assert(N < res.max_size());
    res.reserve(N);

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_int_distribution<int> dist(1, 100);

    for (size_t i = 0; i < N; i++) {
       res.emplace_back(std::to_string(dist(mt)));
    }

    return res;
}

#endif
