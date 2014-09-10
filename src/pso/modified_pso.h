/*
 * modified_pso.h
 *
 *      Author: eugene
 */

#ifndef MODIFIED_PSO_H_
#define MODIFIED_PSO_H_

#include "pso.h"

class ModifiedPSO : public IPSO {
private:
	static const std::string NAME_VERSION_PSO;
	static constexpr double G_MAX = 4.0;
	static constexpr double G_MIN = -4.0;
	ModifiedPSO() {};
protected:
	virtual void update_particles();
	void update_genotype(double &promG, double velocity);
	std::vector<std::vector<double>> genotype;
public:
	ModifiedPSO(IDecrypt *&decrypt, IFunctionCost *&func,
			IRandomGenerator *&randG, const std::string &outFile,
			const std::string & fileKeys);
	~ModifiedPSO() {};
};

#endif /* MODIFIED_PSO_H_ */
