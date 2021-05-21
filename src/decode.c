#include <stdlib.h>
#include <regex.h>

#include "json.h"
#include "grammar.h"

#define TOK_END 0
#define TOK_INVALID -1

static const struct {
	char * pattern;
	int    id;
} tokens[] = {
	{ "^null",                     TOK_NULL },
	{ "^true",                     TOK_TRUE },
	{ "^false",                    TOK_FALSE },
	{ "^\"(\\\\.|[^\\\"])*\"",     TOK_STRING },
	{ "^(-|\\+)?([0-9]+\\.[0-9]+|[0-9]+)", TOK_NUMBER },
	{ "^:",                        TOK_COLON },
	{ "^,",                        TOK_COMMA },
	{ "^\\[",                      TOK_LBRACKET },
	{ "^\\]",                      TOK_RBRACKET },
	{ "^{",                        TOK_LCURLY },
	{ "^}",                        TOK_RCURLY },
	{ NULL,                        TOK_INVALID }
};

#include "grammar.c"

int regmatch(char *pattern, char *string) {
	regex_t reg;
	regmatch_t match;
	int result;

	// default to programming errors
	result = -1;

	if (regcomp(&reg, pattern, REG_EXTENDED|REG_NEWLINE) == 0) {
		switch (regexec(&reg, string, 1, &match, 0)) {
			case 0:
				result = match.rm_eo - match.rm_so;
				break;

			case REG_NOMATCH:
				result = 0;
				break;
		}
	}

	return result;
}

int tok(char **in, int *toklen) {
	int i;

	// skip any whitespace before the token
	while ((**in == ' ' || **in == '\n' || **in == '\r' || **in == '\t') && (**in)++);

	// end at NULL
	if (**in == 0)
		return TOK_END;

	for (i = 0; tokens[i].pattern != NULL; i++) {
		if ((*toklen = regmatch(tokens[i].pattern, *in)) > 0)
			return tokens[i].id;
	}

	return TOK_INVALID;
}

JSONItem *json_decode(char *input) {
	void *parser;
	char *p, *ttext;
	int token, toklen;
	JSONItem *root;

	root = (JSONItem *)malloc(sizeof(JSONItem));
	memset(root, 0, sizeof(JSONItem));

	parser = ParseAlloc(malloc);

	token = TOK_INVALID;

	if (*input != '\0' && parser) {
		p = (char *)input;

		while ((token = tok(&p, &toklen)) > 0) {
			if (token == TOK_INVALID)
				break;

			// don't include the quotes in strings
			if (token == TOK_STRING) {
				ttext = strndup(p+1, toklen-2);
			} else {
				ttext = strndup(p, toklen);
			}
			
			if (!ttext)
				break;

			Parse(parser, token, ttext, root);

			p += toklen;
		}

		Parse(parser, 0, "", root);

		ParseFree(parser, free);
	}

	if (token == TOK_INVALID) {
		json_free(root);
		root = NULL;
	}

	return root;
}

