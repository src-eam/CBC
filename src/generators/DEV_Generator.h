/*
 * DEV_Generator.h
 *
 *  Created on: 21 июля 2014 г.
 *      Author: root
 */

#ifndef DEV_GENERATOR_H_
#define DEV_GENERATOR_H_

#include "generators.h"
#include <tr1/random>

class DEV_Generator : public IRandomGenerator {
private:
	std::tr1::random_device *dev;
	std::string name;
public:
	DEV_Generator(const std::string &);
	DEV_Generator();
	virtual int getRandom();
	virtual double getMinMaxRandom(const int &min, const int &max);
	virtual double getRandom_01();
	virtual std::string getNameGenerator() const;
	virtual ~DEV_Generator();
};

#endif /* DEV_GENERATOR_H_ */
