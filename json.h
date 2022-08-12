#ifndef _JSON_H_
#define _JSON_H_

typedef struct json_item JSONItem;
typedef struct json_list JSONList;
typedef struct json_object JSONObject;
typedef struct json_decode_state JSONDecodeState;

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
	JSONItem   *value;
	JSONObject *next;
};


struct json_decode_state {
	JSONItem *root;
	int line;
	int offset;
	char *token;
};

JSONItem *json_decode(const char *);
JSONDecodeState *json_decode_state(const char *);
char *json_encode(JSONItem *);

JSONItem *json_create(enum json_types);
JSONItem *json_create_string(char *);
JSONItem *json_create_boolean(unsigned int);

JSONItem *json_create_number(double);
JSONItem *json_create_number_str(char *);
double json_number(JSONItem *);

#define JSON_LIST_LAST -1

void json_list_add(JSONItem *, JSONItem *);
void json_list_insert(JSONItem *, JSONItem *, int);
void json_list_delete(JSONItem *, int);
int json_list_count(JSONItem *);
JSONItem *json_list_get(JSONItem *, int);

void json_object_set(JSONItem *, char *, JSONItem *);
JSONItem *json_object_get(JSONItem *, char *);
void json_object_delete(JSONItem *, char *);

void json_free(JSONItem *);

void json_dump(JSONItem *);

#endif /* _JSON_H_ */
