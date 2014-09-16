/*
 * fibonacci_lagged.cpp
 *
 *      Author: eugene
 */

#include "Fibonacci_Generator.h"
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int.hpp>
#include <climits>

const std::string Fibonacci_Generator::name = "Fibonacci60";

Fibonacci_Generator::Fibonacci_Generator() {
	this->gen = new boost::lagged_fibonacci607();
}

Fibonacci_Generator::~Fibonacci_Generator() {
	delete this->gen;
}

std::string Fibonacci_Generator::getNameGenerator() const {
	return this->name;
}

int Fibonacci_Generator::getRandom() {
	boost::uniform_int<> rand_int(0, INT32_MAX);
	return rand_int(*(this->gen));
}

double Fibonacci_Generator::getMinMaxRandom(const int &min,const int &max) {
	boost::uniform_real<> rand_real(min, max);
	return rand_real(*(this->gen));
}

double Fibonacci_Generator::getRandom_01() {
	boost::uniform_01<boost::lagged_fibonacci607&> rand_01(*(this->gen));
	return rand_01();
}
