/* all these are needed by cmocka.h */
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

/* malloc/free */
#include <stdlib.h>

#include "json.h"

/*************************************************************************
 * Nulls
 ************************************************************************/

void encoding_a_NULL_outputs_a_null_string(void **state) {
	char *json;

	json = json_encode(NULL);

	assert_string_equal("null", json);

	free(json);
}

void encoding_a_null_object_outputs_a_null_string(void **state) {
	JSONItem *item;
	char *json;

	item = json_create(JSON_NULL);
	json = json_encode(item);
	assert_string_equal("null", json);

	free(json);
	json_free(item);
}

/*************************************************************************
 * Boolean
 ************************************************************************/

void encoding_a_boolean_outputs_true_for_1_and_false_for_0(void **state) {
	JSONItem *item;
	char *json;

	item = json_create(JSON_BOOLEAN);
	/* true */

	item->value.boolean = 1;

	json = json_encode(item);

	assert_string_equal("true", json);

	free(json);

	/* false */

	item->value.boolean = 0;

	json = json_encode(item);

	assert_string_equal("false", json);

	free(json);
	json_free(item);
}

/*************************************************************************
 * Strings
 ************************************************************************/

void given_a_string_json_create_string_creates_a_string_object(void **state) {
	JSONItem *item;

	item = json_create_string("string");

	assert_int_equal(JSON_STRING, item->type);
	assert_string_equal("string", item->value.string);

	json_free(item);
}

void given_a_NULL_json_create_string_creates_an_empty_string_object(void **state) {
	JSONItem *item;

	item = json_create_string(NULL);

	assert_int_equal(JSON_STRING, item->type);
	assert_null(item->value.string);

	json_free(item);
}

void encoding_a_NULL_value_string_outputs_an_empty_string(void **state) {
	char *json;
	JSONItem *item;

	item = json_create_string(NULL);

	json = json_encode(item);

	assert_string_equal("\"\"", json);

	free(json);
	json_free(item);
}

void encoding_a_string_item_includes_quotes(void **state) {
	char *json;
	JSONItem *item;

	item = json_create_string("Hello world!");
	json = json_encode(item);

	assert_string_equal("\"Hello world!\"", json);

	free(json);
	json_free(item);
}

/*************************************************************************
 * Numbers
 ************************************************************************/

void encoding_a_whole_number_outputs_no_decimals(void **state) {
	JSONItem *item;
	char *json;

	item = json_create(JSON_NUMBER);
	
	item->value.number = 42.0;

	json = json_encode(item);

	assert_string_equal("42", json);

	free(json);
	json_free(item);
}

void encoding_a_decimal_number_outputs_decimals(void **state) {
	JSONItem *item;
	char *json;

	item = json_create(JSON_NUMBER);
	
	item->value.number = 4.2;

	json = json_encode(item);

	assert_string_equal("4.2", json);

	free(json);
	json_free(item);
}

void encoding_a_floating_point_number_includes_trailing_zeroes(void **state) {
	JSONItem *item;
	char *json;

	skip(); /* this doesn't happen and can't while number_to_string uses sprintf */

	item = json_create(JSON_NUMBER);
	
	item->value.number = 4.20;

	json = json_encode(item);

	assert_string_equal("4.20", json);

	free(json);
	json_free(item);
}

void encoding_a_large_floating_point_number_outputs_a_decimal_string(void **state) {
	JSONItem *item;
	char *json;

	skip(); /* we can't guarantee anything about the output from number_to_string here */

	item = json_create(JSON_NUMBER);
	
	item->value.number = 1234123.1234567;

	json = json_encode(item);

	assert_string_equal("1234123.1234567", json);

	free(json);
	json_free(item);
}

/*************************************************************************
 * Lists
 ************************************************************************/

void adding_an_item_to_an_empty_list_makes_it_contain_the_item(void **state) {
	JSONItem *list;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create(JSON_NULL));

	/*
	 * these two asserts interrogate the object internals so as not to depend on
	 * json_list_count and json_list_get working
	 */

	assert_non_null(list->value.list);
	assert_int_equal(list->value.list->item->type, JSON_NULL);

	json_free(list);
}

