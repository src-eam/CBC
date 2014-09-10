/*
 * Rubbish_Grams.h
 *
 *      Author: eugene
 */

#ifndef RUBBISH_GRAMS_H_
#define RUBBISH_GRAMS_H_

#include "functions.h"

class RubbishGramsFunc: public IFunctionCost {
private:
	static const std::string NAME_FOR_FUNC;
	static constexpr double ALPHA = 0.5;
public:
	virtual double getCost(const std::string &);
	explicit RubbishGramsFunc(const double &min_cost);
	RubbishGramsFunc();
	~RubbishGramsFunc() {};
};

#endif /* RUBBISH_GRAMS_H_ */
