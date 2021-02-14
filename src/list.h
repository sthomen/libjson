#ifndef _LIST_H_
#define _LIST_H_

#include "json.h"

char *json_list_encode(JSONItem *item);
void json_list_free(JSONList *list);
void json_list_add(JSONItem *root, JSONItem *item);

#endif /* _LIST_H_ */