void adding_an_item_to_a_list_makes_the_count_go_from_zero_to_one(void **state) {
	JSONItem *list;

	list = json_create(JSON_LIST);

	assert_int_equal(0, json_list_count(list));

	json_list_add(list, json_create(JSON_NULL));

	assert_int_equal(1, json_list_count(list));

	json_free(list);
}

void counting_a_list_with_multiple_items_outputs_the_number_of_items(void **state) {
	JSONItem *list;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create(JSON_NULL));
	json_list_add(list, json_create(JSON_NULL));
	json_list_add(list, json_create(JSON_NULL));
	json_list_add(list, json_create(JSON_NULL));
	json_list_add(list, json_create(JSON_NULL));

	assert_int_equal(5, json_list_count(list));

	json_free(list);
}

void getting_an_item_from_a_list_gets_the_right_item_and_null_on_invalid_value(void **state) {
	JSONItem *list, *item;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("a"));
	json_list_add(list, json_create_string("b"));
	json_list_add(list, json_create_string("c"));

	item = json_list_get(list, 1);

	assert_string_equal("b", item->value.string);

	item = json_list_get(list, 0);

	assert_string_equal("a", item->value.string);

	item = json_list_get(list, 2);

	assert_string_equal("c", item->value.string);

	item = json_list_get(list, 3);

	assert_null(item);

	json_free(list);
}

void deleting_the_first_item_of_a_single_item_list_empties_it(void **state) {
	JSONItem *list;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("a"));

	json_list_delete(list, 0);
	
	assert_null(list->value.list);
	
	assert_int_equal(0, json_list_count(list));

	json_free(list);
}

void deleting_an_intermediate_item_of_a_list_maintains_list_coherency(void **state) {
	JSONItem *list, *item;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("a"));
	json_list_add(list, json_create_string("b"));
	json_list_add(list, json_create_string("c"));

	json_list_delete(list, 1);
	
	assert_int_equal(2, json_list_count(list));

	item = json_list_get(list, 1);

	assert_string_equal("c", item->value.string);

	json_free(list);
}

void inserting_an_item_inserts_the_item_at_the_right_index(void **state) {
	JSONItem *list, *item;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("a"));
	json_list_add(list, json_create_string("b"));

	json_list_insert(list, json_create_string("c"), 1);

	item = json_list_get(list, 1);

	assert_string_equal("c", item->value.string);
}

void inserting_an_item_at_position_0_prepends_it_to_the_list(void **state) {
	JSONItem *list, *item;

	list = json_create(JSON_LIST);

	json_list_add(list, json_create_string("a"));

	json_list_insert(list, json_create_string("x"), 0);

	item = json_list_get(list, 0);

	assert_string_equal("x", item->value.string);

	item = json_list_get(list, 1);

	assert_string_equal("a", item->value.string);

	json_free(list);
}

/*** encoding */

void encoding_an_empty_list_outputs_brackets_and_null(void **state) {
	char *json;
	JSONItem *item;

	item = json_create(JSON_LIST);
	json = json_encode(item);

	assert_string_equal("[]", json);

	free(json);
	json_free(item);
}

void encoding_a_list_with_a_single_string_item(void **state) {
	char *json;
	JSONItem *list, *new;

	list = json_create(JSON_LIST);
	new = json_create_string("Hello!");

	json_list_add(list, new);

	json = json_encode(list);

	assert_string_equal("[\"Hello!\"]", json);
	
	free(json);
	json_free(list);
}

void encoding_a_list_with_multiple_string_items(void **state) {
	char *json;
	JSONItem *list;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("Hello"));
	json_list_add(list, json_create_string("World!"));

	json = json_encode(list);

	assert_string_equal("[\"Hello\",\"World!\"]", json);
	
	free(json);
	json_free(list);
}

