/**
 * This is utils file for all utility functions
 * conversion between bytes and string/object
 */

std::string EMPTY_STR("\n");

std::string get_row_key(std::string table_name, int id) {
  return (table_name + ":" + std::to_string(id));
}

std::vector<unsigned char> obj_to_bytes(std::string str) {
  std::vector<unsigned char> buffer(str.begin(), str.end());
  return buffer;
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

std::string bytes_to_string(unsigned char* buffer, int n_col) {
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

std::string parse_to_clean(std::string msg) {
  if (msg.find("/") != -1) {
    return msg.substr(msg.find("/")+1, std::string::npos);
  }
  return "_";
}

std::string parse_int(std::string msg) {
  size_t found_delimeter;
  std::string id("-1");
  if ((found_delimeter = msg.find(":")) != std::string::npos) {
    id = msg.substr(0, found_delimeter);
  }
  return id;
}

std::string parse_name(std::string msg) {
  size_t found_delimeter;
  std::string name("-1");
  if ((found_delimeter = msg.find(":")) != std::string::npos) {
    name =  msg.substr(found_delimeter+1, std::string::npos);
  }
  return name;
}

std::string parse_insert_table_name(std::string msg) {
  size_t found_delimeter_1, found_delimeter_2;
  std::string name("-1");
  if (((found_delimeter_1 = msg.find(":")) != std::string::npos) && (((found_delimeter_2 = msg.find(";")) != std::string::npos))) {
    name = msg.substr(found_delimeter_1+1, found_delimeter_2-found_delimeter_1-1);
  }
  return name;
}

std::string trim_insert_query(std::string msg) {
  size_t found_delimeter;
  std::string query("-1");
  if ((found_delimeter = msg.find(";")) != std::string::npos) {
    query = msg.substr(found_delimeter+1, std::string::npos);
  }
  return query;
}

std::vector<std::string> parse_insert_query(std::string query) {
  std::vector<std::string> vect;
  std::stringstream ss(query);
  std::string col;
  while(std::getline(ss, col, ';')) {
    vect.push_back(col);
  }
  return vect;
}
