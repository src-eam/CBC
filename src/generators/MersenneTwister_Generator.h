/*
 * MersenneTwister_Generator.h
 *
 *      Author: eugene
 */

#ifndef MERSENNETWISTER_GENERATOR_H_
#define MERSENNETWISTER_GENERATOR_H_

#include "generators.h"
#include <boost/random/mersenne_twister.hpp>

class MersenneTwister_Generator: public IRandomGenerator {
private:
	static const std::string name;
	boost::mt11213b *gen;
public:
	MersenneTwister_Generator();
	virtual int getRandom();
	virtual double getMinMaxRandom(const int &min, const int &max);
	virtual double getRandom_01();
	virtual std::string getNameGenerator() const;
	virtual ~MersenneTwister_Generator();
};

#endif /* MERSENNETWISTER_GENERATOR_H_ */