void encoding_a_list_with_all_bare_types(void **state) {
	char *json;
	JSONItem *list, *item;

	list = json_create(JSON_LIST);
	json_list_add(list, json_create_string("a string"));

	json = json_encode(list);
	assert_string_equal("[\"a string\"]", json);
	free(json);

	json_list_add(list, json_create(JSON_NULL));

	json = json_encode(list);
	assert_string_equal("[\"a string\",null]", json);
	free(json);

	json_list_add(list, NULL);

	json = json_encode(list);
	assert_string_equal("[\"a string\",null,null]", json);
	free(json);

	item = json_create(JSON_BOOLEAN);
	item->value.boolean = 1;
	json_list_add(list, item);

	json = json_encode(list);
	assert_string_equal("[\"a string\",null,null,true]", json);
	free(json);

	item = json_create(JSON_BOOLEAN);
	item->value.boolean = 0;
	json_list_add(list, item);

	json = json_encode(list);
	assert_string_equal("[\"a string\",null,null,true,false]", json);
	free(json);

	item = json_create(JSON_NUMBER);
	item->value.number = 3.14;
	json_list_add(list, item);

	json = json_encode(list);
	assert_string_equal("[\"a string\",null,null,true,false,3.14]", json);
	free(json);

	json_free(list);
}

void encoding_a_list_with_nested_lists(void **state) {
	char *json;
	JSONItem *list, *item;

	list = json_create(JSON_LIST);

	item = json_create(JSON_LIST);

	json_list_add(item, json_create_string("Hello"));

	json_list_add(list, item);

	item = json_create(JSON_LIST);

	json_list_add(item, json_create_string("World!"));

	json_list_add(list, item);

	json = json_encode(list);

	assert_string_equal("[[\"Hello\"],[\"World!\"]]", json);

	free(json);
	json_free(list);
}

/*************************************************************************
 * Objects
 ************************************************************************/

void getting_an_item_from_an_empty_object_returns_null(void **state) {
	JSONItem *object;

	object = json_create(JSON_OBJECT);

	assert_null(json_object_get(object, "something"));

	json_free(object);
}

void setting_an_item_in_an_empty_object_results_in_an_object_with_the_item(void **state) {
	JSONItem *object, *item;

	object = json_create(JSON_OBJECT);

	json_object_set(object, "item", json_create(JSON_NULL));

	/* there is SOMETHING there */
	assert_non_null(object->value.object);

	item = json_object_get(object, "item");

	/* the thing we put there, is there! */
	assert_non_null(item);

	/* it's the right type */
	assert_int_equal(JSON_NULL, item->type);

	json_free(object);
}

void replacing_an_item_in_an_object_replaces_the_item(void **state) {
	JSONItem *object, *item;

	object = json_create(JSON_OBJECT);

	json_object_set(object, "item", json_create_string("foo"));

	json_object_set(object, "item", json_create_string("bar"));

	item = json_object_get(object, "item");

	assert_non_null(item);

	assert_string_equal("bar", item->value.string);

	json_free(object);
}

void replacing_the_first_item_in_an_object_retains_the_other_items(void **state) {
	JSONItem *object, *item;

	object = json_create(JSON_OBJECT);

	json_object_set(object, "one", json_create(JSON_NULL));
	json_object_set(object, "two", json_create_string("The creeper is a spy!"));
	json_object_set(object, "one", json_create_string("Hello World!"));

	item = json_object_get(object, "two");

	assert_non_null(item);

	json_free(object);
}

void deleting_an_item_in_an_object_properly_unlinks_it(void **state) {
	JSONItem *object;

	object = json_create(JSON_OBJECT);
	json_object_set(object, "foo", json_create_string("bar"));

	assert_non_null(json_object_get(object, "foo"));

	json_object_delete(object, "foo");

	assert_null(json_object_get(object, "foo"));

	json_free(object);
}

/** encoding */

void encoding_a_empty_object_outputs_brackets_and_null(void **state) {
	char *json;
	JSONItem *object;

	object = json_create(JSON_OBJECT);

	json = json_encode(object);

	assert_string_equal("{}", json);

	free(json);
	json_free(object);
}

