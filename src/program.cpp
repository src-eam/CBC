#include <iostream>
#include "generators/DEV_Generator.h"

using namespace std;

int main() {
	DEV_Generator ss("/dev/urandom");
	IRandomGenerator *dd = &ss;
	cout << dd->getRandom_01() << endl;
	cout << (int) dd->getMinMaxRandom(1, 65534) << endl;
	return 0;
}
