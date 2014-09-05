/*
 * select_grams.cpp
 *
 *  Created on: 02 сент. 2014 г.
 *      Author: eugene
 */

#include "Select_Grams.h"
#include <fstream>
#include <iostream>
#include <math.h>

const std::string SelectGramsFunc::NAME_FOR_FUNC = "Select_Grams_Cost";

void SelectGramsFunc::counting_statistics(const std::string &data,
		std::unordered_map<std::string, double> &table) {
	int size_data = data.length();
	double unigram_count = 0, digram_count = 0, trigram_count = 0;
	std::string grams = "";
	char prom_char;
	for (int index = 0; index < size_data; index++) {
		grams = "";
		prom_char = tolower(data[index]);
		if (prom_char >= 'a' && prom_char <= 'z') {
			grams += prom_char;
			if (table[grams] > 0) {
				table[grams]++;
				unigram_count++;
			} else {
				table[grams] = 1;
				unigram_count++;
			}
			if ((index + 1) < size_data) {
				prom_char = tolower(data[index + 1]);
				if (prom_char >= 'a' && prom_char <= 'z') {
					grams += prom_char;
					if (table[grams] > 0) {
						table[grams]++;
						digram_count++;
					} else {
						table[grams] = 1;
						digram_count++;
					}
					if ((index + 2) < size_data) {
						prom_char = tolower(data[index + 2]);
						if (prom_char >= 'a' && prom_char <= 'z') {
							grams += prom_char;
							if (table[grams] > 0) {
								table[grams]++;
								trigram_count++;
							} else {
								table[grams] = 1;
								trigram_count++;
							}

						}
					}
				}
			}
		}
	}
	std::unordered_map<std::string, double>::iterator iter;
	int len;
	for (iter = table.begin(); iter != table.end(); iter++) {
		len = (*iter).first.length();
		(*iter).second *= 100;
		if (len == 1 && unigram_count > 0)
			(*iter).second /= unigram_count;
		else if (len == 2 && digram_count > 0)
			(*iter).second /= digram_count;
		else if (trigram_count > 0)
			(*iter).second /= trigram_count;
	}
}
SelectGramsFunc::SelectGramsFunc(const std::string & filename) {
	nameFunc = NAME_FOR_FUNC;
	std::ifstream ifs;
	ifs.open(filename);
	if (!ifs.is_open()) {
		std::cerr << "Error opening file: " << filename << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string key_str;
	double value_stat;
	while (!ifs.eof()) {
		ifs >> key_str;
		ifs >> value_stat;
		best_stat_table[key_str] = value_stat;
		cur_stat_table[key_str] = 0;
	}
	ifs.close();
}

double SelectGramsFunc::getCost(const std::string &txt) {
	this->counting_statistics(txt, cur_stat_table);
	std::cout<< cur_stat_table["t"]<<" "<<ALPHA<<std::endl;
	double unigrams = 0, digrams = 0, trigrams = 0;
	int len;
	double sub = 0;
	std::unordered_map<std::string, double>::const_iterator it;
	for (it = best_stat_table.begin(); it != best_stat_table.end(); it++) {
		len = (*it).first.length();
		sub = (cur_stat_table.count((*it).first) > 0) ?
				best_stat_table[(*it).first] - cur_stat_table[(*it).first] :
				best_stat_table[(*it).first];
		if (len == 1)
			unigrams += fabs(sub);
		else if (len == 2)
			digrams += fabs(sub);
		else if (len == 3)
			trigrams += fabs(sub);
		 cur_stat_table[(*it).first] = 0;
	}
	return ALPHA * unigrams + BETA * digrams + GAMA * trigrams;
}