void encoding_an_object_with_a_single_string_item(void **state) {
	char *json;
	JSONItem *object;

	object = json_create(JSON_OBJECT);

	json_object_set(object, "test", json_create_string("Hello World!"));

	json = json_encode(object);

	assert_string_equal("{\"test\":\"Hello World!\"}", json);

	free(json);
	json_free(object);
}

void encoding_an_object_with_multiple_items_of_multiple_types(void **state) {
	char *json;
	JSONItem *object, *item;

	object = json_create(JSON_OBJECT);

	json_object_set(object, "string 1", json_create_string("Hello World!"));
	json_object_set(object, "string 2", json_create_string("Zoot!"));
	json_object_set(object, "null value", json_create(JSON_NULL));

	item = json_create(JSON_BOOLEAN);
	item->value.boolean = 1;

	json_object_set(object, "boolean true", item);

	json = json_encode(object);

	assert_string_equal("{\"string 1\":\"Hello World!\",\"string 2\":\"Zoot!\",\"null value\":null,\"boolean true\":true}", json);

	free(json);
	json_free(object);
}

void given_an_invalid_token_decode_should_stop_and_return_NULL(void **state) {
	JSONItem *item;

	item = json_decode("foobar");
	
	assert_null(item);
}

void given_an_empty_string_decode_should_stop_and_return_NULL(void **state) {
	JSONItem *item;

	item = json_decode("");

	assert_null(item);
}

void given_a_literal_null_decode_should_produce_a_JSON_NULL_object(void **state) {
	JSONItem *item;

	item = json_decode("null");

	assert_non_null(item);

	/*
	 * NOTE: This might have false positives since JSON_NULL could be
	 * defined as 0 by it being the first enum item
	 */
	assert_int_equal(item->type, JSON_NULL);
	json_free(item);
}

void given_a_json_number_decode_should_produce_a_JSON_NUMBER_object(void **state) {
	JSONItem *item;

	item = json_decode("1");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_NUMBER);
	assert_float_equal(1.0, (float)item->value.number, 0.0);
	json_free(item);

	item = json_decode("1234");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_NUMBER);
	assert_float_equal(1234.0, (float)item->value.number, 0.0);
	json_free(item);

	item = json_decode("12.34");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_NUMBER);
	assert_float_equal(12.34, (float)item->value.number, 0.01);
	json_free(item);

	item = json_decode("-42");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_NUMBER);
	assert_float_equal(-42.0, (float)item->value.number, 0.0);
	json_free(item);

	item = json_decode("+42.5");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_NUMBER);
	assert_float_equal(42.5, (float)item->value.number, 0.01);
	json_free(item);
}

void given_a_json_string_decode_should_produce_a_JSON_STRING_object(void **state) {
	JSONItem *item;

	item = json_decode("\"hello world\"");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_STRING);
	assert_string_equal("hello world", item->value.string);
	json_free(item);
}

void given_a_json_string_with_an_escaped_quote_decode_should_produce_a_JSON_STRING_object_with_the_entire_string(void **state) {
	JSONItem *item;

	item = json_decode("\"hello \\\"world\"");

	assert_non_null(item);
	assert_int_equal(item->type, JSON_STRING);
	assert_string_equal("hello \\\"world", item->value.string);
	json_free(item);
}

void given_json_with_an_empty_list_decode_should_produce_an_empty_JSON_LIST_object(void **state) {
	JSONItem *item;

	item = json_decode("[]");

	assert_non_null(item);

	assert_int_equal(JSON_LIST, item->type);
	assert_int_equal(0, json_list_count(item));
	json_free(item);
}

void given_json_with_an_empty_object_decode_should_produce_an_empty_JSON_OBJECT_object(void **state) {
	JSONItem *item;

	item = json_decode("{}");

	assert_non_null(item);

	assert_int_equal(JSON_OBJECT, item->type);
	assert_null(item->value.object);
	json_free(item);
	
}

