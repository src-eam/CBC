/*
 * pso_service.cpp
 *
 *      Author: eugene
 */

#include "pso.h"
#include <math.h>

Particle::Particle(const unsigned int &dim) {
	this->dim = dim;
	this->value_particle = 0;
	this->cost = 0;
	this->particle.resize(dim);
	this->velocity.resize(dim);
}

Particle::Particle() {
	this->dim = 1;
	this->value_particle = 0;
	this->cost = 0;
}

double Particle::getCost() const {
	return this->cost;
}

void Particle::setCost(const double &cost) {
	this->cost = cost;
}

unsigned int Particle::getDim() const {
	return this->dim;
}

void Particle::setDim(const unsigned int &dim) {
	this->dim = dim;
	this->particle.resize(dim);
	this->velocity.resize(dim);
}

unsigned int Particle::getIndParticle(const unsigned int &index) const {
	return particle[index];
}

void Particle::setIndParticle(const unsigned int &index,
		const unsigned int & value) {
	particle[index] = value;
}

double Particle::getIndVelocity(const unsigned int &index) const {
	return velocity[index];
}

void Particle::setIndVelocity(const unsigned int &index, const double & value) {
	velocity[index] = value;
}

void Particle::setValueParticle(const unsigned int &value) {
	this->value_particle = value;
}

unsigned int Particle::getValueParticle() const {
	return this->value_particle;
}

std::ostream & operator <<(std::ostream & os, Particle const & particle) {
	os << "KEY: " << particle.getValueParticle() << ", COST: "
			<< particle.getCost();
	return os;
}

double IPSO::signum_function(const double &v) const {
	return 1 / (1 + (exp((-1) * v)));
}

unsigned int IPSO::masbin_to_int(Particle *&p) const {
	unsigned int res = 0, pow = 1;
	for (unsigned int i = 0; i < dim; i++) {
		res += p->getIndParticle(i) * pow;
		pow <<= 1;
	}
	return res;
}

double IPSO::compute_cost_value(const unsigned int &key) {
	std::string decryptTxt;
	decryptAlg->decrypt(decryptTxt, key);
	return funcCost->getCost(decryptTxt);
}

void IPSO::generateSwarmKeys(std::vector<unsigned int> &keysGen,const std::string &fileKeys) {
	if (fileKeys == "") {
		for (int index = 0; index < INITIAL_POPULATION; index++) {
			keysGen[index] = decryptAlg->getPossibleKey();
		}
	}
	else {
		std::ifstream ifs;
		ifs.open(fileKeys);
		if (!ifs.is_open()) {
			std::cerr << "Error opening file: " << fileKeys << std::endl;
			exit(EXIT_FAILURE);
		}
		for (int index = 0; index < INITIAL_POPULATION && !ifs.eof(); index++) {
			ifs >> keysGen[index];
		}
		ifs.close();
	}
}

