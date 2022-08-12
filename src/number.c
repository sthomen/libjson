#include <math.h>
#include <stdlib.h>
#include <assert.h>

#include "ftoa.h"
#include "json.h"

double json_number(JSONItem *item) {
	assert(item->type == JSON_NUMBER);

	/* XXX
	 * Since the number is encoded internally as whatever
	 * number-like string was provided by the input JSON,
	 * this might be an exponent, do we need to parse it?
	 */

	return strtod(item->value.string, NULL);
}

JSONItem *json_create_number(double value) {
	JSONItem *item;

	/* JSON does not support encoding NaN and Infinity
	 * and the implementations that I have seen encode
	 * them as null, so let's go with that. 
	 */
	if (isnan(value) || isinf(value)) {
		item = json_create(JSON_NULL);
	} else {
		item = json_create(JSON_NUMBER);
		item->value.string = ftoa(value);
	}
	return item;
}

JSONItem *json_create_number_str(char *value) {
	JSONItem *item = json_create(JSON_NUMBER);

	item->value.string = value;
	return item;
}
