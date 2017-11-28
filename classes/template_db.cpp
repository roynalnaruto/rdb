#include <iostream>
#include <string>
#include <mutex>

struct ColumnKey;

template <class Type, class Key = ColumnKey>
class Column {
protected:
  std::shared_ptr<Type> doGet() {
    std::lock_guard<std::mutex> lock(mutex_);
    return std::make_shared<Type>(value_);
  }
  void doSet(const std::shared_ptr<Type> &value) {
    std::lock_guard<std::mutex> lock(mutex_);
    value_ = *value;
  }
private:
  Type value_;
  std::mutex mutex_;
};

template<class... Columns>
class Table : private Columns... {
public:
  template<class Type, class Key = ColumnKey>
  std::shared_ptr<Type> get() {
    return Column<Type, Key>::doGet();
  }

  template<class Type, class Key = ColumnKey>
  void set(const std::shared_ptr<Type> &value) {
    Column<Type, Key>::doSet(value);
  }

  std::string get_row() {
    return "do something about this";
  }
};

// restrict: table can support 4 columns (other than id)
// can be any type (std::string taken only for example)
// how to construct type by input from user ?
// structure required at compile time
struct Key1;
struct Key2;
struct Key3;
struct Key4;

// restrict: 3 types of tables
// can define more, but at compile time should know table structure
using Table2 = Table<Column<int>, Column<std::string>>;
using Table3 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>>;
using Table4 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>, Column<std::string, Key3>>;
using Table5 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>, Column<std::string, Key3>, Column<std::string, Key4>>;

/*
int main() {
//  Table2 table2;
//  table2.set<int>(std::make_shared<int>(1));
//  table2.set<std::string>(std::make_shared<std::string>("hello_2"));

  Table3 table3;
  table3.set<int>(std::make_shared<int>(1));
  table3.set<std::string, Key1>(std::make_shared<std::string>("hello_3a"));
  table3.set<std::string, Key2>(std::make_shared<std::string>("hello_3b"));

//  Table4 table4;
//  table4.set<int>(std::make_shared<int>(1));
//  table4.set<std::string, Key1>(std::make_shared<std::string>("hello_4a"));
//  table4.set<std::string, Key2>(std::make_shared<std::string>("hello_4b"));
//  table4.set<std::string, Key3>(std::make_shared<std::string>("hello_4c"));


  std::cout<<*table3.get<int>()<<std::endl;
  std::cout<<*table3.get<std::string, Key1>()<<std::endl;
  std::cout<<*table3.get<std::string, Key2>()<<std::endl;
  std::cout<<table3.test_out()<<std::endl;
  return 0;
}
*/