void given_json_with_a_list_with_some_items_decode_should_produce_a_JSON_LIST_with_the_items(void **state) {
	JSONItem *item;
	JSONItem *list;

	list = json_decode(
"["
"   null,"
"	true,"
"   false,"
"	1.23,"
"	\"hello\""
"]"
);

	assert_non_null(list);

	assert_int_equal(JSON_LIST, list->type);
	assert_int_equal(5, json_list_count(list));

	item = json_list_get(list, 0);
	assert_int_equal(JSON_NULL, item->type);

	item = json_list_get(list, 1);
	assert_int_equal(JSON_BOOLEAN, item->type);
	assert_int_equal(1, item->value.boolean);

	item = json_list_get(list, 2);
	assert_int_equal(JSON_BOOLEAN, item->type);
	assert_int_equal(0, item->value.boolean);

	item = json_list_get(list, 3);
	assert_int_equal(JSON_NUMBER, item->type);
	assert_float_equal(1.23, (float)item->value.number, 0.0);

	item = json_list_get(list, 4);
	assert_int_equal(JSON_STRING, item->type);
	assert_string_equal("hello", item->value.string);

	json_free(list);
}

void given_json_with_an_object_and_some_items_should_produce_a_JSON_OBJECT_with_the_items(void **state) {
	JSONItem *item;
	JSONItem *object;

	object = json_decode(
"{"
"	\"first\": null,"
"   \"second\": true,"
"   \"third\": false,"
"	\"fourth\": 1.23,"
"	\"fifth\": \"hello\""
"}"
);

	assert_non_null(object);
	assert_int_equal(JSON_OBJECT, object->type);

	item = json_object_get(object, "first");
	assert_int_equal(JSON_NULL, item->type);

	item = json_object_get(object, "second");
	assert_int_equal(JSON_BOOLEAN, item->type);
	assert_int_equal(1, item->value.boolean);

	item = json_object_get(object, "third");
	assert_int_equal(JSON_BOOLEAN, item->type);
	assert_int_equal(0, item->value.boolean);

	item = json_object_get(object, "fourth");
	assert_int_equal(JSON_NUMBER, item->type);
	assert_float_equal(1.23, (float)item->value.number, 0.0);

	item = json_object_get(object, "fifth");
	assert_int_equal(JSON_STRING, item->type);
	assert_string_equal("hello", item->value.string);

	json_free(object);
	
}

void given_extra_data_after_a_list_json_decode_should_return_an_error(void **state) {
	JSONItem *item;

	item = json_decode("[]x");

	assert_null(item);

	json_free(item);

	item = json_decode("{}x");

	assert_null(item);

	json_free(item);

	item = json_decode("nullx");

	assert_null(item);

	json_free(item);

	item = json_decode("falsex");

	assert_null(item);

	json_free(item);

	// and finally a tricky one, when the extra data is a valid token
	item = json_decode("null null");

	assert_null(item);

	json_free(item);
}

void parse_errors_should_indicate_where_errors_happened(void **state) {
	JSONDecodeState *js;

	// non-JSON
	js = json_decode_state("error");

	assert_null(js->root);

	assert_int_equal(1, js->line);
	assert_int_equal(0, js->offset);

	free(js);

	// all in a line
	js = json_decode_state("[true, false, error]");

	assert_null(js->root);

	assert_int_equal(1, js->line);
	assert_int_equal(14, js->offset);

	free(js);

	// some kind of formatting
	js = json_decode_state("[\n\ttrue,\n\tfalse,\n\terror\n]\n");

	assert_null(js->root);

	assert_int_equal(4, js->line);
	assert_int_equal(2, js->offset);

	free(js);
}

