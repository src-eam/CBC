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
#include <iterator>

const std::string SelectGramsFunc::NAME_FOR_FUNC = "Select_Grams_Cost";
const std::unordered_map<std::string, double> SelectGramsFunc::best_stat_table {
		{ "a", 6.18144 }, { "th", 2.30197 }, { "b", 1.12362 }, { "c", 1.77244 },
		{ "ou", 0.784988 }, { "d", 3.49011 }, { "he", 2.30415 }, { "e", 9.561 },
		{ "z", 0.0488463 }, { "f", 1.7271 }, { "se", 0.558328 },
		{ "h", 5.24444 }, { "at", 0.776603 }, { "has", 0.0249341 }, { "an",
				1.368 }, { "ti", 0.394206 }, { "for", 0.263932 }, { "to",
				0.709508 }, { "i", 5.29629 }, { "and", 0.785912 }, { "nd",
				1.03411 }, { "w", 1.84525 }, { "hi", 0.787685 }, { "et",
				0.259216 }, { "ing", 0.557355 }, { "as", 0.609058 }, { "ere",
				0.246583 }, { "p", 1.27255 }, { "the", 1.3855 }, { "is",
				0.649162 }, { "or", 0.658594 }, { "s", 4.80226 }, { "in",
				1.38742 }, { "of", 0.548502 }, { "v", 0.806684 },
		{ "n", 5.34878 }, { "ion", 0.174379 }, { "re", 1.01125 }, { "ed",
				0.817335 }, { "tha", 0.254673 }, { "ea", 0.470375 }, { "it",
				0.702465 }, { "y", 1.54251 }, { "nt", 0.432105 }, { "ha",
				0.827493 }, { "r", 4.39744 }, { "ar", 0.532261 }, { "te",
				0.559854 }, { "q", 0.0830522 }, { "her", 0.459785 }, { "es",
				0.599096 }, { "men", 0.0787919 }, { "o", 5.79671 }, { "x",
				0.117566 }, { "me", 0.481666 }, { "m", 1.93219 }, { "er",
				1.34524 }, { "u", 2.10113 }, { "l", 3.01412 }, { "g", 1.51352 },
		{ "t", 6.86324 }, { "k", 0.600402 }, { "on", 0.820647 }, { "was",
				0.213806 }, { "j", 0.118588 } };

void SelectGramsFunc::counting_statistics(const std::string &data) {
	int size_data = data.length();
	std::string grams = "";
	char prom_char;
	for (int index = 0; index < size_data; index++) {
		grams = "";
		prom_char = tolower(data[index]);
		if (prom_char >= 'a' && prom_char <= 'z') {
			grams += prom_char;
			if (cur_stat_table.count(grams) > 0) {
				cur_stat_table[grams]++;
			}
			if ((index + 1) < size_data) {
				prom_char = tolower(data[index + 1]);
				if (prom_char >= 'a' && prom_char <= 'z') {
					grams += prom_char;
					if (cur_stat_table.count(grams) > 0) {
						cur_stat_table[grams]++;
					}
					if ((index + 2) < size_data) {
						prom_char = tolower(data[index + 2]);
						if (prom_char >= 'a' && prom_char <= 'z') {
							grams += prom_char;
							if (cur_stat_table.count(grams) > 0) {
								cur_stat_table[grams]++;
							}
						}
					}
				}
			}
		}
	}
	int len;
	for (auto iter = cur_stat_table.begin(); iter != cur_stat_table.end(); iter++) {
		len = (*iter).first.length();
		(*iter).second *= 100;
		if (len == 1)
			(*iter).second /= size_data;
		else if (len == 2 )
			(*iter).second /= (size_data -1);
		else
			(*iter).second /= (size_data - 2);
	}
}
SelectGramsFunc::SelectGramsFunc() {
	nameFunc = NAME_FOR_FUNC;
	MIN_COST = 0;
	std::unordered_map<std::string, double>::const_iterator it;
	for (it = best_stat_table.begin(); it != best_stat_table.end(); it++) {
		cur_stat_table[(*it).first] = 0.0;
	}
}

double SelectGramsFunc::getCost(const std::string &txt) {
	this->counting_statistics(txt);
	double unigrams = 0, digrams = 0, trigrams = 0;
	int len;
	double sub = 0;
	std::unordered_map<std::string, double>::const_iterator it;
	for (it = best_stat_table.begin(); it != best_stat_table.end(); it++) {
		len = (*it).first.length();
		sub = (*it).second - cur_stat_table[(*it).first];
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

void SelectGramsFunc::frequency_language(const std::string & inFile,
		const std::string & outFile) {
	std::ifstream input(inFile);
	if (!input.is_open()) {
		std::cerr << "Error opening the input file... " << inFile << std::endl;
		exit(EXIT_FAILURE);
	}
	input.unsetf(std::ios::skipws);
	std::istream_iterator<char> eos;
	std::string data(std::istream_iterator<char>(input), eos);
	input.close();
	this->counting_statistics(data);
	std::ofstream output(outFile);
	if (!output.is_open()) {
		std::cerr << "Error opening the output file: " << outFile << std::endl;
		exit(EXIT_FAILURE);
	}
	for (auto it = cur_stat_table.begin(); it != cur_stat_table.end(); it++) {
		output << "{\"" <<(*it).first << "\", " << (*it).second << "}," << std::endl;
		(*it).second = 0;
	}
	output.close();
}
