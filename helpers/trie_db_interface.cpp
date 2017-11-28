/**
 * This is the interface to access
 * cpp-ethereum trie db
 */
void insert(GenericTrieDB<MemoryDB>* t, int id, std::string name) {
  Table2 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string>(std::make_shared<std::string>(name));
  (*t).insert(int_to_bytes(id), obj_to_bytes(&row));
}

void insert(GenericTrieDB<MemoryDB>* t, int id, std::string col1, std::string col2) {
  Table3 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  (*t).insert(int_to_bytes(id), obj_to_bytes(&row));
}

void insert(GenericTrieDB<MemoryDB>* t, int id, std::string col1, std::string col2, std::string col3) {
  Table4 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  row.set<std::string, Key3>(std::make_shared<std::string>(col3));
  (*t).insert(int_to_bytes(id), obj_to_bytes(&row));
}

void insert(GenericTrieDB<MemoryDB>* t, int id, std::string col1, std::string col2, std::string col3, std::string col4) {
  Table5 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  row.set<std::string, Key3>(std::make_shared<std::string>(col3));
  row.set<std::string, Key4>(std::make_shared<std::string>(col4));
  (*t).insert(int_to_bytes(id), obj_to_bytes(&row));
}

std::string lookup(GenericTrieDB<MemoryDB>* t, int n_col, int id) {
  if ((*t).contains(int_to_bytes(id))) {
    return bytes_to_string(&(*t).at(int_to_bytes(id))[0], n_col);
  } else {
    return "\n";
  }
}

void create_table(std::string name, int n_col) {
  MemoryDB db;
  GenericTrieDB<MemoryDB> t(&db);
  tables.insert( table_map_pair(name, &t) );
  tables_info.insert( table_col_info_pair(name, n_col) );
}
