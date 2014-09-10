/*
 * Select_Grams.h
 *
 * Функция стоимости самых встречающихся (1,2,3)-грамм.
 *
 *      Author: eugene
 */

#ifndef SELECT_GRAMS_H_
#define SELECT_GRAMS_H_

#include "functions.h"
#include <unordered_map>

class SelectGramsFunc: public IFunctionCost {
private:
	static const std::string NAME_FOR_FUNC;
	static constexpr double ALPHA = 0.2;
	static constexpr double BETA = 0.4;
	static constexpr double GAMA = 0.4;
	SelectGramsFunc() {};
protected:
	std::unordered_map<std::string, double> best_stat_table, cur_stat_table;
	void counting_statistics(const std::string &data,
			std::unordered_map<std::string, double> &table);
public:
	SelectGramsFunc(const std::string &,const double &minCost);
	virtual double getCost(const std::string &);
	~SelectGramsFunc() {};
};

#endif /* SELECT_GRAMS_H_ */
