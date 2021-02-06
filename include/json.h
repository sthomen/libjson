#ifndef _JSON_H_
#define _JSON_H_

const char *nullstr = "null";

const char *boolstr[] = {
	"false",
	"true"
};

typedef struct json_pair JSONPair;
typedef struct json_item JSONItem;

struct json_item {
	enum {
		JSON_NULL,
		JSON_BOOLEAN,
		JSON_INTEGER,
		JSON_DECIMAL,
		JSON_STRING,
		JSON_LIST,
		JSON_OBJECT
	} type;

	union {
		unsigned int boolean : 1;
		int64_t      integer;
		float        decimal;
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
void json_free(JSONItem *root);

#endif /* _JSON_H_ */
