#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>

#define FTOA_NUMBER_PRECISION 1000000

uint32_t _digits(uint64_t in) {
	return 1 + (uint32_t)log10(in);
}

void _store_integer(char *p, uint64_t value, uint32_t length) {
	uint64_t cur, mul;
	uint32_t i;

	i = length - 1;

	do {
		mul = pow(10, i);
		cur = value/mul;
		
		*p++ = (char)(48 + cur);

		value -= cur*mul;
	} while (i-- > 0);

	*p = '\0';
}

char *ftoa(double value) {
	char *output, *p;
	double abs;
	uint64_t whole, fraction;
	uint32_t len_whole, len_fraction;

	abs = fabs(value);

	whole = (uint64_t)abs;
	fraction = (uint64_t)((abs - whole) * FTOA_NUMBER_PRECISION);

	len_whole = _digits(whole);
	len_fraction = (fraction > 0) ? _digits(fraction) : 0;

	output = (char *)malloc(
		(value < 0 ? 1 : 0) +
		len_whole +
		(len_fraction > 0 ? 1 + len_fraction : 0) +
		1
	);

	p = output;

	_store_integer(p, whole, len_whole);

	if (len_fraction > 0) {
		p += len_whole;
		*p++ = '.';

		_store_integer(p, fraction, len_fraction);

		p += len_fraction;

		while (*p == '0' || *p == '\0') {
			*p-- = '\0';
		}
	}

	return output;
}

