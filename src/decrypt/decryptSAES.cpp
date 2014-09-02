#include "DecryptSAES.h"

const uint8_t DecryptSAES::s_box_i[16] = { 0xa, 0x5, 0x9, 0xb, 0x1, 0x7, 0x8, 0xf, 0x6, 0x0, 0x2,
				0x3, 0xc, 0x4, 0xd, 0xe };

void DecryptSAES::invSubNibbles() {
	for (uint8_t ind = 0; ind < Nk; ind++) {
		state[ind] = (s_box_i[state[ind] & 0xf])
				| (s_box_i[((state[ind] & 0b11110000) >> 4)] << 4);
	}
}

void DecryptSAES::invMixColumns() {
	uint8_t a, b, e, d;
	a = g_mul(9, (state[0] & 0xf)) ^ g_mul(2, ((state[0] & 0b11110000) >> 4));
	b = g_mul(2, state[0] & 0xf) ^ g_mul(9, ((state[0] & 0b11110000) >> 4));
	e = g_mul(9, (state[1] & 0xf)) ^ g_mul(2, ((state[1] & 0b11110000) >> 4));
	d = g_mul(2, state[1] & 0xf) ^ g_mul(9, ((state[1] & 0b11110000) >> 4));
	state[0] = (a | (b << 4));
	state[1] = (e | (d << 4));
}

void DecryptSAES::decryptSAES() {
	addRoundKey(2);
	shiftRows();
	invSubNibbles();
	addRoundKey(1);
	invMixColumns();
	shiftRows();
	invSubNibbles();
	addRoundKey(0);
}

std::string DecryptSAES::decrypt(const int &key) {
	std::string result;
	decrypt(result,key);
	return result;
}

void DecryptSAES::decrypt(std::string &resultDecrypt, const int &key) {
	if (this->text == "")
		resultDecrypt = "";
	else {
		keyExpand(key);
		if(resultDecrypt.capacity() < text.size() )
			resultDecrypt.reserve(this->text.size());
		for (auto iter = this->text.begin(); iter != this->text.end(); iter +=
				2) {
			state[0] = (*iter);
			state[1] = (*(iter + 1));
			decryptSAES();
			resultDecrypt += std::string(state, state + Nk);
		}
	}
}

DecryptSAES::DecryptSAES() {
	this->text = "";
}

DecryptSAES::DecryptSAES(const std::string &txt) {
	this->text = txt;
	if (this->text.size() & 1)
		text += " ";
}

DecryptSAES::~DecryptSAES() {
	;
}

