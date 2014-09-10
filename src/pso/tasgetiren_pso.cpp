/*
 * tasgetiren_pso.cpp
 *
 *      Author: eugene
 */

#include "tasgetiren_pso.h"

const std::string TasgetirenPSO::NAME_VERSION_PSO = "Tasgetiren_PSO";

TasgetirenPSO::TasgetirenPSO(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG, const std::string &outFile,
		const std::string &fileKeys) {
	this->init_pso(decrypt, func, randG, outFile);
	generate_particles(fileKeys);
	algName = NAME_VERSION_PSO;
}

void TasgetirenPSO::update_velocity(double &promV, double velocity) {
	double new_v = promV + velocity;
	promV = (new_v > V_MAX) ? V_MAX : (new_v < V_MIN) ? V_MIN : new_v;
}

void TasgetirenPSO::update_particles() {
	double next_velocity;
	Particle *particle;
	for (int i_th = 0; i_th < INITIAL_POPULATION; i_th++) {
		particle = &swarm[i_th];
		for (unsigned int j_th = 0; j_th < dim; j_th++) {
			next_velocity = C1 * randGenAlg->getRandom_01()
					* (p_best[i_th].getIndParticle(j_th)
							- particle->getIndParticle(j_th))
					+ C2 * randGenAlg->getRandom_01()
							* (g_best.getIndParticle(j_th)
									- particle->getIndParticle(j_th));
			update_velocity(next_velocity, particle->getIndVelocity(j_th));
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

