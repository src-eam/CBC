/*
 * rubbush_grams.cpp
 *
 *      Author: eugene
 */

#include "Rubbish_Grams.h"

const std::string RubbishGramsFunc::NAME_FOR_FUNC = "Rubbish_Grams_Cost";

RubbishGramsFunc::RubbishGramsFunc(const double &min_cost) {
	nameFunc = NAME_FOR_FUNC;
	MIN_COST = min_cost;
}

RubbishGramsFunc::RubbishGramsFunc() {
	nameFunc = NAME_FOR_FUNC;
	MIN_COST = 0;
}

double RubbishGramsFunc::getCost(const std::string &txt) {
	double result = 0;
	char prom_char;
	int len_txt = txt.length();
	for (int index = 0; index < len_txt; index++) {
		prom_char = txt[index];
		if (!(prom_char == 0 || prom_char == '\n' || ((prom_char >= ' ') && (prom_char <= '~')))) {
			result++;
		}
	}
	return this->ALPHA * (result * 100) / len_txt;
}

