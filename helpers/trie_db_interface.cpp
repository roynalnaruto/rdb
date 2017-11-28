/**
 * This is the interface to access
 * cpp-ethereum trie db
 */

void insert(int id, std::string name) {
  Person person;
  person.set_person(id, name);
  t.insert(int_to_bytes(id), obj_to_bytes(&person));
}

std::string lookup(int id) {
  if (t.contains(int_to_bytes(id))) {
    return bytes_to_string(&t.at(int_to_bytes(id))[0]);
  } else {
    return "\n";
  }
}
