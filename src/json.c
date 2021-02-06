#include <stdlib.h>
#include <inttypes.h>

#include "json.h"

const char *unknown = "?";

char *json_encode(JSONItem *item) {
	// should we handle programming errors?
	if (item == NULL)
		return (char *)nullstr;

	switch (item->type) {
		case JSON_NULL:
			return (char *)nullstr;

		case JSON_BOOLEAN:
			return (char *)boolean[item->boolean];
			
		case JSON_INTEGER:
			return (char *)unknown;
			
		case JSON_DECIMAL:
			return (char *)unknown;

		case JSON_STRING:
			return item->string;

		case JSON_LIST:
		case JSON_OBJECT:
			/* TODO recurse */
			break;
	}
}

void json_free(JSONItem *item) {
	switch (item->type) {
		case JSON_NULL:
		case JSON_BOOLEAN:
		case JSON_INTEGER:
			break;

		case JSON_STRING:
			free(item->string);
			break;

		case JSON_LIST:
		case JSON_OBJECT:
			/* TODO recurse */
			break;
	}

	free(item);
}


JSONItem *json_decode(char *input) {
	JSONItem *item = (JSONItem *)malloc(sizeof(JSONItem));

	return item;
}

