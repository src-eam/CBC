#include <iostream>
#include <iterator>
#include "pso/pso.h"
#include "pso/original_pso.h"
#include "pso/tasgetiren_pso.h"
#include "pso/modified_pso.h"
#include "pso/novel_pso.h"
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <ctime>
#include <iomanip>
#include <sstream>

using namespace std;

int semid;
static struct sembuf sem_lock[2] = { { 0, 0, 0 }, { 0, 1, 0 } };
static struct sembuf sem_unlock[1] = { 0, -1, 0 };

void setSemaphore() {
	key_t key;
	if ((key = ftok("/home/eugene/workspace/ExecExper/src/ExecExper.cpp", 1)) //todo
	< 0) {
		perror("ftok");
		exit(1);
	}
	semid = semget(key, 1, 0666 | IPC_EXCL);
	if (semid < 0) {
		perror("semget");
		exit(1);
	}
}

/*double analis_key(Particle &gbest, const vector<uint8_t> &key) {
 double succ = 0;
 uint8_t tmp;
 unsigned int j, ind = 0;
 for (auto iter = key.begin(); iter != key.end(); iter++) {
 tmp = *iter;
 j = 8;
 while (j--) {
 if (gbest.getIndParticle(ind++) == (tmp & 1)) {
 succ++;
 tmp >>= 1;
 }
 }
 }
 return (succ / gbest.getDim()) * 100;
 }

 void printResult(ostream &out, IPSO *&pso, const unsigned int &count_iter,
 const vector<uint8_t> &key) {
 pso->printInit(out);
 Particle g_best = pso->getBestParticle();
 out << "COST_RESULT: " << g_best.getCost() << endl;
 out << "SEARCH_KEYS: " << pso->getSearchKeys() << endl;
 out << "ITER_COUNT: " << count_iter << endl;
 double succ = analis_key(g_best, key);
 (succ == 100) ?
 out << "OK!! Successive bits: 100 percent.\n" :
 out << "FAIL!! Successive bits: " << succ << " percent.\n";
 out << "KEY_RESULT: " << g_best.getValueParticle() << endl << endl;
 out << "----------------------------------------------------------\n\n";
 }*/

