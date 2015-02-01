/*
 * tasgetiren_pso.h
 *
 *      Author: eugene
 */

#ifndef TASGETIREN_PSO_H_
#define TASGETIREN_PSO_H_

#include "pso.h"

class TasgetirenPSO : public IPSO {
private:
	static constexpr double V_MAX = 4.0;
	static constexpr double V_MIN = -4.0;
	static const std::string NAME_VERSION_PSO;
	TasgetirenPSO() {};
protected:
	virtual void update_particles();
	void update_velocity(double &promV, double v);
public:
	TasgetirenPSO(IDecrypt *&decrypt, IFunctionCost *&func,
			IRandomGenerator *&randG, const std::string &outFile,
			const std::string & fileKeys, const unsigned int &population);
	~TasgetirenPSO() {};
};



#endif /* TASGETIREN_PSO_H_ */
