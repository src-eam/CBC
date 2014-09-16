/*
 * mersenne_twister.cpp
 *
 *      Author: eugene
 */

#include "MersenneTwister_Generator.h"
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int.hpp>
#include <climits>

const std::string MersenneTwister_Generator::name = "MersenneTwister";

MersenneTwister_Generator::MersenneTwister_Generator() {
	this->gen = new boost::mt11213b();
}

MersenneTwister_Generator::~MersenneTwister_Generator() {
	delete this->gen;
}

std::string MersenneTwister_Generator::getNameGenerator() const {
	return this->name;
}

int MersenneTwister_Generator::getRandom() {
	boost::uniform_int<> rand_int(0, INT32_MAX);
	return rand_int(*(this->gen));
}

double MersenneTwister_Generator::getMinMaxRandom(const int &min,const int &max) {
	boost::uniform_real<> rand_real(min, max);
	return rand_real(*(this->gen));
}

double MersenneTwister_Generator::getRandom_01() {
	boost::uniform_01<boost::mt11213b&> rand_01(*(this->gen));
	return rand_01();
}
