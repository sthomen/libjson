#include <stdlib.h>
#include <regex.h>

#ifdef __sgi
char *strndup(const char *, size_t);
#else
#include <string.h>
#endif

#include "json.h"
#include "grammar.h"

#define TOK_END 0
#define TOK_INVALID -1

static const struct {
	char * pattern;
	int    id;
} tokens[] = {
	{ "^null",                             TOK_NULL     },
	{ "^true",                             TOK_TRUE     },
	{ "^false",                            TOK_FALSE    },
	{ "^\"(\\\\.|[^\\\"])*\"",             TOK_STRING   },
	{ "^(-|\\+)?([0-9]+\\.[0-9]+|[0-9]+)", TOK_NUMBER   },
	{ "^:",                                TOK_COLON    },
	{ "^,",                                TOK_COMMA    },
	{ "^\\[",                              TOK_LBRACKET },
	{ "^\\]",                              TOK_RBRACKET },
	{ "^\\{",                              TOK_LCURLY   },
	{ "^\\}",                              TOK_RCURLY   },
	{ NULL,                                TOK_INVALID  }
};

#include "grammar.c"

size_t regmatch(const char *pattern, const char *string) {
	regex_t reg;
	regmatch_t match;
	size_t result;

	result = -1;

	if (regcomp(&reg, pattern, REG_EXTENDED|REG_NEWLINE) == 0) {
		switch (regexec(&reg, string, 1, &match, 0)) {
			case 0:
				result = (size_t)(match.rm_eo - match.rm_so);
				break;

			case REG_NOMATCH:
				result = 0;
				break;
		}
	}

	return result;
}

int tok(const char **in, size_t *toklen) {
	int i;

	while (**in == ' ' || **in == '\n' || **in == '\r' || **in == '\t')
		(*in)++;

	if (**in == 0)
		return TOK_END;

	for (i = 0; tokens[i].pattern != NULL; i++) {
		if ((*toklen = regmatch(tokens[i].pattern, *in)) > 0)
			return tokens[i].id;
	}

	return TOK_INVALID;
}

JSONItem *json_decode(const char *input) {
	JSONDecodeState *state;
	JSONItem *root;

	state = json_decode_state(input);

	/* clean up state before returning the root */

	if (state->token != NULL)
		free(state->token);

	root = state->root;

	free(state);

	return root;
}

void update_state_position(const char *input, const char *stop, JSONDecodeState *state) {
	const char *p;
	const char *start;

	start = input;

	for (p = stop; p != input; p--) {
		if (*p == '\n') {
			if (start == input)
				start = p;

			state->line++;
		}
	}

	state->offset = stop - start;
}

JSONDecodeState *json_decode_state(const char *input) {
	void *parser;
	const char *p, *ttext;
	int token;
	size_t toklen;
	JSONDecodeState *state;

	p = NULL;

	state = (JSONDecodeState *)malloc(sizeof(JSONDecodeState));
	state->line = 1;
	state->offset = 0;

	state->token = NULL;
	state->root = (JSONItem *)malloc(sizeof(JSONItem));
	memset(state->root, 0, sizeof(JSONItem));

	parser = ParseAlloc(malloc);

	token = TOK_INVALID;

	if (*input != '\0' && parser) {
		p = input;

		while ((token = tok(&p, &toklen)) > 0) {
			if (token == TOK_INVALID)
				break;

			if (token == TOK_STRING) {
				ttext = strndup(p+1, toklen-2);
			} else {
				ttext = strndup(p, toklen);
			}
			
			if (!ttext)
				break;

			Parse(parser, token, (char *)ttext, state);

			if (state->token != NULL) {
				token = TOK_INVALID;
				break;
			}

			p += toklen;
		}

		Parse(parser, 0, "", state);

		ParseFree(parser, free);
	}

	if (token == TOK_INVALID) {
		// p might be unitialized if the string is empty
		if (p != NULL)
			update_state_position(input, p, state);

		json_free(state->root);
		state->root = NULL;
	}

	return state;
}

