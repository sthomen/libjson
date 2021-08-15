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

struct decode_state {
	JSONItem *root;
	char *token;
};

#include "grammar.c"

size_t regmatch(char *pattern, char *string) {
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

int tok(char **in, size_t *toklen) {
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
	void *parser;
	char *p, *ttext;
	int token;
	size_t toklen;
	struct decode_state state;

	state.token = NULL;
	state.root = (JSONItem *)malloc(sizeof(JSONItem));
	memset(state.root, 0, sizeof(JSONItem));

	parser = ParseAlloc(malloc);

	token = TOK_INVALID;

	if (*input != '\0' && parser) {
		p = (char *)input;

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

			Parse(parser, token, ttext, &state);

			p += toklen;
		}

		Parse(parser, 0, "", &state);

		ParseFree(parser, free);
	}

	if (state.token != NULL) {
		// TODO state.token now contains the unexpected token in the
		// bagging area, and can be communicated upstream somehow.
		free(state.token);
		token = TOK_INVALID;
	}

	if (token == TOK_INVALID) {
		json_free(state.root);
		state.root = NULL;
	}

	return state.root;
}

