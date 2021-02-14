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

void encoding_a_whole_number_outputs_no_decimals(void **state) {
	JSONItem *item = json_create(JSON_NUMBER);
	
	item->value.number = 42.0;

	char *json = json_encode(item);

	assert_string_equal("42", json);

	free(json);
	json_free(item);
}

void encoding_a_decimal_number_outputs_decimals(void **state) {
	JSONItem *item = json_create(JSON_NUMBER);
	
	item->value.number = 4.2;

	char *json = json_encode(item);

	assert_string_equal("4.2", json);

	free(json);
	json_free(item);
}

void encoding_a_floating_point_number_includes_trailing_zeroes(void **state) {
	char *json;

	skip(); // this doesn't happen and can't while number_to_string uses sprintf

	JSONItem *item = json_create(JSON_NUMBER);
	
	item->value.number = 4.20;

	json = json_encode(item);

	assert_string_equal("4.20", json);

	free(json);
	json_free(item);
}

void encoding_a_large_floating_point_number_outputs_a_decimal_string(void **state) {
	char *json;

	skip(); // we can't guarantee anything about the output from number_to_string here

	JSONItem *item = json_create(JSON_NUMBER);
	
	item->value.number = 1234123.1234567;

	json = json_encode(item);

	assert_string_equal("1234123.1234567", json);

	free(json);
	json_free(item);
}

void encoding_an_empty_list_outputs_brackets_and_null(void **state) {
	char *json;
	JSONItem *item = json_create(JSON_LIST);

	json = json_encode(item);

	assert_string_equal("[]", json);

	free(json);
	json_free(item);
}

void encoding_a_list_with_a_single_string_item(void **state) {
	char *json;
	JSONItem *item = json_create(JSON_LIST);
	JSONItem *new = json_create_string("Hello!");
	json_list_add(item, new);

	json = json_encode(item);

	assert_string_equal("[\"Hello!\"]", json);
	
	free(json);
	json_free(item);
}

void encoding_a_list_with_multiple_string_items(void **state) {
	char *json;
	JSONItem *root, *item;

	root = json_create(JSON_LIST);
	json_list_add(root, json_create_string("Hello"));
	json_list_add(root, json_create_string("World!"));

	json = json_encode(root);

	assert_string_equal("[\"Hello\",\"World!\"]", json);
	
	free(json);
	json_free(root);
}

void encoding_a_list_with_all_bare_types(void **state) {
	char *json;
	JSONItem *root, *item;

	root = json_create(JSON_LIST);
	json_list_add(root, json_create_string("a string"));

	json = json_encode(root);
	assert_string_equal("[\"a string\"]", json);
	free(json);

	json_list_add(root, json_create(JSON_NULL));

	json = json_encode(root);
	assert_string_equal("[\"a string\",null]", json);
	free(json);

	json_list_add(root, NULL);

	json = json_encode(root);
	assert_string_equal("[\"a string\",null,null]", json);
	free(json);

	item = json_create(JSON_BOOLEAN);
	item->value.boolean = 1;
	json_list_add(root, item);

	json = json_encode(root);
	assert_string_equal("[\"a string\",null,null,true]", json);
	free(json);

	item = json_create(JSON_BOOLEAN);
	item->value.boolean = 0;
	json_list_add(root, item);

	json = json_encode(root);
	assert_string_equal("[\"a string\",null,null,true,false]", json);
	free(json);

	item = json_create(JSON_NUMBER);
	item->value.number = 3.14;
	json_list_add(root, item);

	json = json_encode(root);
	assert_string_equal("[\"a string\",null,null,true,false,3.14]", json);
	free(json);

	json_free(root);
}

void encoding_a_list_with_nested_lists(void **state) {
	char *json;
	JSONItem *root, *item;

	root = json_create(JSON_LIST);

	item = json_create(JSON_LIST);

	json_list_add(item, json_create_string("Hello"));

	json_list_add(root, item);

	item = json_create(JSON_LIST);

	json_list_add(item, json_create_string("World!"));

	json_list_add(root, item);

	json = json_encode(root);

	assert_string_equal("[[\"Hello\"],[\"World!\"]]", json);

	free(json);
	json_free(root);

}

void encoding_a_empty_object_outputs_brackets_and_null(void **state) {
	char *json;
	JSONItem *root;

	root = json_create(JSON_OBJECT);

	json = json_encode(root);

	assert_string_equal("{}", json);

	free(json);
	json_free(root);
}

void encoding_an_object_with_a_single_string_item(void **state) {
	char *json;
	JSONItem *root;

	root = json_create(JSON_OBJECT);

	json_object_set(root, "test", json_create_string("Hello World!"));

	json = json_encode(root);

	assert_string_equal("{\"test\":\"Hello World!\"}", json);

	free(json);
	json_free(root);
}

void encoding_an_object_with_two_string_items(void **state) {
	char *json;
	JSONItem *root;

	root = json_create(JSON_OBJECT);

	json_object_set(root, "test", json_create_string("Hello World!"));
	json_object_set(root, "test2", json_create_string("Zoot!"));

	json = json_encode(root);

	assert_string_equal("{\"test\":\"Hello World!\",\"test2\":\"Zoot!\"}", json);

	free(json);
	json_free(root);
}

void encoding_an_object_with_two_values_and_replacing_one(void **state) {
	char *json;
	JSONItem *root;

	root = json_create(JSON_OBJECT);

	json_object_set(root, "one", json_create(JSON_NULL));
	json_object_set(root, "two", json_create_string("The creeper is a spy!"));
	json_object_set(root, "one", json_create_string("Hello World!"));

	json = json_encode(root);

	assert_string_equal("{\"one\":\"Hello World!\",\"two\":\"The creeper is a spy!\"}", json);

	free(json);
	json_free(root);
}

int main(void) {
	struct CMUnitTest tests[] = {
		cmocka_unit_test(encoding_a_null_must_yield_a_null_string),
		cmocka_unit_test(encoding_an_explicit_null_item_must_also_yield_a_null_string),
		cmocka_unit_test(encoding_a_boolean_yields_the_appropriate_string),
		cmocka_unit_test(encoding_a_string_item_adds_quotes),
		cmocka_unit_test(encoding_a_whole_number_outputs_no_decimals),
		cmocka_unit_test(encoding_a_decimal_number_outputs_decimals),
		cmocka_unit_test(encoding_a_floating_point_number_includes_trailing_zeroes),
		cmocka_unit_test(encoding_a_large_floating_point_number_outputs_a_decimal_string),
		cmocka_unit_test(encoding_an_empty_list_outputs_brackets_and_null),
		cmocka_unit_test(encoding_a_list_with_a_single_string_item),
		cmocka_unit_test(encoding_a_list_with_multiple_string_items),
		cmocka_unit_test(encoding_a_list_with_nested_lists),
		cmocka_unit_test(encoding_a_empty_object_outputs_brackets_and_null),
		cmocka_unit_test(encoding_an_object_with_a_single_string_item),
		cmocka_unit_test(encoding_an_object_with_two_string_items),
		cmocka_unit_test(encoding_an_object_with_two_values_and_replacing_one)
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

