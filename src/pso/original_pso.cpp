/*
 * original_pso.cpp
 *
 *      Author: eugene
 */
#include "original_pso.h"

const std::string OriginalPSO::NAME_VERSION_PSO = "Original_PSO";

OriginalPSO::OriginalPSO(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG, const std::string &outFile,
		const std::string &fileKeys, const unsigned int &population) {
	this->R_MUT = 0.25;
	this->C1 = 1.42694;
	this->C2 = 1.42694;
	this->INETRIA_WEIGHT = 0.689343;
	this->init_pso(decrypt, func, randG, outFile, population);
	generate_particles(fileKeys);
	algName = NAME_VERSION_PSO;
}

void OriginalPSO::update_particles() {
	double next_velocity;
	Particle *particle;
	for (unsigned int i_th = 0; i_th < INITIAL_POPULATION; i_th++) {
		particle = &swarm[i_th];
		for (unsigned int j_th = 0; j_th < dim; j_th++) {
			next_velocity = INETRIA_WEIGHT * particle->getIndVelocity(j_th)
					+ C1 * randGenAlg->getRandom_01()
							* (p_best[i_th].getIndParticle(j_th)
									- particle->getIndParticle(j_th))
					+ C2 * randGenAlg->getRandom_01()
							* (g_best.getIndParticle(j_th)
									- particle->getIndParticle(j_th));
			if (randGenAlg->getRandom_01() < R_MUT) {
				next_velocity *= -1;
			}
			particle->setIndVelocity(j_th, next_velocity);
			particle->setIndParticle(j_th,
					(randGenAlg->getRandom_01() < signum_function(next_velocity)) ?
							1 : 0);
		}
		update_best_particle(particle, i_th);
	}
}

