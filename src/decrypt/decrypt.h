/*
 * decrypt.h
 *	Интерфейс для алгоритмов расшифрования.
 *      Author: eugene
 */

#ifndef DECRYPT_H_
#define DECRYPT_H_

#include <string>
#include <vector>
#include <stdint.h>

class IDecrypt {
public:
	virtual std::string decrypt(const std::vector<uint8_t> &key) = 0;
	virtual void decrypt(std::string & resultDecrypt, const std::vector<uint8_t> &k) = 0;
	virtual void getPossibleKey(std::vector<uint8_t> &key) const = 0;
	virtual unsigned int getDimDecrypt() const = 0;
	virtual unsigned int getLengthData() const = 0;
	virtual std::string getCipherName() const = 0;
	virtual ~IDecrypt() {};
};

#endif /* DECRYPT_H_ */
