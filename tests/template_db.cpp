#define BOOST_TEST_MODULE template_db_test
#include <boost/test/included/unit_test.hpp>

using byte = uint8_t;

#include "../classes/template_db.cpp"

BOOST_AUTO_TEST_SUITE(template_db_test)

BOOST_AUTO_TEST_CASE(test_table2)
{
  Table2 table;
  table.set<int>(std::make_shared<int>(1));
  table.set<std::string>(std::make_shared<std::string>("col_1"));
  BOOST_CHECK_EQUAL(*table.get<int>(), 1);
  BOOST_CHECK(*table.get<std::string>() == "col_1");
  BOOST_CHECK_EQUAL(table.get_row(), "1, col_1");
}

BOOST_AUTO_TEST_CASE(test_table3)
{
  Table3 table;
  table.set<int>(std::make_shared<int>(1));
  table.set<std::string, Key1>(std::make_shared<std::string>("col_1"));
  table.set<std::string, Key2>(std::make_shared<std::string>("col_2"));
  BOOST_CHECK_EQUAL(*table.get<int>(), 1);
  BOOST_CHECK((*table.get<std::string, Key1>()) == "col_1");
  BOOST_CHECK((*table.get<std::string, Key2>()) == "col_2");
  BOOST_CHECK_EQUAL(table.get_row(), "1, col_1, col_2");
}

BOOST_AUTO_TEST_CASE(test_table4)
{
  Table4 table;
  table.set<int>(std::make_shared<int>(1));
  table.set<std::string, Key1>(std::make_shared<std::string>("col_1"));
  table.set<std::string, Key2>(std::make_shared<std::string>("col_2"));
  table.set<std::string, Key3>(std::make_shared<std::string>("col_3"));
  BOOST_CHECK_EQUAL(*table.get<int>(), 1);
  BOOST_CHECK((*table.get<std::string, Key1>()) == "col_1");
  BOOST_CHECK((*table.get<std::string, Key2>()) == "col_2");
  BOOST_CHECK((*table.get<std::string, Key3>()) == "col_3");
  BOOST_CHECK_EQUAL(table.get_row(), "1, col_1, col_2, col_3");
}

BOOST_AUTO_TEST_CASE(test_table5)
{
  Table5 table;
  table.set<int>(std::make_shared<int>(1));
  table.set<std::string, Key1>(std::make_shared<std::string>("col_1"));
  table.set<std::string, Key2>(std::make_shared<std::string>("col_2"));
  table.set<std::string, Key3>(std::make_shared<std::string>("col_3"));
  table.set<std::string, Key4>(std::make_shared<std::string>("col_4"));
  BOOST_CHECK_EQUAL(*table.get<int>(), 1);
  BOOST_CHECK((*table.get<std::string, Key1>()) == "col_1");
  BOOST_CHECK((*table.get<std::string, Key2>()) == "col_2");
  BOOST_CHECK((*table.get<std::string, Key3>()) == "col_3");
  BOOST_CHECK((*table.get<std::string, Key4>()) == "col_4");
  BOOST_CHECK_EQUAL(table.get_row(), "1, col_1, col_2, col_3, col_4");
}

BOOST_AUTO_TEST_SUITE_END()
