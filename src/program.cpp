#include <iostream>
#include <iterator>
#include "generators/DEV_Generator.h"
#include "functions/Select_Grams.h"
#include "functions/Rubbish_Grams.h"
#include "pso/pso.h"
#include "decrypt/DecryptSAES.h"
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

	IDecrypt *decryptApi = new DecryptSAES(data);

	IFunctionCost *funcCost = new RubbishGramsFunc();
	/*IFunctionCost *funcCost =
			new SelectGramsFunc(
					"/home/eugene/workspace/CryptE/src/system_data/select_statistics.dat",
					0);*/
	funcCost->setMinCost(funcCost->getCost(decryptApi->decrypt(KEY)));

	IRandomGenerator *gen = new JKISS_Generator();
	//cout << funcCost->getCost(decryptApi->decrypt(KEY) << endl;
/*	IPSO *pso = new OriginalPSO(decryptApi, funcCost, gen,
			"/home/eugene/workspace/CryptE/report.txt",
			//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
			"");*/
	IPSO *pso = new TasgetirenPSO(decryptApi, funcCost, gen,
	 "/home/eugene/workspace/CryptE/report.txt",
	 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
	 "");

	/*IPSO *pso = new ModifiedPSO(decryptApi, funcCost, gen,
	 "/home/eugene/workspace/CryptE/report.txt",
	 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
	 "");*/
	/*IPSO *pso = new NovelPSO(decryptApi, funcCost, gen,
	 "/home/eugene/workspace/CryptE/report.txt",
	 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
	 "");*/
	pso->printInit(fileOut);
	pso->printInit(cout);
	unsigned int count_iter = pso->attacking_pso();
	Particle g_best = pso->getBestParticle();

	printResult(fileOut, pso, count_iter, KEY);
	printResult(cout, pso, count_iter, KEY);
	//cout << decryptApi->decrypt(g_best.getValueParticle());

	delete decryptApi;
	delete funcCost;
	delete gen;
	delete pso;

	fileOut.close();

	return 0;
}
