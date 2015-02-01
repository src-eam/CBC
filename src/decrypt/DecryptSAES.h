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
	virtual std::string decrypt(const std::vector<uint8_t> &key);
	virtual void decrypt(std::string & resultDecrypt,
			const std::vector<uint8_t> &key);
	virtual void getPossibleKey(std::vector<uint8_t> &key) const;
	virtual unsigned int getDimDecrypt() const;
	virtual unsigned int getLengthData() const;

	DecryptSAES();
	explicit DecryptSAES(const std::string &txt);
	~DecryptSAES();
};

#endif
