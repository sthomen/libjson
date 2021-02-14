#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "number.h"
#include "list.h"
#include "object.h"

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
			output = json_list_encode(item);
			break;

		case JSON_OBJECT:
			output = json_object_encode(item);
			break;
	}

	return output;
}

