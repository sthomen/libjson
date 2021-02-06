#ifndef _JSON_H_
#define _JSON_H_

typedef struct json_pair JSONPair;
typedef struct json_item JSONItem;

struct json_item {
	enum json_types {
		JSON_NULL,
		JSON_BOOLEAN,
		JSON_NUMBER,
		JSON_STRING,
		JSON_LIST,
		JSON_OBJECT
	} type;

	union {
		unsigned int boolean : 1;
		double       number;
		char         *string;
		JSONItem     *list;
		JSONPair     **object;
	} value;
};

struct json_pair {
	char     *key;
	JSONItem *value;
};

JSONItem *json_decode(char *input);
char *json_encode(JSONItem *root);

JSONItem *json_create(enum json_types type);
JSONItem *json_create_string(char *string);
void json_free(JSONItem *root);

#endif /* _JSON_H_ */