int main(int argc, char *argv[]) {
	if (argc < 4) {
		cerr << "Error input arguments.\n" << argc << endl;
		exit(EXIT_FAILURE);
	}
	setSemaphore();
	std::string dataFile = string(argv[2]);

	string reportFile = string(argv[3]);

	ifstream data_file(dataFile);

	if (!data_file.is_open()) {
		cerr << "Error opening the data file: " << dataFile << endl;
		exit(EXIT_FAILURE);
	}
	data_file.unsetf(ios::skipws);
	std::istream_iterator<char> eos;
	string data(istream_iterator<char>(data_file), eos);
	data_file.close();
	ofstream fileOut(reportFile, ios::app);
	if (!fileOut.is_open()) {
		cerr << "Error opening the report file: " << reportFile << endl;
		exit(EXIT_FAILURE);
	}

	ifstream config(
			"/home/eugene/workspace/CryptE/src/system_data/configPSO.dat");
	if (!config.is_open()) {
		cerr << "Error opening the config file: configPSO.dat." << endl;
		exit(EXIT_FAILURE);
	}

	string keyFile = string(argv[1]);
	ifstream kf(keyFile);
	if (!kf.is_open()) {
		cerr << "Error opening the key file: " << keyFile << endl;
		exit(EXIT_FAILURE);
	}

	vector<uint8_t> key_true;

	string str;
	string randName, psoName, keyTrueString = "";
//	IRandomGenerator *gen;
	IDecrypt *decryptApi;
	IFunctionCost *funcCost;
//	IPSO *pso;
	unsigned int population = 10, iteration = 2000, launches = 1;

	while (!config.eof()) {
		config >> str;
		if (str == "CIPHER:") {
			config >> str;
			if (str == "AES") {
				decryptApi = new DecryptAES(data);
			}
			else if (str == "SAES") {
				decryptApi = new DecryptSAES(data);
			}
			else {
				cerr << "Error config data. Algorithm undefined.\n";
				config.close();
				fileOut.close();
				kf.close();
				exit(EXIT_FAILURE);
			}
			unsigned int count_byte_key = decryptApi->getDimDecrypt() / 8, tmp;
			key_true.resize(count_byte_key);
			stringstream stream;
			for (unsigned int ind = 0; ind < count_byte_key; ind++) {
				kf >> hex >> tmp;
				stream << std::hex << std::setfill('0') << std::setw(2) << tmp;
				key_true[ind] = (tmp & 0xff);
			}
			keyTrueString = stream.str();
			kf.close();
		}
		else if (str == "RAND:") {
			config >> randName;
			if (randName == "ANY") {
				DEV_Generator rnG("/dev/urandom");
				unsigned int randGen = rnG.getRandom() % 3;
				randName = (randGen == 0) ? "JKISS" :
							(randGen == 1) ? "MersenneTwister" : "Fibonacci60";
			}
			/*if (str == "JKISS") {
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
			 }*/
		}
		else if (str == "COST_FUNC:") {
			config >> str;
			if (str == "Rubbish_Grams_Cost") {
				funcCost = new RubbishGramsFunc();
			}
			else if (str == "Select_Grams_Cost") {
				funcCost = new SelectGramsFunc();
			}
			else {
				cerr << "Error config data. Function undefined.\n";
				config.close();
				delete decryptApi;
				fileOut.close();
				exit(EXIT_FAILURE);
			}
			funcCost->setMinCost(
					funcCost->getCost(decryptApi->decrypt(key_true)));
		}
		else if (str == "INITIAL_POPULATION:") {
			config >> population;
		}
		else if (str == "ALGORITHM:") {
			config >> psoName;
			/*if (str == "Original_PSO") {
			 pso = new OriginalPSO(decryptApi, funcCost, gen, "",
			 //"/home/eugene/workspace/CryptE/report.txt",
			 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
			 "", population);
			 }
			 else if (str == "Tasgetiren_PSO") {
			 pso = new TasgetirenPSO(decryptApi, funcCost, gen, "",
			 //"/home/eugene/workspace/CryptE/report.txt",
			 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
			 "", population);
			 }
			 else if (str == "Modified_PSO") {
			 pso = new ModifiedPSO(decryptApi, funcCost, gen, "",
			 //"/home/eugene/workspace/CryptE/report.txt",
			 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
			 "", population);
			 }
			 else if (str == "Novel_PSO") {
			 pso = new NovelPSO(decryptApi, funcCost, gen, "",
			 //"/home/eugene/workspace/CryptE/report.txt",
			 //"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
			 "", population);
			 }*/
		}
		else if (str == "NUMBER_OF_ITERATION:") {
			config >> iteration;
			//pso->setIteration(iteration);
		}
		else if (str == "NUMBER_OF_LAUNCHES:") {
			config >> launches;
		}
	}
	config.close();
	IRandomGenerator *gen;
	IPSO *pso;
	Particle g_best;
	unsigned int count_iter = 0, search_keys = 0;
	bool success = false;
	string printStr;
	for (unsigned int start = 0; start < launches; start++) {
		if (randName == "JKISS")
			gen = new JKISS_Generator();
		else if (randName == "MersenneTwister")
			gen = new MersenneTwister_Generator();
		else if (randName == "Fibonacci60")
			gen = new Fibonacci_Generator();
		else if (randName == "/dev/urandom")
			gen = new DEV_Generator("/dev/urandom");
		else {
			delete decryptApi;
			delete funcCost;
			fileOut.close();
			exit(EXIT_FAILURE);
		}
		if (psoName == "Original_PSO")
			pso = new OriginalPSO(decryptApi, funcCost, gen, "",
			//"/home/eugene/workspace/CryptE/report.txt",
			//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt",
					"", population);

		else if (psoName == "Tasgetiren_PSO")
			pso = new TasgetirenPSO(decryptApi, funcCost, gen, "",
			//"/home/eugene/workspace/CryptE/report.txt",
			//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
					"", population);
		else if (psoName == "Modified_PSO")
			pso = new ModifiedPSO(decryptApi, funcCost, gen, "",
			//"/home/eugene/workspace/CryptE/report.txt",
			//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
					"", population);
		else if (psoName == "Novel_PSO")
			pso = new NovelPSO(decryptApi, funcCost, gen, "",
			//"/home/eugene/workspace/CryptE/report.txt",
			//"/home/eugene/workspace/CryptE/src/system_data/gen_key.txt");
					"", population);
		else {
			delete decryptApi;
			delete funcCost;
			delete gen;
			fileOut.close();
			exit(EXIT_FAILURE);
		}
		pso->setIteration(iteration);
		if (start == 0) {
			printStr = "CIPHER: " + decryptApi->getCipherName()
					+ "\nALGORITHM: " + pso->getAlgName() + "\n"
					+ pso->getParameters() + "INITIAL_POPULATION: "
					+ to_string(pso->getInitPopulation())
					+ "\nNUMBER_OF_ITERATION: "
					+ to_string(pso->getIterations()) + "\nNUMBER_OF_LAUNCHES: "
					+ to_string(launches) + "\nRAND: " + gen->getNameGenerator()
					+ "\nCOST_FUNC: " + funcCost->getNameFunc() + "\nKEY_TRUE: "
					+ keyTrueString + "\nDATA_FILE: " + dataFile
					+ "\nDATA_LENGTH: " + to_string(decryptApi->getLengthData())
					+ "\nKEYS_INIT:\n";
		}
		for (unsigned int index = 0; index < pso->getInitPopulation();
				index++) {
			printStr += pso->getParticle(index).getValueParticle() + " ";
		}
		printStr += ";\n";
		count_iter += pso->attacking_pso();
		g_best = pso->getBestParticle();
		search_keys += pso->getSearchKeys();
		if (g_best.getValueParticle() == keyTrueString) {
			success = true;
			delete gen;
			delete pso;
			break;
		}
//		setSemaphore();
//		semop(semid, &sem_lock[0], 2);
//		printResult(fileOut, pso, count_iter, key_true);
//		printResult(cout, pso, count_iter, key_true);
//		semop(semid, &sem_unlock[0], 1);
		delete gen;
		delete pso;
	}
	printStr += "\nCOST_RESULT: " + to_string(g_best.getCost())
			+ "\nITER_COUNT: " + to_string(count_iter) + "\nSEARCH_KEYS: "
			+ to_string(search_keys) + "\nTIME: "
			+ to_string((double) clock() / CLOCKS_PER_SEC);
	printStr += (success) ? "\nOK!!! " : "\nFAIL!!! ";
	printStr += "KEY_RESULT: " + g_best.getValueParticle()
			+ "\n---------------------\n\n";
	semop(semid, &sem_lock[0], 2);
	//cout << printStr;
	fileOut << printStr;
	semop(semid, &sem_unlock[0], 1);
//	unsigned int count_iter = pso->attacking_pso();
//	Particle g_best = pso->getBestParticle();
//	setSemaphore();
//	semop(semid, &sem_lock[0], 2);
//	printResult(fileOut, pso, count_iter, key_true);
//	printResult(cout, pso, count_iter, key_true);
//	semop(semid, &sem_unlock[0], 1);

	delete decryptApi;
	delete funcCost;
//	delete gen;
//	delete pso;

	fileOut.close();
//	SelectGramsFunc qq;
//	qq.frequency_language("/home/eugene/workspace/CryptE/src/system_data/anna_karenina.txt","/home/eugene/workspace/CryptE/src/system_data/select_statistics_2.txt");

	return 0;
}