int main(void) {
	struct CMUnitTest tests[] = {
		/* encoding, basic values */
		cmocka_unit_test(encoding_a_NULL_outputs_a_null_string),
		cmocka_unit_test(encoding_a_null_object_outputs_a_null_string),
		cmocka_unit_test(encoding_a_boolean_outputs_true_for_1_and_false_for_0),

		/* encoding, strings */
		/* TODO This needs a lot more tests, including encoding, high bytes etc. */
		cmocka_unit_test(given_a_string_json_create_string_creates_a_string_object),
		cmocka_unit_test(given_a_NULL_json_create_string_creates_an_empty_string_object),
		cmocka_unit_test(encoding_a_NULL_value_string_outputs_an_empty_string),
		cmocka_unit_test(encoding_a_string_item_includes_quotes),

		/* encoding, numbers */
		cmocka_unit_test(encoding_a_whole_number_outputs_no_decimals),
		cmocka_unit_test(encoding_a_decimal_number_outputs_decimals),
		cmocka_unit_test(encoding_a_floating_point_number_includes_trailing_zeroes),
		cmocka_unit_test(encoding_a_large_floating_point_number_outputs_a_decimal_string),

		/* lists */
		cmocka_unit_test(adding_an_item_to_an_empty_list_makes_it_contain_the_item),
		cmocka_unit_test(adding_an_item_to_a_list_makes_the_count_go_from_zero_to_one),
		cmocka_unit_test(counting_a_list_with_multiple_items_outputs_the_number_of_items),
		cmocka_unit_test(getting_an_item_from_a_list_gets_the_right_item_and_null_on_invalid_value),
		cmocka_unit_test(deleting_the_first_item_of_a_single_item_list_empties_it),
		cmocka_unit_test(deleting_an_intermediate_item_of_a_list_maintains_list_coherency),
		cmocka_unit_test(inserting_an_item_inserts_the_item_at_the_right_index),
		cmocka_unit_test(inserting_an_item_at_position_0_prepends_it_to_the_list),

		/* encoding lists */
		cmocka_unit_test(encoding_an_empty_list_outputs_brackets_and_null),
		cmocka_unit_test(encoding_a_list_with_a_single_string_item),
		cmocka_unit_test(encoding_a_list_with_multiple_string_items),
		cmocka_unit_test(encoding_a_list_with_nested_lists),

		/* objects */
		cmocka_unit_test(getting_an_item_from_an_empty_object_returns_null),
		cmocka_unit_test(setting_an_item_in_an_empty_object_results_in_an_object_with_the_item),
		cmocka_unit_test(replacing_an_item_in_an_object_replaces_the_item),
		cmocka_unit_test(replacing_the_first_item_in_an_object_retains_the_other_items),
		cmocka_unit_test(deleting_an_item_in_an_object_properly_unlinks_it),

		/* encoding objects */
		cmocka_unit_test(encoding_a_empty_object_outputs_brackets_and_null),
		cmocka_unit_test(encoding_an_object_with_a_single_string_item),
		cmocka_unit_test(encoding_an_object_with_multiple_items_of_multiple_types),

		/* decoding objects */
		cmocka_unit_test(given_an_empty_string_decode_should_stop_and_return_NULL),
		cmocka_unit_test(given_an_invalid_token_decode_should_stop_and_return_NULL),
		cmocka_unit_test(given_a_literal_null_decode_should_produce_a_JSON_NULL_object),
		cmocka_unit_test(given_a_json_number_decode_should_produce_a_JSON_NUMBER_object),
		cmocka_unit_test(given_a_json_string_decode_should_produce_a_JSON_STRING_object),
		cmocka_unit_test(given_json_with_an_empty_list_decode_should_produce_an_empty_JSON_LIST_object),
		cmocka_unit_test(given_json_with_an_empty_object_decode_should_produce_an_empty_JSON_OBJECT_object),
		cmocka_unit_test(given_a_json_string_with_an_escaped_quote_decode_should_produce_a_JSON_STRING_object_with_the_entire_string),
		cmocka_unit_test(given_json_with_a_list_with_some_items_decode_should_produce_a_JSON_LIST_with_the_items),
		cmocka_unit_test(given_json_with_an_object_and_some_items_should_produce_a_JSON_OBJECT_with_the_items),
		cmocka_unit_test(given_extra_data_after_a_list_json_decode_should_return_an_error),
		cmocka_unit_test(parse_errors_should_indicate_where_errors_happened)
	};
		

	return cmocka_run_group_tests(tests, NULL, NULL);
}

