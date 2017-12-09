#define BOOST_TEST_MODULE utils_test
#include <boost/test/included/unit_test.hpp>

using byte = uint8_t;

#include "../classes/template_db.cpp"
#include "../utils/utils.cpp"

boost::test_tools::predicate_result validate_hex_vect(std::vector<unsigned char> a, std::vector<unsigned char> b) {
  std::vector<unsigned char>::iterator it1 = a.begin();
  std::vector<unsigned char>::iterator it2 = b.begin();
  for(; it1 != a.end() && it2 != b.end(); ++it1, ++it2) {
    if (*it1 != *it2) return false;
  }
  return true;
}

boost::test_tools::predicate_result validate_string_vec(std::vector<std::string> a, std::vector<std::string> b) {
  std::vector<std::string>::iterator it1 = a.begin();
  std::vector<std::string>::iterator it2 = b.begin();
  for(; it1 != a.end() && it2 != b.end(); ++it1, ++it2) {
    if (*it1 != *it2) return false;
  }
  return true;
}

BOOST_AUTO_TEST_SUITE(utils_test)

BOOST_AUTO_TEST_CASE(test_get_row_key)
{
  std::string table_name ("test_table");
  int table_row_id = 1;
  BOOST_CHECK_EQUAL(get_row_key(table_name, table_row_id), "test_table:1");
}

BOOST_AUTO_TEST_CASE(test_string_to_bytes)
{
  std::vector<unsigned char> string_obj = obj_to_bytes("hello");
  std::vector<unsigned char> string_obj_expected{0x68, 0x65, 0x6c, 0x6c, 0x6f};
  BOOST_CHECK(validate_hex_vect(string_obj, string_obj_expected));
}

BOOST_AUTO_TEST_CASE(test_obj_to_byte_to_string)
{
  Table2 row2;
  row2.set<int>(std::make_shared<int>(1));
  row2.set<std::string>(std::make_shared<std::string>("col_2"));
  BOOST_CHECK_EQUAL(bytes_to_string(&obj_to_bytes(&row2)[0], 2), "1, col_2");

  Table3 row3;
  row3.set<int>(std::make_shared<int>(1));
  row3.set<std::string, Key1>(std::make_shared<std::string>("col_2"));
  row3.set<std::string, Key2>(std::make_shared<std::string>("col_3"));
  BOOST_CHECK_EQUAL(bytes_to_string(&obj_to_bytes(&row3)[0], 3), "1, col_2, col_3");

  Table4 row4;
  row4.set<int>(std::make_shared<int>(1));
  row4.set<std::string, Key1>(std::make_shared<std::string>("col_2"));
  row4.set<std::string, Key2>(std::make_shared<std::string>("col_3"));
  row4.set<std::string, Key3>(std::make_shared<std::string>("col_4"));
  BOOST_CHECK_EQUAL(bytes_to_string(&obj_to_bytes(&row4)[0], 4), "1, col_2, col_3, col_4");

  Table5 row5;
  row5.set<int>(std::make_shared<int>(1));
  row5.set<std::string, Key1>(std::make_shared<std::string>("col_2"));
  row5.set<std::string, Key2>(std::make_shared<std::string>("col_3"));
  row5.set<std::string, Key3>(std::make_shared<std::string>("col_4"));
  row5.set<std::string, Key4>(std::make_shared<std::string>("col_5"));
  BOOST_CHECK_EQUAL(bytes_to_string(&obj_to_bytes(&row5)[0], 5), "1, col_2, col_3, col_4, col_5");
}

BOOST_AUTO_TEST_CASE(test_parse_clean)
{
  std::string dirty ("trim/keep");
  BOOST_CHECK_EQUAL(parse_to_clean(dirty), "keep");

  std::string illegal ("illegal");
  BOOST_CHECK_EQUAL(parse_to_clean(illegal), "_");
}

BOOST_AUTO_TEST_CASE(test_parse_int)
{
  std::string unparsed ("123:unnecessary");
  BOOST_CHECK_EQUAL(parse_int(unparsed), "123");

  std::string unparsed_2 ("123:unnecessary:notneeded");
  BOOST_CHECK_EQUAL(parse_int(unparsed_2), "123");

  std::string unparsable ("123unnecessary");
  BOOST_CHECK_EQUAL(parse_int(unparsable), "-1");
}

BOOST_AUTO_TEST_CASE(test_parse_name)
{
  std::string unparsed ("123:necessary");
  BOOST_CHECK_EQUAL(parse_name(unparsed), "necessary");

  std::string unparsed_2 ("123:all:necessary");
  BOOST_CHECK_EQUAL(parse_name(unparsed_2), "all:necessary");

  std::string unparsable ("123necessary");
  BOOST_CHECK_EQUAL(parse_name(unparsable), "-1");
}

BOOST_AUTO_TEST_CASE(test_parse_table_name)
{
  std::string unparsed ("123:necessary;unnecessary");
  BOOST_CHECK_EQUAL(parse_insert_table_name(unparsed), "necessary");

  std::string unparsable_1 ("123necessaryunnecessary");
  BOOST_CHECK_EQUAL(parse_insert_table_name(unparsable_1), "-1");

  std::string unparsable_2 ("123:necessaryunnecessary");
  BOOST_CHECK_EQUAL(parse_insert_table_name(unparsable_2), "-1");
}

BOOST_AUTO_TEST_CASE(test_trim_insert_query)
{
  std::string untrimmed ("123:table_name;data");
  BOOST_CHECK_EQUAL(trim_insert_query(untrimmed), "data");

  std::string untrimmed_2 ("123:table_name,data");
  BOOST_CHECK_EQUAL(trim_insert_query(untrimmed_2), "-1");
}

BOOST_AUTO_TEST_CASE(test_parse_insert_query)
{
  std::string unparsed ("one;two;three");
  std::vector<std::string> string_vec_expected{"one", "two", "three"};
  BOOST_CHECK(validate_string_vec(parse_insert_query(unparsed), string_vec_expected));

  std::string unparsed_2 ("one;two");
  std::vector<std::string> string_vec_expected_2{"one", "two"};
  BOOST_CHECK(validate_string_vec(parse_insert_query(unparsed_2), string_vec_expected_2));

  std::string unparsed_3 ("one");
  std::vector<std::string> string_vec_expected_3{"one"};
  BOOST_CHECK(validate_string_vec(parse_insert_query(unparsed_3), string_vec_expected_3));
}

BOOST_AUTO_TEST_SUITE_END()
