/**
 * This is the person class for Table Person (int, std::string)
 */

class Person {
  int id_;
  std::string name_;

public:
  void set_person(int, std::string);
  std::string get_person();
};

void Person::set_person(int id, std::string name) {
  id_ = id;
  name_ = name;
}

std::string Person::get_person() {
  std::string result("");
  result += std::to_string(id_);
  result += ", " + name_;
  return result;
}