void IPSO::init_pso(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG,const std::string &outFile) {
	std::ifstream paramFile(
			"/home/eugene/workspace/CryptE/src/system_data/parametrsPSO.dat"); //TODO не хорошо.
	if (!paramFile.is_open()) {
		std::cerr << "Error opening file : parametrsPSO.dat" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::string str;
	while (!paramFile.eof()) {
		paramFile >> str;
		if (str == "INITIAL_POPULATION:") {
			paramFile >> INITIAL_POPULATION;
		}
		else if (str == "NUMBER_OF_ITERATION:") {
			paramFile >> NUMBER_OF_ITERATION;
		}
		else if (str == "C1:") {
			paramFile >> C1;
		}
		else if (str == "C2:") {
			paramFile >> C2;
		}
		else if (str == "INETRIA_WEIGHT:") {
			paramFile >> INETRIA_WEIGHT;
		}
		else if (str == "R_MUT:") {
			paramFile >> R_MUT;
		}
	}
	paramFile.close();

	this->decryptAlg = decrypt;
	this->funcCost = func;
	this->randGenAlg = randG;
	this->dim = decryptAlg->getDimDecrypt();
	swarm.reserve(INITIAL_POPULATION);
	p_best.reserve(INITIAL_POPULATION);
	key_cost.resize(1 << this->dim,-1);
	search_keys = 0;
	output.open(outFile);
	if (!output.is_open()) {
		std::cerr << "Error opening file : report.txt" << std::endl;
		exit(EXIT_FAILURE);
	}
}
void IPSO::generate_particles(const std::string &fileKeys){
	std::vector<unsigned int> keysGen(INITIAL_POPULATION);
	generateSwarmKeys(keysGen,fileKeys);
	double cur_cost = 0, best_cost = UINT32_MAX;
	int cur_key, pos_index = 0;
	//DEV_Generator genD("/dev/urandom");
	for (int index = 0; index < INITIAL_POPULATION; index++) {
		cur_key = keysGen[index];
		cur_cost = compute_cost_value(cur_key);
		Particle newParticle(dim);
		newParticle.setValueParticle(cur_key);
		newParticle.setCost(cur_cost);
		if (cur_cost < best_cost) {
			pos_index = index;
			best_cost = cur_cost;
		}
		for (unsigned int j = 0; j < dim; j++) {
			newParticle.setIndParticle(j, cur_key & 1);
			cur_key >>= 1;
			//newParticle.setIndVelocity(j, genD.getMinMaxRandom(-4, 4));
			//newParticle.setIndVelocity(j, 0);
			newParticle.setIndVelocity(j, randGenAlg->getMinMaxRandom(-4, 4));
		}
		swarm.push_back(newParticle);
		p_best.push_back(newParticle);
	}
	g_best = p_best[pos_index];
}

void IPSO::printInit(std::ostream & os) {
	os << "ALGORITHM: " << getAlgName() << std::endl;
	os << "DATA_LENGTH: " << decryptAlg->getLengthData() << std::endl;
	os << "RAND: " << randGenAlg->getNameGenerator() << std::endl;
	os << "COST_FUNC: " << funcCost->getNameFunc() << std::endl;
	os << "INITIAL_POPULATION: " << INITIAL_POPULATION << std::endl;
	os << "NUMBER_OF_ITERATION: " << NUMBER_OF_ITERATION << std::endl;
	os << "INETRIA_WEIGHT: " << INETRIA_WEIGHT << std::endl;
	os << "C1: " << C1 << "\nC2: " << C2 << std::endl;
	os << "R_MUT: " << R_MUT << std::endl;
	os << "KEYS_INIT:" << std::endl;
	std::vector<Particle>::const_iterator it;
	for(it = swarm.begin(); it != swarm.end(); it++){
		os << (*it).getValueParticle() << std::endl;
	}
	os << "G_BEST_INIT: " << g_best << "\n\n";
}

void IPSO::update_best_particle(Particle *&particle,
		const unsigned int &index) {
	//output << "tt";
	particle->setValueParticle(masbin_to_int(particle));
	unsigned int cur_key = particle->getValueParticle();
	double cur_cost = key_cost[cur_key];
	if (cur_cost == -1) {
		cur_cost = compute_cost_value(cur_key);
		key_cost[cur_key] = cur_cost;
		search_keys++;
	}
	particle->setCost(cur_cost);
	if (cur_cost < p_best[index].getCost()) {
		p_best[index] = *particle;
		output << "P_BEST!! PARTICLE: " << index << ", " << *particle << std::endl;
		if (cur_cost < g_best.getCost()) {
			g_best = *particle;
			output << "G_BEST!! PARTICLE: " << index << " " << g_best<< std::endl;
		}
	}
}

std::string IPSO::getAlgName() const {
	return algName;
}

unsigned int IPSO::getSearchKeys() const {
	return search_keys;
}

Particle IPSO::getBestParticle() const {
	return g_best;
}

unsigned int IPSO::attacking_pso() {
	int iterations = 0;
	double cost_min = funcCost->getMinCost();
	while ((iterations < NUMBER_OF_ITERATION) && (g_best.getCost() > cost_min)) {
		iterations++;
		update_particles();
		std::cout << "G_BEST: " << g_best << ". ITER: " << iterations
				<< ". SEARCH_KEYS: " << search_keys << std::endl;
	}
	return iterations;
}

IPSO::~IPSO() {
	output.close();
}
