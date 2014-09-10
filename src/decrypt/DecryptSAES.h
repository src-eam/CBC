/*
 * DecryptSAES.h
 *
 *      Author: eugene
 */

#ifndef DECRYPTSAES_H_
#define DECRYPTSAES_H_

#include "SAES.h"
#include "decrypt.h"

class DecryptSAES: public SAES, public IDecrypt {
protected:
	static const uint8_t s_box_i[16];
	void invSubNibbles();
	void invMixColumns();
	void decryptSAES();
public:
	virtual std::string decrypt(const int &key);
	virtual void decrypt(std::string & resultDecrypt,const int &k);
	virtual unsigned int getPossibleKey() const;
	virtual unsigned int getDimDecrypt() const;
	virtual unsigned int getLengthData() const;

	DecryptSAES();
	explicit DecryptSAES(const std::string &txt);
	~DecryptSAES();
};

#endif
