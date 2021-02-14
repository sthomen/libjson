#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "json.h"

char *json_object_encode(JSONItem *item);
void json_object_free(JSONObject *object);
void json_object_set(JSONItem *root, char *key, JSONItem *value);
JSONItem *json_object_get(JSONItem *root, char *key);

#endif /* _OBJECT_H_ */
