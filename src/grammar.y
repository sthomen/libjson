%token_prefix	TOK_
%extra_argument { JSONItem *root }

%include {
	#include <stdlib.h>
	#include <string.h>
	#include <json.h>

	/* A temporary parcel for an object pair, key and JSONItem */
	typedef struct object_pair {
		char *key;
		JSONItem *value;
	} Pair;
}

/* suppress warnings about root being unused (even though it is being used) */
%destructor warning_suppression {
	(void)root;
}

start		::= item(item). { memcpy(root, item, sizeof(JSONItem)); }

item(item) 	::= NULL.		{ item = json_create(JSON_NULL); }
item(item)	::= STRING(str).{ item = json_create_string(str); }
item(item)	::= NUMBER(str).{ item = json_create_number(strtod(str, NULL)); }
item(item)	::= TRUE.		{ item = json_create_boolean(1); }
item(item)	::= FALSE.		{ item = json_create_boolean(0); }

item(item)	::= LBRACKET list_items(list) RBRACKET. { item = list; }
item(item)	::= LCURLY object_items(object) RCURLY. { item = object; }

list_items(list)	::= list_items COMMA item(item). {
	json_list_add(list, item);
}

list_items(list)	::= item(item). {
	list = json_create(JSON_LIST);
	json_list_add(list, item);
}
list_items(list)	::= . {
	list = json_create(JSON_LIST);
}


object_items(object)	::= object_items COMMA object_pair(pair). {
	Pair *op = (Pair *)pair;
	json_object_set(object, op->key, op->value);
	free(op);
}

object_items(object)	::= object_pair(pair). {
	Pair *op = (Pair *)pair;
	object = json_create(JSON_OBJECT);
	json_object_set(object, op->key, op->value);
	free(op);
}

object_items(object)	::= . {
	object = json_create(JSON_OBJECT);
}

object_pair(pair)	::= STRING(str) COLON item(item). {
	Pair *op = (Pair *)malloc(sizeof(Pair));
	op->key = str;
	op->value = item;
	pair = op;
}
