#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "number.h"

/* space to allocate for decimals */
#define NUMBER_DECIMALS 10

/**
 * Render a double into a string, guaranteeing that there's enough space for
 * the integer portion. Also convert numbers with no decimals in to whole
 * numbers.
 *
 * NOTE: This is still very crude, using snprintf and sscanf is less
 * than ideal. It should be stored in some other way than a double to
 * avoid rounding errors due to floating point numbers and also retain
 * the decimal point when parsing and re-encoding numbers (here, 1.000 would
 * be re-encoded as 1).
 *
 * snprintf might decide to round the number or output it in scientific
 * notation under our nose. While this is OK for JSON, it's a bit of a
 * bastard to test.
 *
 * For non-huge numbers, this is still functional, so it can stay for now.
 *
 */
char *number_to_string(double number) {
	char *output;
	double tmp;
	int len;
	int len_whole;
	int len_symbols = 1;
	int len_decimal = NUMBER_DECIMALS;

	/* there's a minus symbol */
	if (number < 0)
		len_symbols++;

	tmp = fabs(number);

	len_whole = 1 + (int)floor(log10(tmp));

	/* no decimals */
	if (floor(tmp) == tmp) {
		len_decimal = 0;
		len_symbols--;
	}

	len = len_whole + len_decimal + len_symbols + 1; /* + null */

	output = (char *)malloc(len);

	if (len_decimal == 0) {
		snprintf(output, len, "%d", (int)number);
	} else {
		snprintf(output, len, "%g", number);
	}

	return output;
}
