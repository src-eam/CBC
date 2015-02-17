/*
 * pso.h
 *
 *      Author: eugene
 */

#ifndef PSO_H_
#define PSO_H_

#include <vector>
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "../headersPSO.h"

class Particle {
protected:
	unsigned int dim;
	std::vector<unsigned int> particle;
	std::vector<double> velocity;
	std::string value_particle;
	double cost;
public:
	explicit Particle(const unsigned int &dim);
	Particle();
	unsigned int getIndParticle(const unsigned int &index) const;
	void setIndParticle(const unsigned int &index, const unsigned int &value);
	double getIndVelocity(const unsigned int &index) const;
	void setIndVelocity(const unsigned int &index, const double &value);
	std::string getValueParticle() const;
	void setValueParticle(const std::string & value);
	unsigned int getDim() const;
	virtual void setDim(const unsigned int &dim);
	double getCost() const;
	void setCost(const double &cost);
	friend std::ostream & operator <<(std::ostream & os,
			Particle const & particle);
	virtual ~Particle() {
	}
	;
};

class IPSO {
protected:
	unsigned int INITIAL_POPULATION;
	unsigned int NUMBER_OF_ITERATION = 2000;
	double C1;
	double C2;
	double INETRIA_WEIGHT;
	double R_MUT;
	unsigned int dim;
	IDecrypt *decryptAlg;
	IFunctionCost *funcCost;
	IRandomGenerator *randGenAlg;
	std::vector<Particle> swarm;
	std::vector<Particle> p_best;
	std::unordered_map<std::string, double> key_cost;
	Particle g_best;
	unsigned int search_keys = 0;
	//std::ofstream output;
	std::string algName;

	double signum_function(const double &v) const;
//	unsigned int masbin_to_int(Particle *&p) const;
	void convert_binary_key_to_hex(Particle *&p,
			std::vector<uint8_t> &key) const;
	void convert_key_to_binary(const std::vector<uint8_t> &key,
			Particle &p) const;
	double compute_cost_value(const std::vector<uint8_t> &key);
	virtual void update_particles() = 0;
	void generateSwarmKeys(std::vector<std::vector<uint8_t>> &keysGen,
			const std::string &fileKeys);
	void generate_particles(const std::string &fileKeys);
	void init_pso(IDecrypt *&decrypt, IFunctionCost *&func,
			IRandomGenerator *&randG, const std::string &outFile,
			const unsigned int &population);
	void update_best_particle(Particle *& particle, const unsigned int &index);
public:
	unsigned int  getInitPopulation() const;
	std::string getParameters() const;
	unsigned int getIterations() const;
	//void printInit(std::ostream & os);
	std::string getAlgName() const;
	unsigned int getSearchKeys() const;
	Particle getBestParticle() const;
	Particle getParticle(const unsigned int & ind) const;
	unsigned int attacking_pso();
	void setIteration(const unsigned int &iteration);
	virtual ~IPSO();
};

#endif /* PSO_H_ */
