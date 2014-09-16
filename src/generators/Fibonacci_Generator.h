/*
 * Fibonacci_Generator.h
 *
 *      Author: eugene
 */

#ifndef FIBONACCI_GENERATOR_H_
#define FIBONACCI_GENERATOR_H_

#include "generators.h"
#include <boost/random/lagged_fibonacci.hpp>

class Fibonacci_Generator : public IRandomGenerator {
private:
	static const std::string name;
	boost::lagged_fibonacci607 *gen;
public:
	Fibonacci_Generator();
	virtual int getRandom();
	virtual double getMinMaxRandom(const int &min, const int &max);
	virtual double getRandom_01();
	virtual std::string getNameGenerator() const;
	virtual ~Fibonacci_Generator();
};



#endif /* FIBONACCI_GENERATOR_H_ */
