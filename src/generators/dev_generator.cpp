#include "DEV_Generator.h"
#include <boost/random/uniform_real.hpp>
#include <boost/random/uniform_01.hpp>
#include <boost/random/uniform_int.hpp>
#include <climits>

DEV_Generator::DEV_Generator(const std::string &devName) {
	this->dev = new std::tr1::random_device(devName);
	this->name = devName;
}

DEV_Generator::DEV_Generator() {
	this->dev = new std::tr1::random_device("/dev/urandom");
	this->name = "/dev/urandom";
}

int DEV_Generator::getRandom() {
	boost::uniform_int<> rand_int(0,INT32_MAX);
	return rand_int(*(this->dev));
}

std::string DEV_Generator::getNameGenerator() const {
	return this->name;
}

double DEV_Generator::getRandom_01() {
	boost::uniform_01<std::tr1::random_device&> rand_01(*(this->dev));
	return rand_01();
}

double DEV_Generator::getMinMaxRandom(const int &min, const int &max) {
	boost::uniform_real<> rand_real(min, max);
	return rand_real(*(this->dev));
}

DEV_Generator::~DEV_Generator(){
	delete this->dev;
}
