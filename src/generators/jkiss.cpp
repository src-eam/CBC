#include "JKISS_Generator.h"
#include <climits>
#include<cmath>
const std::string JKISS_Generator::name = "JKISS";

JKISS_Generator::JKISS_Generator() :
		x(123456789), y(987654321), z(43219876), c(6543217) {
}

JKISS_Generator::JKISS_Generator(const unsigned int &x, const unsigned int &y,
		const unsigned int &z, const unsigned int &c) {
	this->x = x;
	this->y = y;
	this->z = z;
	this->c = c;
}

std::string JKISS_Generator::getNameGenerator() const {
	return this->name;
}

int JKISS_Generator::getRandom() {
	unsigned long long t;
	x = 314527869 * x + 1234567;
	y ^= y << 5;
	y ^= y >> 7;
	y ^= y << 22;
	t = 4294584393ULL * z + c;
	c = t >> 32;
	z = t;
	return x + y + z;
}

double JKISS_Generator::getRandom_01() {
	double x;
	unsigned long long a;
	a = ((unsigned long long) this->getRandom() << 32) + this->getRandom();
	a = (a >> 12) | 0x3FF0000000000000ULL;
	*((unsigned long long *) &x) = a;
	return x - 1.0;
}

double JKISS_Generator::getMinMaxRandom(const int &min, const int &max) {
	return fabs(((double) this->getRandom() / (double) UINT_MAX))
			* fabs(((double) max - (double) min)) + (double) min;
}
