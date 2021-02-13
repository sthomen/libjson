#ifndef _JSON_H_
#define _JSON_H_

typedef struct json_item JSONItem;
typedef struct json_list JSONList;
typedef struct json_object JSONObject;

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
		JSONList     *list;
		JSONObject   *object;
	} value;
};

struct json_list {
	JSONItem *item;
	JSONList *next;
};

struct json_object {
	char       *key;
	JSONItem   *item;
	JSONObject *next;
};

JSONItem *json_decode(char *input);
char *json_encode(JSONItem *root);

JSONItem *json_create(enum json_types type);
JSONItem *json_create_string(char *string);
void json_list_add_item(JSONItem *item, JSONItem *new);

void json_free(JSONItem *item);

#endif /* _JSON_H_ */
