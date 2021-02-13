#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <assert.h>

#include "number.h"
#include "json.h"

const char *nullstr = "null";

const char *boolstr[] = {
	"false",
	"true"
};

char *json_encode(JSONItem *item) {
	char *output;
	int outlen, len;
	char *p;
	JSONList *list;

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
			output = (char *)malloc(3); // default to []\0
			output[0] = '[';
			outlen = 1;

			if (item->value.list != NULL) {
				list = item->value.list;

				do {
					p = json_encode(list->item);
					len = strlen(p);

					// the magic number 2 here is for the , (or ] for the last entry)
					// and the terminating NULL.
					output = (char *)realloc(output, outlen + len + 2);
					memcpy(output+outlen, p, len);
					outlen += len;
					output[outlen] = ',';
					outlen++;

					list = list->next;
				} while (list != NULL);

				// move back one spot, so the final , will be replaced with a ]
				outlen--;
			}

			output[outlen] = ']';
			output[outlen+1] = '\0';

			break;

		case JSON_OBJECT:
			break;
	}

	return output;
}

void json_free(JSONItem *item) {
	JSONList *list, *tmp;

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
			list = item->value.list;

			while (list != NULL) {
				tmp = list;
				list = tmp->next;
				json_free(tmp->item);
				free(tmp);
			}
			break;
		case JSON_OBJECT:
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

void json_list_add_item(JSONItem *root, JSONItem *item) {
	JSONList *list, *new;

	assert(root->type == JSON_LIST);

	list = root->value.list;
	new = (JSONList *)malloc(sizeof(JSONList));

	new->item = item;
	new->next = NULL;

	if (list == NULL) {
		root->value.list = new;
	} else {
		while (list->next != NULL)
			list = list->next;

		list->next = new;
	}
}

JSONItem *json_decode(char *input) {
	return NULL;
}

