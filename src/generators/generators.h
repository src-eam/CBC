#ifndef GENERATORS_H_
#define GENERATORS_H_

#include <string>

class IRandomGenerator {
public:
	virtual int getRandom() = 0;
	virtual double getMinMaxRandom(const int &min, const int &max) = 0;
	virtual double getRandom_01() = 0;
	virtual std::string getNameGenerator() const = 0;
	virtual ~IRandomGenerator() {};
};

#endif
