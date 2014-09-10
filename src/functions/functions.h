/*
 * Интерфейс для функций стоимости.
 *      Author: eugene
 */

#ifndef FUNCTIONS_H_
#define FUNCTIONS_H_

#include <string>
#include <map>

class IFunctionCost {
protected:
	std::string nameFunc;
	double MIN_COST;
public:
	std::string getNameFunc() const {
		return this->nameFunc;
	}
	;
	virtual double getCost(const std::string &) = 0;
	double getMinCost() {
		return MIN_COST;
	}
	void setMinCost(const double &cost)  {
		MIN_COST = cost;
	}
	virtual ~IFunctionCost() {};
};

#endif /* FUNCTIONS_H_ */
