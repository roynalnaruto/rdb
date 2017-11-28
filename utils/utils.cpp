/**
 * This is utils file for all utility functions
 * conversion between bytes and string/object
 */

std::string EMPTY_STR("\n");

std::vector<unsigned char> int_to_bytes(int param) {
  std::vector<unsigned char> array_of_byte(4);
  for (int i = 0; i < 4; i++)
    array_of_byte[3 - i] = (param >> (i * 8));
  return array_of_byte;
}

std::vector<unsigned char> obj_to_bytes(Person* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::string bytes_to_string(char* buffer) {
  auto person_deserialized = reinterpret_cast<Person*>(buffer);
  return (*person_deserialized).get_person();
}

