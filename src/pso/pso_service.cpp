/*
 * pso_service.cpp
 *
 *      Author: eugene
 */

#include "pso.h"
#include <math.h>
#include <iomanip>
#include <sstream>

Particle::Particle(const unsigned int &dim) {
	this->dim = dim;
	this->value_particle = "";
	this->cost = 0;
	this->particle.resize(dim);
	this->velocity.resize(dim);
}

Particle::Particle() {
	this->dim = 1;
	this->value_particle = "";
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

void Particle::setValueParticle(const std::string &value) {
	this->value_particle = value;
}

std::string Particle::getValueParticle() const {
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

/*unsigned int IPSO::masbin_to_int(Particle *&p) const {
	unsigned int res = 0, pow = 1;
	for (unsigned int i = 0; i < dim; i++) {
		res += p->getIndParticle(i) * pow;
		pow <<= 1;
	}
	return res;
}*/

void IPSO::convert_binary_key_to_hex(Particle *&p,
		std::vector<uint8_t> &key) const {
	std::string result = "";
	uint8_t tmp_uint, pow;
	unsigned int ind, j,key_ind = 0;
	std::stringstream stream;
	for (unsigned int i = 0; i < dim; i += 8) {
		tmp_uint = 0, pow = 1;
		ind = i, j = 8;
		while (j--) {
			tmp_uint += p->getIndParticle(ind++) * pow;
			pow <<= 1;
		}
		key[key_ind++] = tmp_uint;
		stream << std::hex << std::setfill('0') << std::setw(2)
				<< (unsigned int) tmp_uint;
	}
	p->setValueParticle(stream.str());
}

void IPSO::convert_key_to_binary(const std::vector<uint8_t> &key,
		Particle &p) const {
	uint8_t tmp;
	unsigned int index = 0;
	std::stringstream stream;
	for (auto iter = key.begin(); iter != key.end(); iter++) {
		tmp = *iter;
		stream << std::hex << std::setfill('0') << std::setw(2)
						<< (unsigned int) tmp;
		for (unsigned int i = 0; i < 8; i++) {
			p.setIndParticle(index++,(tmp & 1));
			tmp >>= 1;
		}
	};
	p.setValueParticle(stream.str());
}

double IPSO::compute_cost_value(const std::vector<uint8_t> &key) {
	std::string decryptTxt;
	decryptAlg->decrypt(decryptTxt, key);
	search_keys++;
	return funcCost->getCost(decryptTxt);
}

void IPSO::generateSwarmKeys(std::vector<std::vector<uint8_t>> &keysGen,const std::string &fileKeys) {
	unsigned int count_bytes = dim / 8;
	if (fileKeys == "") {
		for (unsigned int index = 0; index < INITIAL_POPULATION; index++) {
			keysGen[index].resize(count_bytes);
			decryptAlg->getPossibleKey(keysGen[index]);
		}
	}
	else {
		std::ifstream ifs;
		ifs.open(fileKeys);
		if (!ifs.is_open()) {
			std::cerr << "Error opening file: " << fileKeys << std::endl;
			exit(EXIT_FAILURE);
		}
		unsigned int tmp;
		for (unsigned int index = 0; index < INITIAL_POPULATION && !ifs.eof(); index++) {
			keysGen[index].resize(count_bytes);
			for (unsigned int ind = 0; ind < count_bytes; ind++) {
				ifs >> std::hex >> tmp;
				keysGen[index][ind] = tmp & 0xff;
			}
		}
		ifs.close();
	}
}

void IPSO::init_pso(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG,const std::string &outFile,const unsigned int &population) {
	this->INITIAL_POPULATION = population;
	this->decryptAlg = decrypt;
	this->funcCost = func;
	this->randGenAlg = randG;
	this->dim = decryptAlg->getDimDecrypt();
	swarm.reserve(INITIAL_POPULATION);
	p_best.reserve(INITIAL_POPULATION);
	//key_cost.resize(1 << this->dim,-1);
	search_keys = 0;
//	output.open(outFile);
//	if (!output.is_open()) {
//		std::cerr << "Error opening file : report.txt" << std::endl;
//		exit(EXIT_FAILURE);
//	}
}
void IPSO::generate_particles(const std::string &fileKeys) {
	std::vector<std::vector<uint8_t>> keysGen(INITIAL_POPULATION);
	generateSwarmKeys(keysGen, fileKeys);
	double cur_cost = 0, best_cost = UINT32_MAX;
	int pos_index = 0;
	for (unsigned int index = 0; index < INITIAL_POPULATION; index++) {
		Particle newParticle(dim);
		convert_key_to_binary(keysGen[index], newParticle);
		cur_cost = compute_cost_value(keysGen[index]);
		newParticle.setCost(cur_cost);
		key_cost[newParticle.getValueParticle()] = cur_cost;
		if (cur_cost < best_cost) {
			pos_index = index;
			best_cost = cur_cost;
		}
		for (unsigned int j = 0; j < dim; j++) {
			newParticle.setIndVelocity(j, randGenAlg->getMinMaxRandom(-4, 4));
		}
		swarm.push_back(newParticle);
		p_best.push_back(newParticle);
	}
	//search_keys = INITIAL_POPULATION;
	g_best = p_best[pos_index];
}

/*void IPSO::printInit(std::ostream & os) {
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
	std::vector<std::string>::const_iterator it;
	for(it = keysInit.begin(); it != keysInit.end(); it++){
		os << (*it) << std::endl;
	}
	//os << "G_BEST_INIT: " << g_best << "\n\n";
}*/

void IPSO::update_best_particle(Particle *&particle,
		const unsigned int &index) {
	std::vector<uint8_t> cur_key(dim / 8);
	convert_binary_key_to_hex(particle,cur_key);
	std::string cur_key_string = particle->getValueParticle();
	double cur_cost;
	if (key_cost.count(cur_key_string) > 0) {
		cur_cost = key_cost[cur_key_string];
	}
	else {
		cur_cost = compute_cost_value(cur_key);
		key_cost[cur_key_string] = cur_cost;
	}
	particle->setCost(cur_cost);
	if (cur_cost <= p_best[index].getCost()) {
		p_best[index] = *particle;
//		output << "P_BEST!! PARTICLE: " << index << ", " << *particle
//				<< ". SEARCH_KEYS: " << search_keys << std::endl;
		if (cur_cost <= g_best.getCost()) {
			g_best = *particle;
//			output << "G_BEST!! PARTICLE: " << index << " " << g_best
//					<< ". SEARCH_KEYS: " << search_keys << std::endl;
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
	unsigned int iterations = 0;
	double cost_min = funcCost->getMinCost();
	while ((iterations < NUMBER_OF_ITERATION) && (g_best.getCost() > cost_min)) {
		iterations++;
		update_particles();
//		std::cout << "G_BEST: " << g_best << ". ITER: " << iterations
//				<< ". SEARCH_KEYS: " << search_keys << std::endl;
	}
	return iterations;
}

void IPSO::setIteration(const unsigned int &iteration) {
	this->NUMBER_OF_ITERATION = iteration;
}

unsigned int IPSO::getInitPopulation() const {
	return INITIAL_POPULATION;
}

std::string IPSO::getParameters() const {
	std::string paramStr = "C1: " + std::to_string(C1) + "; C2: "
			+ std::to_string(C2) + "; INETRIA_WEIGHT: "
			+ std::to_string(INETRIA_WEIGHT) + "; R_MUT: "
			+ std::to_string(R_MUT) + "\n";
	return paramStr;
}

unsigned int IPSO::getIterations() const {
	return NUMBER_OF_ITERATION;
}

Particle IPSO::getParticle(const unsigned int & ind) const {
	return (ind < INITIAL_POPULATION) ?
			swarm[ind] : swarm[INITIAL_POPULATION - 1];
}

IPSO::~IPSO() {
	//output.close();
}
