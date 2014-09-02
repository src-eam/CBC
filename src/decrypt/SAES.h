#ifndef SAES_H_
#define SAES_H_
#include <string>
#include <stdint.h>
class SAES {

protected:
	static const uint8_t s_box[16];
	const static int Nk = 2;

	uint8_t wwKey[6];
	uint8_t state[2];
	std::string text;

	void keyExpand(uint8_t key[]);
	void keyExpand(const int &k);
	uint8_t subNib(uint8_t nib) const;
	uint8_t rotSubNib(uint8_t nib) const;
	void shiftRows();
	void addRoundKey(uint8_t round);
	uint8_t g_mul(uint8_t a, uint8_t b) const; //GF(2^4)/x^4 + x + 1

public:
	std::string getText();
	virtual ~SAES() = 0;
};



#endif
