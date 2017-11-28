/**
 * This is the interface to access
 * cpp-ethereum trie db
 */

MemoryDB db;
GenericTrieDB<MemoryDB> t(&db);

std::string insert(std::string table_name, int id, std::string col1) {
  Table2 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string>(std::make_shared<std::string>(col1));
  t.insert(obj_to_bytes(get_row_key(table_name, id)), obj_to_bytes(&row));
  return row.get_row();
}

std::string insert(std::string table_name, int id, std::string col1, std::string col2) {
  Table3 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  t.insert(obj_to_bytes(get_row_key(table_name, id)), obj_to_bytes(&row));
  return row.get_row();
}

std::string insert(std::string table_name, int id, std::string col1, std::string col2, std::string col3) {
  Table4 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  row.set<std::string, Key3>(std::make_shared<std::string>(col3));
  t.insert(obj_to_bytes(get_row_key(table_name, id)), obj_to_bytes(&row));
  return row.get_row();
}

std::string insert(std::string table_name, int id, std::string col1, std::string col2, std::string col3, std::string col4) {
  Table5 row;
  row.set<int>(std::make_shared<int>(id));
  row.set<std::string, Key1>(std::make_shared<std::string>(col1));
  row.set<std::string, Key2>(std::make_shared<std::string>(col2));
  row.set<std::string, Key3>(std::make_shared<std::string>(col3));
  row.set<std::string, Key4>(std::make_shared<std::string>(col4));
  t.insert(obj_to_bytes(get_row_key(table_name, id)), obj_to_bytes(&row));
  return row.get_row();
}

std::string lookup(std::string table_name, int n_col, int id) {
  if (t.contains(obj_to_bytes(get_row_key(table_name, id)))) {
    return bytes_to_string(&t.at(obj_to_bytes(get_row_key(table_name, id)))[0], n_col);
  } else {
    return "\n";
  }
}

void create_table(std::string name, int n_col) {
  tables_info.insert( table_col_info_pair(name, n_col) );
}
