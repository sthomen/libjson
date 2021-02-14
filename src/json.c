#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "object.h"

#include "json.h"

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

void json_free(JSONItem *item) {
	if (item == NULL)
		return;

	switch (item->type) {
		case JSON_NULL:
		case JSON_NUMBER:
			break;

		case JSON_STRING:
			free(item->value.string);
			break;

		case JSON_LIST:
			json_list_free(item->value.list);
			break;

		case JSON_OBJECT:
			json_object_free(item->value.object);
			break;
	}

	free(item);
}
