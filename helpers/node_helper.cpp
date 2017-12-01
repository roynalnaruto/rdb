/**
 * helper functions for node class
 */

struct node;
io_service service;
typedef boost::shared_ptr<node> node_ptr;
typedef std::vector<node_ptr> array;
array nodes;
boost::recursive_mutex cs;

std::string create_table_helper(std::string msg) {
  int n_columns = std::stoi(parse_int(msg));
  std::string table_name = parse_name(msg);
  std::cout<<"creating table: "<<table_name<<" with "<<n_columns<<" columns"<<std::endl;
  boost::recursive_mutex::scoped_lock lk(cs);
  create_table(table_name, n_columns);
  return table_name;
}

std::string lookup_helper(std::string msg) {
  int id = std::stoi(parse_int(msg));
  std::string table_name = parse_name(msg);
  std::cout<<"looking up "<<id<<" in table "<<table_name<<std::endl;
  boost::recursive_mutex::scoped_lock lk(cs);
  return lookup(table_name, tables_info.at(table_name), id);
}

std::string insert_row_helper(std::string msg) {
  int id = std::stoi(parse_int(msg));
  std::string table_name = parse_insert_table_name(msg);
  std::vector<std::string> columns = parse_insert_query(trim_insert_query(msg));
  int col_len = columns.size();

  boost::recursive_mutex::scoped_lock lk(cs);
  std::string response = "\n";
  // our restriction on columns in table
  if (col_len<1 && col_len>5) {
    return response;
  }
  switch(col_len) {
    case 1:
      response = insert(table_name, id, columns.at(0));
      break;
    case 2:
      response = insert(table_name, id, columns.at(0), columns.at(1));
      break;
    case 3:
      response = insert(table_name, id, columns.at(0), columns.at(1), columns.at(2));
      break;
    case 4:
      response = insert(table_name, id, columns.at(0), columns.at(1), columns.at(2), columns.at(3));
      break;
    default:
      break;
  }
  return response;
}

std::string list_tables_helper() {
  std::string response("");
  for(std::map<std::string, int>::iterator it = tables_info.begin(); it != tables_info.end(); ++it) {
    response += "(" + it->first + ", " + std::to_string(it->second) + ")";
  }
  return response;
}
