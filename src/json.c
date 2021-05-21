#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

	if (string != NULL) {
		item->value.string = (char *)malloc(strlen(string));
		strcpy(item->value.string, string);
	}

	return item;
}

JSONItem *json_create_boolean(unsigned int value) {
	JSONItem *item = json_create(JSON_BOOLEAN);

	item->value.boolean = (value > 0 ? 1 : 0);
	return item;
}

JSONItem *json_create_number(double value) {
	JSONItem *item = json_create(JSON_NUMBER);

	item->value.number = value;
	return item;
}

void json_free(JSONItem *item) {
	if (item == NULL)
		return;

	switch (item->type) {
		case JSON_NULL:
		case JSON_BOOLEAN:
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

void json_dump(JSONItem *item) {
	JSONList *list;
	JSONObject *object;
	int i;
	static int depth = 0;

	if (item == NULL) {
		printf("json_dump(): NULL input\n");
		return;
	}

	for (i = 0; i < depth; i++)
		printf(" ");

	switch (item->type) {
		case JSON_NULL:
			printf("null\n");
			break;

		case JSON_NUMBER:
			printf("number: %f\n", item->value.number);
			break;

		case JSON_BOOLEAN:
			printf("boolean: %s\n", (item->value.boolean == 0 ? "false" : "true"));
			break;

		case JSON_STRING:
			printf("string: %s\n", item->value.string);
			break;

		case JSON_LIST:
			printf("list (%d):\n", json_list_count(item));
			list = item->value.list;

			depth++;

			while (list != NULL) {
				json_dump(list->item);
				list = list->next;
			}

			depth--;

			break;

		case JSON_OBJECT:
			printf("object:\n");
			object = item->value.object;

			depth++;

			while (object != NULL) {
				for (i = 0; i < depth; i++)
					printf(" ");

				printf("key: %s\n", object->key);

				json_dump(object->value);
				object = object->next;
				printf("\n");
			}

			depth--;

			break;
	}
}
