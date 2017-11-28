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

std::vector<unsigned char> obj_to_bytes(Table2* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::vector<unsigned char> obj_to_bytes(Table3* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::vector<unsigned char> obj_to_bytes(Table4* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::vector<unsigned char> obj_to_bytes(Table5* p) {
  auto ptr = reinterpret_cast<byte*>(p);
  auto buffer = std::vector<byte>{ptr, ptr + sizeof(*p)};
  return buffer;
}

std::string bytes_to_string(char* buffer, int n_col) {
  if (n_col == 2) {
    auto row_deserialized = reinterpret_cast<Table2*>(buffer);
    return (*row_deserialized).get_row();
  } else if (n_col == 3) {
    auto row_deserialized = reinterpret_cast<Table3*>(buffer);
    return (*row_deserialized).get_row();
  } else if (n_col == 4) {
    auto row_deserialized = reinterpret_cast<Table4*>(buffer);
    return (*row_deserialized).get_row();
  } else if (n_col == 5) {
    auto row_deserialized = reinterpret_cast<Table5*>(buffer);
    return (*row_deserialized).get_row();
  } else return "\n";
}

