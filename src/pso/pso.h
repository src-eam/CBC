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
#include "../headersPSO.h"

//TODO Привязка ключей алгоритма расшифрования к типу unsigned int

class Particle {
protected:
	unsigned int dim;
	std::vector<unsigned int> particle;
	std::vector<double> velocity;
	unsigned int value_particle;
	double cost;
public:
	explicit Particle(const unsigned int &dim);
	Particle();
	unsigned int getIndParticle(const unsigned int &index) const;
	void setIndParticle(const unsigned int &index, const unsigned int &value);
	double getIndVelocity(const unsigned int &index) const;
	void setIndVelocity(const unsigned int &index, const double &value);
	unsigned int getValueParticle() const;
	void setValueParticle(const unsigned int & value);
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
	int INITIAL_POPULATION;
	int NUMBER_OF_ITERATION;
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
	std::vector<double> key_cost; //TODO Привязка ключей к типу unsigned int
	Particle g_best;
	unsigned int search_keys;
	std::ofstream output;
	std::string algName;

	double signum_function(const double &v) const;
	unsigned int masbin_to_int(Particle *&p) const;
	double compute_cost_value(const unsigned int &key);
	virtual void update_particles() = 0;
	void generateSwarmKeys(std::vector<unsigned int> &keysGen,
			const std::string &fileKeys);
	void generate_particles(const std::string &fileKeys);
	void init_pso(IDecrypt *&decrypt, IFunctionCost *&func,
			IRandomGenerator *&randG, const std::string &outFile);
	void update_best_particle(Particle *& particle, const unsigned int &index);
public:
	void printInit(std::ostream & os);
	std::string getAlgName() const;
	unsigned int getSearchKeys() const;
	Particle getBestParticle() const;
	unsigned int attacking_pso();
	virtual ~IPSO();
};

#endif /* PSO_H_ */
