
#include <stdint.h>
#include "console.h"
#include "dudstdio.h"

# define do_div(n, base) ({						\
	unsigned int __base = (base);					\
	unsigned int __rem;						\
	__rem = ((unsigned long long)(n)) % __base;			\
	(n) = ((unsigned long long)(n)) / __base;			\
	__rem;								\
})

void dud_number(unsigned long long num, int base)
{
	unsigned long long orignum;
	char tmp[86];
	int i;

	// tmp[0] = 0x39;
	// tmp[1] = 0x38;
	// tmp[2] = 0x0;
	// puts(tmp);

	i = 0;
	orignum = num;
	while (num != 0) {
		unsigned int rem = do_div(num, base);
		tmp[i++] = rem;
	}
	printhex("dud_number1", tmp, i);

	num = orignum;
	i = 0;
	while (num != 0) {
		unsigned int rem = do_div(num, base);
		tmp[i++] = rem;
	}
	printhex("dud_number2", tmp, i);
	printhex("^ base2", &base, sizeof(base));
	printhex("^ num2", &orignum, sizeof(orignum));
}
