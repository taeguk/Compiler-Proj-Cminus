#include <stdio.h>
extern int bb;
typedef long long int ll;
int main() {
	register int i = 1;
	signed int si = 1;
	unsigned int ui = 1;
	static const int super;
	auto int a;
	volatile float f = 123.456e1 * 123.45e02 * 1.2f * 1.2e0f * 1.2;
	double d = 123.456;

	sizeof(int);

	return 0;
}
