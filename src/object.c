#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "json.h"

char *json_object_encode(JSONItem *item) {
	int outlen, klen, vlen;
	JSONObject *object;
	char *value;

	char *output = (char *)malloc(3);

	output[0] = '{';
	outlen = 1;

	if (item->value.object != NULL) {
		object = item->value.object;

		do {
			klen = strlen(object->key) + 3; // quotes and colon
			value = json_encode(object->value);
			vlen = strlen(value);

			output = (char *)realloc(output, outlen + klen + vlen + 2);
			sprintf(output+outlen, "\"%s\":%s", object->key, value);
			outlen += klen + vlen;
			output[outlen] = ',';
			outlen++;

			object = object->next;
		} while (object != NULL);
		outlen--;
	}

	output[outlen] = '}';
	output[outlen+1] = '\0';

	return output;
}

void json_object_free(JSONObject *object) {
	JSONObject *tmp;

	while (object != NULL) {
		tmp = object;
		object = tmp->next;
		free(tmp->key);
		json_free(tmp->value);
		free(tmp);
	}
}

void json_object_set(JSONItem *root, char *key, JSONItem *value) {
	int klen;
	JSONObject *object, *last, *tmp;

	// Anything but an JSON_OBJECT type here is a programming error
	assert(root->type == JSON_OBJECT);

	last = object = NULL;
	tmp = root->value.object;

	// loop through the chain of items, recording which one is the
	// one we're looking for and which is last, so we can append later
	while (tmp != NULL) {
		if (strcmp(tmp->key, key) == 0)
			object = tmp;

		if (tmp->next == NULL)
			last = tmp;

		tmp = tmp->next;
	}

	// no object found? allocate a new one
	if (object == NULL) {
		object = (JSONObject *)malloc(sizeof(JSONObject));
		memset(object, 0, sizeof(JSONObject));

		if (last == NULL) {
			root->value.object = object;
		} else {
			last->next = object;
		}
	} else { // an object was found, so clear the values for setting
		free(object->key);
		json_free(object->value);
	}

	klen = strlen(key) + 1;
	object->key = (char *)malloc(klen);
	memcpy(object->key, key, klen);
	object->value = value;
}

JSONItem *json_object_get(JSONItem *root, char *key) {
	JSONObject *object;

	assert(root->type == JSON_OBJECT);

	object = root->value.object;

	while (object != NULL) {
		if (strcmp(object->key, key) == 0)
			return object->value;

		object = object->next;
	}

	return NULL;
}
