#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "json.h"

char *json_list_encode(JSONItem *item) {
	int outlen, vlen;
	JSONList *list;
	char *value;

	char *output = (char *)malloc(3); /* default to []\0 */

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

void json_list_insert(JSONItem *root, JSONItem *item, int index) {
	int i;
	JSONList *last, *list, *new;

	assert(root->type == JSON_LIST);

	last = NULL;
	list = root->value.list;

	new = (JSONList *)malloc(sizeof(JSONList));
	new->item = item;
	new->next = NULL;

	for (i = 0; list != NULL; i++) {
		if (i == index) {
			break;
		}

		last = list;
		list = list->next;
	}

	if (last == NULL) {
		last = root->value.list;
		root->value.list = new;
		new->next = last;
	} else {
		if (last->next != NULL)
			new->next = last->next;

		last->next = new;
	}
}

void json_list_add(JSONItem *root, JSONItem *item) {
	json_list_insert(root, item, JSON_LIST_LAST);
}

int json_list_count(JSONItem *root) {
	int result = 0;
	JSONList *list;

	assert(root->type == JSON_LIST);

	list = root->value.list;

	while (list != NULL) {
		result++;
		list = list->next;
	}

	return result;
}

JSONItem *json_list_get(JSONItem *root, int index) {
	int i;
	JSONList *list;
	JSONItem *item;

	assert(root->type == JSON_LIST);

	list = root->value.list;
	item = NULL;

	for (i = 0; list != NULL; i++) {
		if (i == index) {
			item = list->item;
			break;
		}

		list = list->next;
	}

	return item;
}

void json_list_delete(JSONItem *root, int index) {
	int i;
	JSONList *list, *last;

	assert(root->type == JSON_LIST);

	last = NULL;
	list = root->value.list;

	for (i = 0; list != NULL; i++) {
		if (i == index) {
			if (i == 0) { /* last == NULL */
				root->value.list = list->next;
			} else {
				last->next = list->next;
			}

			json_free(list->item);
			free(list);
			break;
		}

		last = list;
		list = list->next;
	}
}
