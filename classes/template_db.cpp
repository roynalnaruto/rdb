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

template<class FirstColumn, class... Columns>
class Table : private FirstColumn, private Columns... {
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
    return (std::to_string(*FirstColumn::doGet()) + ... + (", " + *Columns::doGet()));
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

// restrict: 4 types of tables
// can define more, but at compile time should know table structure
// cannot do runtime
using Table2 = Table<Column<int>, Column<std::string>>;
using Table3 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>>;
using Table4 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>, Column<std::string, Key3>>;
using Table5 = Table<Column<int>, Column<std::string, Key1>, Column<std::string, Key2>, Column<std::string, Key3>, Column<std::string, Key4>>;
