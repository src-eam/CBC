/*
 * original_pso.h
 *
 *      Author: eugene
 */

#ifndef ORIGINAL_PSO_H_
#define ORIGINAL_PSO_H_

#include "pso.h"

class OriginalPSO: public IPSO {
private:
	static const std::string NAME_VERSION_PSO;
	OriginalPSO() {};
protected:
	virtual void update_particles();
public:
	OriginalPSO(IDecrypt *&decrypt, IFunctionCost *&func,
			IRandomGenerator *&randG, const std::string &outFile,
			const std::string & fileKeys,const unsigned int &population);
	~OriginalPSO() {};
};

#endif /* ORIGINAL_PSO_H_ */
