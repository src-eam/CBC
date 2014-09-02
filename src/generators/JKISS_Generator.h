#ifndef JKISS_GENERATOR_H_
#define JKISS_GENERATOR_H_

#include "generators.h"

class JKISS_Generator: public IRandomGenerator {
private:
	unsigned int x, y, z, c;
	static const std::string name;
public:
	JKISS_Generator();
	JKISS_Generator(const unsigned int &x, const unsigned int &y,
			const unsigned int &z, const unsigned int &c);

	virtual int getRandom();
	virtual double getMinMaxRandom(const int &min, const int &max);
	virtual double getRandom_01();
	virtual std::string getNameGenerator() const;
	virtual ~JKISS_Generator() { };
};

#endif
