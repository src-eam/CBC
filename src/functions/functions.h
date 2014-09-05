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
public:
	std::string getNameFunc() const{
		return this->nameFunc;
	};
	virtual double getCost(const std::string &) = 0;
	virtual ~IFunctionCost() {};
};

#endif /* FUNCTIONS_H_ */
