#include "SAES.h"

const uint8_t SAES::s_box[16] = { 0x9, 0x4, 0xa, 0xb, 0xd, 0x1, 0x8, 0x5,0x6, 0x2, 0x0, 0x3, 0xc, 0xe, 0xf, 0x7 };

void SAES::keyExpand(uint8_t key[]) {
	wwKey[0] = key[0];
	wwKey[1] = key[1];
	wwKey[2] = wwKey[0] ^ 0b10000000 ^ rotSubNib(wwKey[1]);
	wwKey[3] = wwKey[2] ^ wwKey[1];
	wwKey[4] = wwKey[2] ^ 0b00110000 ^ rotSubNib(wwKey[3]);
	wwKey[5] = wwKey[3] ^ wwKey[4];
}

void SAES::keyExpand(const int &k){
	uint8_t key[2];
	key[0] = (k & 0b11111111);
	key[1] = ((k & 0b1111111100000000) >> 8);
	keyExpand(key);
}

uint8_t SAES::rotSubNib(uint8_t nib) const {
	return (subNib(nib) << 4) | subNib((nib >> 4));
}

uint8_t SAES::subNib(uint8_t nib) const {
	return s_box[nib & 0xf];
}

void SAES::addRoundKey(uint8_t round) {
	round <<= 1;
	state[0] ^= wwKey[round];
	state[1] ^= wwKey[round + 1];
}

void SAES::shiftRows() {
	uint8_t prom = (state[0] & 0xf);
	state[0] = (state[0] & 0b11110000) | (state[1] & 0xf);
	state[1] = (state[1] & 0b11110000) | prom;
}

uint8_t SAES::g_mul(uint8_t a, uint8_t b) const {
	uint8_t result = 0;
	while (b) {
		if (b & 1)
			result ^= a;
		a <<= 1;
		if (a & 0b10000)
			a ^= 0b11;
		b >>= 1;
	}
	return result & 0b1111;
}

std::string SAES::getText() {
	return text;
}

SAES::~SAES() {
	;
}
