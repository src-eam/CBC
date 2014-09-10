/*
 * decrypt.h
 *	Интерфейс для алгоритмов расшифрования.
 *      Author: eugene
 */

#ifndef DECRYPT_H_
#define DECRYPT_H_

#include <string>

class IDecrypt {
public:
	virtual std::string decrypt(const int &key) = 0;
	virtual void decrypt(std::string & resultDecrypt, const int &k) = 0;
	virtual unsigned int getPossibleKey() const = 0;
	virtual unsigned int getDimDecrypt() const = 0;
	virtual unsigned int getLengthData() const = 0;
	virtual ~IDecrypt() {};
};

#endif /* DECRYPT_H_ */
