/*
 * modified_pso.cpp
 *
 *      Author: eugene
 */

#include "modified_pso.h"

const std::string ModifiedPSO::NAME_VERSION_PSO = "Modified_PSO";

ModifiedPSO::ModifiedPSO(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG, const std::string &outFile,
		const std::string &fileKeys) {
	this->init_pso(decrypt, func, randG, outFile);
	generate_particles(fileKeys);
	algName = NAME_VERSION_PSO;
	genotype.resize(INITIAL_POPULATION,std::vector<double>(dim));
	std::vector<std::vector<double>>::iterator it;
	for(it = genotype.begin();it != genotype.end();it++) {
		for(unsigned int index = 0; index < dim; index++) {
			(*it)[index] = randGenAlg->getMinMaxRandom(G_MIN,G_MAX);
		}
	}

}

void ModifiedPSO::update_genotype(double &promG, double velocity) {
	double new_g = promG + velocity;
	promG = (new_g > G_MAX) ? G_MAX : (new_g < G_MIN) ? G_MIN : new_g;
}

void ModifiedPSO::update_particles() {
	double next_velocity;
	double next_genotype;
	Particle *particle;
	for (int i_th = 0; i_th < INITIAL_POPULATION; i_th++) {
		particle = &swarm[i_th];
		for (unsigned int j_th = 0; j_th < dim; j_th++) {
			next_velocity = INETRIA_WEIGHT * particle->getIndVelocity(j_th)
					+ C1 * randGenAlg->getRandom_01()
							* (p_best[i_th].getIndParticle(j_th)
									- particle->getIndParticle(j_th))
					+ C2 * randGenAlg->getRandom_01()
							* (g_best.getIndParticle(j_th)
									- particle->getIndParticle(j_th));
			next_genotype = genotype[i_th][j_th];
			update_genotype(next_genotype, next_velocity);
			if (randGenAlg->getRandom_01() < R_MUT) {
				next_genotype *= -1;
			}

			genotype[i_th][j_th] = next_genotype;
			particle->setIndVelocity(j_th, next_velocity);
			particle->setIndParticle(j_th,
					(randGenAlg->getRandom_01() < signum_function(next_genotype)) ?
							1 : 0);
		}
		update_best_particle(particle , i_th);
	}
}
