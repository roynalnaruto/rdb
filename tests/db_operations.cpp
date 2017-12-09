#define BOOST_TEST_MODULE test_db_operations
#include <boost/test/included/unit_test.hpp>
#include "../libdevcore/TrieDB.h"
#include "../libdevcore/MemoryDB.h"

using byte = uint8_t;

using namespace dev;

typedef std::map<std::string, int> table_col_info;
typedef std::pair<std::string, int> table_col_info_pair;
table_col_info tables_info;

#include "../classes/template_db.cpp"
#include "../utils/utils.cpp"
#include "../helpers/trie_db_interface.cpp"

BOOST_AUTO_TEST_SUITE(test_db_operations)

BOOST_AUTO_TEST_CASE(test_table2)
{
  t.init();
  create_table("two_column_table", 2);
  insert("two_column_table", 1, "col_2_row_1");
  insert("two_column_table", 2, "col_2_row_2");
  insert("two_column_table", 3, "col_2_row_3");
  BOOST_CHECK_EQUAL(lookup("two_column_table", 2, 1), "1, col_2_row_1");
  BOOST_CHECK_EQUAL(lookup("two_column_table", 2, 2), "2, col_2_row_2");
  BOOST_CHECK_EQUAL(lookup("two_column_table", 2, 3), "3, col_2_row_3");
  BOOST_CHECK_EQUAL(lookup("two_column_table", 2, 4), "\n");
}

BOOST_AUTO_TEST_CASE(test_table3)
{
  t.init();
  create_table("three_column_table", 3);
  insert("three_column_table", 1, "col_2_row_1", "col_3_row_1");
  insert("three_column_table", 2, "col_2_row_2", "col_3_row_2");
  insert("three_column_table", 3, "col_2_row_3", "col_3_row_3");
  BOOST_CHECK_EQUAL(lookup("three_column_table", 3, 1), "1, col_2_row_1, col_3_row_1");
  BOOST_CHECK_EQUAL(lookup("three_column_table", 3, 2), "2, col_2_row_2, col_3_row_2");
  BOOST_CHECK_EQUAL(lookup("three_column_table", 3, 3), "3, col_2_row_3, col_3_row_3");
  BOOST_CHECK_EQUAL(lookup("three_column_table", 3, 4), "\n");
}

BOOST_AUTO_TEST_CASE(test_table4)
{
  t.init();
  create_table("four_column_table", 3);
  insert("four_column_table", 1, "col_2_row_1", "col_3_row_1", "col_4_row_1");
  insert("four_column_table", 2, "col_2_row_2", "col_3_row_2", "col_4_row_2");
  insert("four_column_table", 3, "col_2_row_3", "col_3_row_3", "col_4_row_3");
  BOOST_CHECK_EQUAL(lookup("four_column_table", 4, 1), "1, col_2_row_1, col_3_row_1, col_4_row_1");
  BOOST_CHECK_EQUAL(lookup("four_column_table", 4, 2), "2, col_2_row_2, col_3_row_2, col_4_row_2");
  BOOST_CHECK_EQUAL(lookup("four_column_table", 4, 3), "3, col_2_row_3, col_3_row_3, col_4_row_3");
  BOOST_CHECK_EQUAL(lookup("four_column_table", 4, 4), "\n");
}

BOOST_AUTO_TEST_CASE(test_table5)
{
  t.init();
  create_table("five_column_table", 3);
  insert("five_column_table", 1, "col_2_row_1", "col_3_row_1", "col_4_row_1", "col_5_row_1");
  insert("five_column_table", 2, "col_2_row_2", "col_3_row_2", "col_4_row_2", "col_5_row_2");
  insert("five_column_table", 3, "col_2_row_3", "col_3_row_3", "col_4_row_3", "col_5_row_3");
  BOOST_CHECK_EQUAL(lookup("five_column_table", 5, 1), "1, col_2_row_1, col_3_row_1, col_4_row_1, col_5_row_1");
  BOOST_CHECK_EQUAL(lookup("five_column_table", 5, 2), "2, col_2_row_2, col_3_row_2, col_4_row_2, col_5_row_2");
  BOOST_CHECK_EQUAL(lookup("five_column_table", 5, 3), "3, col_2_row_3, col_3_row_3, col_4_row_3, col_5_row_3");
  BOOST_CHECK_EQUAL(lookup("five_column_table", 5, 4), "\n");
}

BOOST_AUTO_TEST_SUITE_END()
