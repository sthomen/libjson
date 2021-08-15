/**
 * IRIX doesn't have strndup, so here's an implementation
 */
#include <stdlib.h>
#include <string.h>

#ifdef __sgi
char *strndup(const char *str, size_t len) {
	char *out = (char *)malloc(len+1);

	if (!out)
		return NULL;

	memcpy(out, str, len);
	out[len] = '\0';

	return out;
}

#endif
