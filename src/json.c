#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "number.h"
#include "json.h"

const char *nullstr = "null";

const char *boolstr[] = {
	"false",
	"true"
};

char *json_encode(JSONItem *item) {
	char *output;

	if (item == NULL)
		item = json_create(JSON_NULL);

	switch (item->type) {
		case JSON_NULL:
			output = (char *)malloc(strlen(nullstr));
			strcpy(output, nullstr);
			break;

		case JSON_BOOLEAN:
			output = (char *)malloc(strlen(boolstr[item->value.boolean]));
			strcpy(output, boolstr[item->value.boolean]);
			break;

		case JSON_NUMBER:
			output = number_to_string(item->value.number);
			break;

		case JSON_STRING:
			output = (char *)malloc(strlen(item->value.string) + 2);
			sprintf(output, "\"%s\"", item->value.string);
			break;

		case JSON_LIST:
		case JSON_OBJECT:
			/* TODO recurse */
			break;
	}

	return output;
}

void json_free(JSONItem *item) {
	switch (item->type) {
		case JSON_NULL:
		case JSON_NUMBER:
			break;

		case JSON_STRING:
			free(item->value.string);
			break;

		case JSON_LIST:
		case JSON_OBJECT:
			/* TODO recurse */
			break;
	}

	free(item);
}

JSONItem *json_create(enum json_types type) {
	JSONItem *item = (JSONItem *)malloc(sizeof(JSONItem));
	memset(item, 0, sizeof(JSONItem));
	item->type = type;
	return item;
}

JSONItem *json_create_string(char *string) {
	JSONItem *item = json_create(JSON_STRING);
	item->value.string = (char *)malloc(strlen(string));
	strcpy(item->value.string, string);
	return item;
}


JSONItem *json_decode(char *input) {
	return NULL;
}

