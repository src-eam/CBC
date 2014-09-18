#include <iostream>
#include <iterator>
#include "pso/pso.h"
#include "pso/original_pso.h"
#include "pso/tasgetiren_pso.h"
#include "pso/modified_pso.h"
#include "pso/novel_pso.h"

using namespace std;

double analis_key(Particle &gbest, unsigned int key) {
	double succ = 0;
	unsigned int res_key = gbest.getValueParticle();
	for (unsigned int ind = 0; ind < gbest.getDim(); ind++) {
		if ((res_key & 1) == (key & 1)) {
			succ++;
		}
		res_key >>= 1;
		key >>= 1;
	}
	return (succ / gbest.getDim()) * 100;
}

void printResult(ostream &out, IPSO *&pso, const unsigned int &count_iter,
		const unsigned int &KEY) {
	Particle g_best = pso->getBestParticle();
	unsigned int key_result = g_best.getValueParticle();
	out << "COST_RESULT: " << g_best.getCost() << endl;
	out << "SEARCH_KEYS: " << pso->getSearchKeys() << endl;
	out << "ITER_COUNT: " << count_iter << endl;
	(key_result == KEY) ?
			out << "OK!! Successive bits: 100 percent.\n" :
			out << "FAIL!! Successive bits: " << analis_key(g_best, KEY)
					<< " percent.\n";
	out << "KEY_RESULT: " << key_result << endl << endl;
	out << "----------------------------------------------------------\n\n";
}

int main(int argc, char *argv[]) {
	if (argc != 4) {
		cerr << "Error input arguments.\n";
		exit(EXIT_FAILURE);
	}

	std::string dataFile = string(argv[2]);
	unsigned int KEY = atoi(argv[1]);

	string reportFile = string(argv[3]);

	ifstream data_file(dataFile);

	if (!data_file.is_open()) {
		cerr << "Error opening data file: " << dataFile << endl;
		exit(EXIT_FAILURE);
	}
	data_file.unsetf(ios::skipws);
	std::istream_iterator<char> eos;
	string data(istream_iterator<char>(data_file), eos);
	data_file.close();
	ofstream fileOut(reportFile, ios::app);
	if (!fileOut.is_open()) {
		cerr << "Error opening report file: " << reportFile << endl;
		exit(EXIT_FAILURE);
	}

	ifstream config(
			"/home/eugene/workspace/CryptE/src/system_data/configPSO.dat");
	if (!config.is_open()) {
		cerr << "Error opening config file: configPSO.dat." << endl;
		exit(EXIT_FAILURE);
	}

	string str;

	IDecrypt *decryptApi = new DecryptSAES(data);
	IRandomGenerator *gen;
	IFunctionCost *funcCost;
	IPSO *pso;

	while (!config.eof()) {
		config >> str;
		if (str == "RAND:") {
			config >> str;
			if (str == "JKISS") {
				gen = new JKISS_Generator();
			}
			else if (str == "MersenneTwister") {
				gen = new MersenneTwister_Generator();
			}
			else if (str == "Fibonacci60") {
				gen = new Fibonacci_Generator();
			}
			else if (str == "/dev/urandom") {
				gen = new DEV_Generator("/dev/urandom");
			}
			else {
				cerr << "Error config data. Generator undefined.\n";
				config.close();
				delete decryptApi;
				fileOut.close();
				exit(EXIT_FAILURE);
			}
		}
		else if (str == "COST_FUNC:") {
			config >> str;
			if (str == "Rubbish_Grams_Cost") {
				funcCost = new RubbishGramsFunc();
			}
			else if (str == "Select_Grams_Cost") {
				funcCost =
						new SelectGramsFunc(
								"/home/eugene/workspace/CryptE/src/system_data/select_statistics.dat",
								0);
			}
			else {
				cerr << "Error config data. Function undefined.\n";
				config.close();
				delete decryptApi;
				delete gen;
				fileOut.close();
				exit(EXIT_FAILURE);
			}
			funcCost->setMinCost(funcCost->getCost(decryptApi->decrypt(KEY)));
		}
		else if (str == "ALGORITHM:") {
			config >> str;
			if (str == "Original_PSO") {
				pso = new OriginalPSO(decryptApi, funcCost, gen,
						"/home/eugene/workspace/CryptE/report.txt",
						//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
						"");
			}
			else if (str == "Tasgetiren_PSO") {
				pso = new TasgetirenPSO(decryptApi, funcCost, gen,
						"/home/eugene/workspace/CryptE/report.txt",
						//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
						"");
			}
			else if (str == "Modified_PSO") {
				pso = new ModifiedPSO(decryptApi, funcCost, gen,
						"/home/eugene/workspace/CryptE/report.txt",
						//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
						"");
			}
			else if (str == "Novel_PSO") {
				pso = new NovelPSO(decryptApi, funcCost, gen,
						"/home/eugene/workspace/CryptE/report.txt",
						//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
						"");
			}
			else {
				cerr << "Error config data. Algorithm undefined.\n";
				config.close();
				delete decryptApi;
				delete funcCost;
				delete gen;
				fileOut.close();
				exit(EXIT_FAILURE);
			}
		}
	}
	config.close();

	pso->printInit(fileOut);
	pso->printInit(cout);
	unsigned int count_iter = pso->attacking_pso();
	Particle g_best = pso->getBestParticle();

	printResult(fileOut, pso, count_iter, KEY);
	printResult(cout, pso, count_iter, KEY);

	delete decryptApi;
	delete funcCost;
	delete gen;
	delete pso;

	fileOut.close();

	return 0;
}
