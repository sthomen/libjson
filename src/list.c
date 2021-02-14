#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "json.h"

char *json_list_encode(JSONItem *item) {
	int outlen, vlen;
	JSONList *list;
	char *value;

	char *output = (char *)malloc(3); // default to []\0

	assert(item->type == JSON_LIST);

	output[0] = '[';
	outlen = 1;

	if (item->value.list != NULL) {
		list = item->value.list;

		do {
			value = json_encode(list->item);
			vlen = strlen(value);

			output = (char *)realloc(output, outlen + vlen + 2);
			memcpy(output+outlen, value, vlen);
			outlen += vlen;
			output[outlen] = ',';
			outlen++;

			list = list->next;
		} while (list != NULL);

		outlen--;
	}

	output[outlen] = ']';
	output[outlen+1] = '\0';

	return output;
}

void json_list_free(JSONList *list) {
	JSONList *tmp;

	while (list != NULL) {
		tmp = list;
		list = tmp->next;
		json_free(tmp->item);
		free(tmp);
	}
}

void json_list_add(JSONItem *root, JSONItem *item) {
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
