/* all these are needed by cmocka.h */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

#include "json.h"

void encoding_a_null_must_yield_a_null_string(void **state) {
	assert_string_equal("null", json_encode(NULL));
}

void encoding_an_explicit_null_item_must_also_yield_a_null_string(void **state) {
	JSONItem *item = json_create(JSON_NULL);
	
	assert_string_equal("null", json_encode(item));

	json_free(item);
}

void encoding_a_boolean_yields_the_appropriate_string(void **state) {
	JSONItem *item = json_create(JSON_BOOLEAN);

	item->value.boolean = 1;

	assert_string_equal("true", json_encode(item));

	item->value.boolean = 0;

	assert_string_equal("false", json_encode(item));

	json_free(item);
}

int main(void) {
	struct CMUnitTest tests[] = {
		cmocka_unit_test(encoding_a_null_must_yield_a_null_string),
		cmocka_unit_test(encoding_an_explicit_null_item_must_also_yield_a_null_string),
		cmocka_unit_test(encoding_a_boolean_yields_the_appropriate_string)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

