/*
 * novel_pso.h
 *
 *      Author: eugene
 */

#ifndef NOVEL_PSO_H_
#define NOVEL_PSO_H_

#include "pso.h"

class NovelPSO: public IPSO {
private:
	static const std::string NAME_VERSION_PSO;
	NovelPSO() {};
protected:
	virtual void update_particles();
	std::vector<std::vector<double>> vel_one;
	std::vector<std::vector<double>> vel_zero;
	void velocity_update(const unsigned int &index);
public:
	NovelPSO(IDecrypt *&decrypt, IFunctionCost *&func, IRandomGenerator *&randG,
			const std::string &outFile, const std::string & fileKeys,
			const unsigned int &population);
	~NovelPSO() {};
};

#endif /* NOVEL_PSO_H_ */
