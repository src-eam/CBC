/*
 * novel_pso.cpp
 *
 *      Author: eugene
 */

#include "novel_pso.h"

const std::string NovelPSO::NAME_VERSION_PSO = "Novel_PSO";

NovelPSO::NovelPSO(IDecrypt *&decrypt, IFunctionCost *&func,
		IRandomGenerator *&randG, const std::string &outFile,
		const std::string & fileKeys, const unsigned int &population) {
	this->R_MUT = 0.4;
	this->C1 = 3.0;
	this->C2 = 1.0;
	this->INETRIA_WEIGHT = 0.96;
	this->init_pso(decrypt, func, randG, outFile, population);
	//generate_particles(fileKeys);
	algName = NAME_VERSION_PSO;
	vel_one.resize(INITIAL_POPULATION, std::vector<double>(dim));
	vel_zero.resize(INITIAL_POPULATION, std::vector<double>(dim));

	std::vector<std::vector<uint8_t>> keysGen(INITIAL_POPULATION);
	generateSwarmKeys(keysGen, fileKeys);
	double cur_cost = 0, best_cost = UINT32_MAX;
	int pos_index = 0;
	//DEV_Generator genD("/dev/urandom");
	for (unsigned int index = 0; index < INITIAL_POPULATION; index++) {
		Particle newParticle(dim);
		//std::vector<uint8_t> tt = keysGen[index];
		convert_key_to_binary(keysGen[index], newParticle);;
		cur_cost = compute_cost_value(keysGen[index]);
		//newParticle.setValueParticle(cur_key);
		newParticle.setCost(cur_cost);
		if (cur_cost < best_cost) {
			pos_index = index;
			best_cost = cur_cost;
		}
		for (unsigned int j = 0; j < dim; j++) {
			//newParticle.setIndVelocity(j, genD.getMinMaxRandom(-4, 4));
			//newParticle.setIndVelocity(j, 0);
			//newParticle.setIndVelocity(j, randGenAlg->getMinMaxRandom(-4, 4));
			vel_one[index][j] = randGenAlg->getMinMaxRandom(-4, 4);
			vel_zero[index][j] = randGenAlg->getMinMaxRandom(-4, 4);
			newParticle.setIndVelocity(j,
					(newParticle.getIndParticle(j) == 1) ?
							vel_zero[index][j] : vel_one[index][j]);
		}
		swarm.push_back(newParticle);
		p_best.push_back(newParticle);
	}
	g_best = p_best[pos_index];
}

void NovelPSO::velocity_update(const unsigned int &index) {
}

void NovelPSO::update_particles() {
	double d_one_1, d_one_2;
	double d_zero_1, d_zero_2;
	double r1, r2;
	unsigned int tmp;
	double next_velocity;
	Particle *particle;
	for (unsigned int i_th = 0; i_th < INITIAL_POPULATION; i_th++) {
		particle = &swarm[i_th];
		for (unsigned int j_th = 0; j_th < dim; j_th++) {
			tmp = p_best[i_th].getIndParticle(j_th);
			r1 = randGenAlg->getRandom_01();
			r2 = randGenAlg->getRandom_01();
			if (tmp == 1) {
				d_one_1 = C1 * r1;
				d_zero_1 = -(C1 * r1);
			}
			//tmp = 0
			else {
				d_zero_1 = C1 * r1;
				d_one_1 = -(C1 * r1);
			}
			tmp = g_best.getIndParticle(j_th);
			if (tmp == 1) {
				d_one_2 = C2 * r2;
				d_zero_2 = -(C2 * r2);
			}
			//tmp = 0
			else {
				d_zero_2 = C2 * r2;
				d_one_2 = -(C2 * r2);
			}
			vel_one[i_th][j_th] = INETRIA_WEIGHT * vel_one[i_th][j_th] + d_one_1
					+ d_one_2;
			vel_zero[i_th][j_th] = INETRIA_WEIGHT * vel_zero[i_th][j_th]
					+ d_zero_1 + d_zero_2;

			tmp = particle->getIndParticle(j_th);
			next_velocity =
					(tmp == 1) ? vel_zero[i_th][j_th] : vel_one[i_th][j_th];
			if (randGenAlg->getRandom_01() < R_MUT) {
				next_velocity *= -1;
				/*(tmp == 1) ? vel_zero[i_th][j_th] *= -1 : vel_one[i_th][j_th] *=
										-1;*/
			}
			particle->setIndVelocity(j_th, next_velocity);
			particle->setIndParticle(j_th,
					(randGenAlg->getRandom_01()
							< signum_function(particle->getIndVelocity(j_th))) ?
							!tmp : tmp);
		}
		update_best_particle(particle, i_th);
	}
}

