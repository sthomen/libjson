/* all these are needed by cmocka.h */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include <stdlib.h>

#include "json.h"

void encoding_a_null_must_yield_a_null_string(void **state) {
	assert_string_equal("null", json_encode(NULL));
}

void encoding_an_explicit_null_item_must_also_yield_a_null_string(void **state) {
	JSONItem *item = json_create(JSON_NULL);

	char *json = json_encode(item);
	
	assert_string_equal("null", json);

	free(json);
	json_free(item);
}

void encoding_a_boolean_yields_the_appropriate_string(void **state) {
	JSONItem *item = json_create(JSON_BOOLEAN);
	char *json;

	// true

	item->value.boolean = 1;

	json = json_encode(item);

	assert_string_equal("true", json);

	free(json);

	// false

	item->value.boolean = 0;

	json = json_encode(item);

	assert_string_equal("false", json);

	free(json);
	json_free(item);
}

void encoding_a_string_item_adds_quotes(void **state) {
	JSONItem *item = json_create_string("Hello world!");

	char *json = json_encode(item);

	assert_string_equal("\"Hello world!\"", json);

	free(json);
	json_free(item);
}

int main(void) {
	struct CMUnitTest tests[] = {
		cmocka_unit_test(encoding_a_null_must_yield_a_null_string),
		cmocka_unit_test(encoding_an_explicit_null_item_must_also_yield_a_null_string),
		cmocka_unit_test(encoding_a_boolean_yields_the_appropriate_string),
		cmocka_unit_test(encoding_a_string_item_adds_quotes)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

